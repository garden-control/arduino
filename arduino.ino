#include "cc_Modulo.h"
#include "cc_wifi.h"
#include "cc_sens_dht.h"
#include "cc_sens_solo.h"
#include "cc_pluviometro.h"
#include "cc_clino.h"
#include "cc_globais.h"
#include "cc_util.h"
#include "cc_ClimaFire.h"
#include "cc_util.h"
#include "cc_mutex.h"

#include <ArduinoJson.h>

#include <list>

cc::Clino cliSerial(Serial, "cliSerial");

//cc::ClimaFireStream cliStream("cli");

unsigned long long intervaloLeituras = 30000; //ms
void aoEventoRTDB_leituras(JsonVariant var, void*)
{
    if (var.containsKey("leituras_intervalo_s"))
    {
        int novoIntervalo_s = var["leituras_intervalo_s"];
        if (novoIntervalo_s >= 30)
        {
            intervaloLeituras = (long long)novoIntervalo_s * 1000LL;
            DEBUG_SERIAL("[Leituras] Intervalo configurado para %i segundos.\n", novoIntervalo_s);
        }
        else
        {
            DEBUG_SERIAL("[Leituras] Intervalo de leituras invalido (%i).\n", novoIntervalo_s);
            cc::ClimaFire::set("variaveis/leituras_intervalo_s", (int)(intervaloLeituras / 1000));
        }
    }
}

void setup()
{
    cc::ClimaFire::inscreverParaEventoRTDB(aoEventoRTDB_leituras);
    cc::iniciarModulos();
}

struct leitura
{
    float temperatura, umidade, umidade_solo, indice_pluv;
    int count;
};
std::list<leitura> leituras;
long long int tp1 = 0, tp2 = 0;
cc::mutex mtxLeituras;

bool tentandoEnviar(bool set = false, bool val = true)
{
    static bool bEnviando = false;
    static unsigned long long uUltimoSet = 0;
    if (set)
    {
        bEnviando = val;
        if (bEnviando)
            uUltimoSet = millis();
    }
    else if (uUltimoSet + 40000 < millis())
    {
        DEBUG_SERIAL("[Leituras] Ultima tentativa de envio expirou.\n");
        bEnviando = false;
        uUltimoSet = millis();
    }
    return bEnviando;
}

void pushCallback(AsyncResult& result)
{
    static String ultimoInserido = "a";
    auto rtdbResult = result.to<RealtimeDatabaseResult>();

    if (!rtdbResult.name().isEmpty() && rtdbResult.name() != ultimoInserido)
    {
        ultimoInserido = rtdbResult.name();
        DEBUG_SERIAL("[pushCallback] Envio bem sucedido (%s). Enviando proxima leitura\n", rtdbResult.name().c_str());
        mtxLeituras.capturar();
        leituras.erase(leituras.begin());
        mtxLeituras.liberar();
    }
    else if (rtdbResult.name().isEmpty())
    {
        DEBUG_SERIAL("[pushCallback] Envio falhou\n");
    }
    tentandoEnviar(true, false);
}

void push()
{
    static cc::mutex mtxPush;
    
    mtxPush.capturar();
    mtxLeituras.capturar();

    if (!leituras.empty() && !tentandoEnviar())
    {
        const auto &l = *leituras.begin();

        JsonDocument doc;
        doc["pluviometro"] = l.indice_pluv;
        doc["temperatura"] = l.temperatura;
        doc["umidade_ar"] = l.umidade;
        doc["umidade_solo"] = l.umidade_solo * 100.0f;
        doc["count"] = l.count;
        mtxLeituras.liberar();

        String jsonSerial;
        serializeJson(doc, jsonSerial);

        // enviar dados
        cc::ClimaFire::pushLeituras(number_t(jsonSerial), pushCallback);
        
        tentandoEnviar(true);
    }
    else
    {
        mtxLeituras.liberar();
    }

    mtxPush.liberar();
}

void loop()
{
    static unsigned long long proximaLeitura = 0;
    
    static int count = 0;
    if (proximaLeitura < millis())
    {
        proximaLeitura += intervaloLeituras;
        
        cc::sens_solo::liga(10);
        mtxLeituras.capturar();
        leituras.push_back({
            cc::sens_dht::temperatura(),
            cc::sens_dht::umidade(),
            cc::sens_solo::umidade(),
            cc::pluv::indice_mm(),
            count
        });
        mtxLeituras.liberar();
        cc::sens_solo::desliga();
        DEBUG_SERIAL("[Leituras] Leitura coletada (%i).\n", count);
        DEBUG_SERIAL("[Leituras] Status: %i && %i && %i && %i\n", !leituras.empty(), !tentandoEnviar(), cc::wifi::conectado(), cc::ClimaFire::pronto());
        count++;
    }
    if (!leituras.empty() && !tentandoEnviar() && cc::wifi::conectado() && cc::ClimaFire::pronto())
    {
        DEBUG_SERIAL("[Leituras] Tentando enviar... (tam buffer: %i)\n", leituras.size());
        push();
    }
    delay(1);
}
