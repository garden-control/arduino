#include "cc_Modulo.h"
#include "cc_wifi.h"
#include "cc_sens_dht.h"
#include "cc_sens_solo.h"
#include "cc_pluviometro.h"
#include "cc_clino.h"
#include "cc_globais.h"
#include "cc_util.h"
#include "cc_ClimaFire.h"

#include <list>

cc::Clino cliSerial(Serial, "cliSerial");

void getCb(JsonVariant var)
{
    if (var.is<int>())
        Serial.print("Yep. It's an int: ");
    serializeJsonPretty(var, Serial);
    Serial.println();
}

void setup()
{
    cc::iniciarModulos();

    cc::ClimaFire::inscreverParaEventoRTDB("teste/string", getCb);
    cc::ClimaFire::inscreverParaEventoRTDB("teste/int", getCb);
    cc::ClimaFire::inscreverParaEventoRTDB("teste", getCb);
    cc::ClimaFire::inscreverParaEventoRTDB("", getCb);


    delay(5000);
    //cc::ClimaFire::imprimirArv();
}

struct leitura
{
    float temperatura, umidade, umidade_solo, indice_pluv;
};
std::list<leitura> leituras;
long long int tp1 = 0, tp2 = 0;


void loop()
{
    /*
    cc::sens_solo::liga(10);
    leituras.push_back({
        cc::sens_dht::temperatura(),
        cc::sens_dht::umidade(),
        cc::sens_solo::umidade(),
        cc::pluv::indice_mm() // fazer...
    });
    cc::sens_solo::desliga();

    if (cc::wifi::conectado())
    {
        char sql[512];
        for (auto it = leituras.begin(); it != leituras.end();)
        {
            const auto &l = *it;
            sprintf(
                sql,
                "insert into SensorData (localizacao, temperatura, umidade, SensorSolo, alturaReservPluv) values (\"Sala\", \"%.2f\", \"%.2f\", \"%.2f\", \"%.2f\")",
                l.temperatura,
                l.umidade,
                l.umidade_solo * 100.0f,
                l.indice_pluv);
            // enviar dados
        }
    }
    tp2 = millis();
    delay(max(0LL, 5 * 1000 - (tp2 - tp1)));
    tp1 = millis();
    */

    delay(1000);
}
