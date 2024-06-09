#include <Arduino.h>
#include "cc_controlador_solo.h"
#include "cc_pins.h"
#include "cc_sens_solo.h"
#include "cc_sens_reserv.h"
#include "cc_util.h"
#include "cc_ClimaFire.h"

cc::controlador_solo cc::controlador_solo::unico;
float cc::controlador_solo::f_umidade_max = 0.55f;
float cc::controlador_solo::f_umidade_min = 0.4f;
bool cc::controlador_solo::pausa = true;
bool cc::controlador_solo::bomba = false;

cc::controlador_solo::controlador_solo()
    : Modulo("controlador_solo", {"Serial"})
{
}

void cc::controlador_solo::tarefa_controle(void *pv_args)
{
    while (1)
    {
        while (pausa)
            delay(10);
        while (!pausa)
        {
            sens_solo::liga(10);
            if ((sens_solo::umidade() < f_umidade_min || (bomba && sens_solo::umidade() < f_umidade_max)) && sens_reserv::nivel() > 0)
            {
                // atualizar status
                if (!bomba)
                {
                    cc::ClimaFire::set("variaveis/controlador_bomba", true);
                }

                digitalWrite(PIN::LIGA_BOMBA, HIGH);
                int nivel_reservatorio = 0;
                while (!pausa && sens_solo::umidade() < f_umidade_max && (nivel_reservatorio = sens_reserv::nivel()))
                {
                    delay(10);
                }
                digitalWrite(PIN::LIGA_BOMBA, LOW);

                // atualizar status
                bomba = false;
                cc::ClimaFire::set("variaveis/controlador_bomba", false);
            }
            else if (bomba)
            {
                // atualizar status
                bomba = false;
                cc::ClimaFire::set("variaveis/controlador_bomba", false);
            }
            sens_solo::desliga();
            // espera 5 segundos checando para "pausa" e "bomba" a cada 100ms (para facilitar demonstração em bancada)
            for (int espera = 5 * 1000, parte = 100, cumprido = 0; cumprido < espera && !pausa && !bomba; cumprido += parte)
                delay(parte);
        }
    }
}

void cc::controlador_solo::aoIniciar()
{
    pinMode(PIN::LIGA_BOMBA, OUTPUT);
    xTaskCreate(tarefa_controle, "controle_solo", 8000, nullptr, 1, nullptr);
}

void cc::controlador_solo::aoReceberEvento(const String& idModulo, const String& evento)
{
    if (idModulo == "ClimaFire" && evento == "PREPARAR_GET")
    {
        iniciar();
        cc::ClimaFire::inscreverParaEventoRTDB(aoEventoRTDB, this);
    }
}

void cc::controlador_solo::aoEventoRTDB(JsonVariant var, void* pvArgs)
{
    ((controlador_solo*)pvArgs)->aoEventoRTDB(var);
}

void cc::controlador_solo::aoEventoRTDB(JsonVariant var)
{
    if (var.containsKey("controlador_ativo"))
    {
        bool bAtivo = var["controlador_ativo"];
        pausa = !bAtivo;
        DEBUG_SERIAL("[controldaor_solo] \"pausa\" configurado para \"%s\".\n", pausa ? "true" : "false");
    }
    if (var.containsKey("controlador_bomba"))
    {
        bomba = var["controlador_bomba"];
        DEBUG_SERIAL("[controldaor_solo] \"bomba\" configurado para \"%s\".\n", bomba ? "true" : "false");
    }
    if (var.containsKey("controlador_umidade_max"))
    {
        f_umidade_max = var["controlador_umidade_max"];
        DEBUG_SERIAL("[controldaor_solo] \"f_umidade_max\" configurado para %.2f.\n", f_umidade_max);
    }
    if (var.containsKey("controlador_umidade_min"))
    {
        f_umidade_min = var["controlador_umidade_min"];
        DEBUG_SERIAL("[controldaor_solo] \"f_umidade_mi;n\" configurado para %.2f.\n", f_umidade_min);
    }
}