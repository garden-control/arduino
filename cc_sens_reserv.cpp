#include "cc_sens_reserv.h"
#include "cc_pins.h"
#include <Arduino.h>

cc::sens_reserv cc::sens_reserv::unico;

cc::sens_reserv::sens_reserv()
    : Modulo("sens_reserv", {"ClimaFire"})
{
}
void cc::sens_reserv::aoIniciar()
{
    pinMode(PIN::SENS_RESERV, INPUT);
    pinMode(PIN::LIGA_SENS_RESERV, OUTPUT);
    digitalWrite(PIN::LIGA_SENS_RESERV, HIGH);

    cc::ClimaFire::inscreverParaEventoRTDB(aoEventoRTDB, this);
    xTaskCreate(tarefa, id.c_str(), 8000, nullptr, 1, nullptr);
}
int cc::sens_reserv::nivel()
{
    return unico.nivelAgua;
}
void cc::sens_reserv::tarefa(void*)
{
    while (1)
    {
        float x = (float)analogRead(PIN::SENS_RESERV) / 4096.0f;
        int i = 4;
        float min_erro = abs(x - (1.0f / 4.0f));
        for (int j = 3; j >= 1; j--)
        {
            float erro = abs(x - (1.0f / (float)j));
            if (erro < min_erro)
            {
                min_erro = erro;
                i = j;
            }
        }
        int novoNivel = 4 - i;
        if (novoNivel != unico.nivelAgua && cc::ClimaFire::pronto())
        {
            DEBUG_SERIAL("[sens_reserv] Atualizando nivel\n");
            cc::ClimaFire::set("variaveis/reserv_nivel", novoNivel);
            unico.nivelAgua = novoNivel;
        }
        delay(1000);
    }
}
void cc::sens_reserv::aoEventoRTDB(JsonVariant var, void*)
{
    if (var.containsKey("reserv_nivel"))
    {
        int nivelRtdb = var["reserv_nivel"];
        if (nivelRtdb != unico.nivelAgua && cc::ClimaFire::pronto())
        {
            DEBUG_SERIAL("[sens_reserv] Atualizando nivel\n");
            cc::ClimaFire::set("variaveis/reserv_nivel", unico.nivelAgua);
        }
    }
}