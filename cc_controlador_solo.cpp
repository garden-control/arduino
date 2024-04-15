#include <Arduino.h>
#include "cc_controlador_solo.h"
#include "cc_pins.h"
#include "cc_sens_solo.h"
#include "cc_sens_reserv.h"
#include "cc_util.h"
#include "cc_clino.h"

cc::controlador_solo cc::controlador_solo::unico;
float cc::controlador_solo::f_umidade_max = 0.55f;
float cc::controlador_solo::f_umidade_min = 0.4f;
bool cc::controlador_solo::pausa = true;

void cc::controlador_solo::tarefa_controle(void* pv_args) {
  while (1) {
    while (pausa) delay(10);
    while (!pausa) {
      sens_solo::liga(10);
      sens_reserv::liga(10);
      if (sens_solo::umidade() < f_umidade_min && !sens_reserv::vazio()) {
        consulta_banco("insert into SensorReservatorio (EstadoBomba, SituacaoReservatorio) values (1, 1)");
        //Serial.println(String("[Controlador] ") + erro);
        digitalWrite(PIN::LIGA_BOMBA, HIGH);
        bool situacao_reservatorio = true;
        while (!pausa && sens_solo::umidade() < f_umidade_max && (situacao_reservatorio = !sens_reserv::vazio())) {
          delay(10);
        }
        digitalWrite(PIN::LIGA_BOMBA, LOW);
        
        consulta_banco(String("insert into SensorReservatorio (EstadoBomba, SituacaoReservatorio) values (0, ") + (situacao_reservatorio ? '1' : '0') + ')');
        //Serial.println(String("[Controlador] ") + erro);
      }
      sens_solo::desliga();
      sens_reserv::desliga();
      //espera 5 segundos checando para "pausa" a cada 100ms (para facilitar demonstração em bancada)
      for (int espera = 5 * 1000, parte = 100, cumprido = 0; cumprido < espera && !pausa; cumprido += parte)
        delay(parte);
    }
  }
}

void cc::controlador_solo::iniciar() {
  pinMode(PIN::LIGA_BOMBA, OUTPUT);
  xTaskCreate(tarefa_controle, "controle_solo", 2048, nullptr, 1, nullptr);

  Clino::comandos.emplace("ctrlPausar", Clino::Comando(clinoPausar, nullptr, "Pausa/inicia o controlador"));
  Clino::comandos.emplace("ctrlStatus", Clino::Comando(clinoStatus, nullptr, "Checa o stauts do controlador"));
  Clino::comandos.emplace("ctrlSetMin", Clino::Comando(clinoSetMin, nullptr, "Configura a umidade mínima permitida"));
  Clino::comandos.emplace("ctrlSetMax", Clino::Comando(clinoSetMax, nullptr, "Configura a umidade máxima permitida"));
}

String cc::controlador_solo::clinoPausar(StreamString& entrada, void* pv_args)
{
  pausa = !pausa;
  return cc::Json()
    .add("pausa", pausa)
    .toString();
}
String cc::controlador_solo::clinoStatus(StreamString& entrada, void* pv_args)
{
  cc::sens_solo::liga(10);
  float umidade = cc::sens_solo::umidade();
  cc::sens_solo::desliga();
  return cc::Json()
    .add("pausa", pausa)
    .add("reservatorio", !cc::sens_reserv::vazio())
    .add("umidade", umidade)
    .add("minima", f_umidade_min)
    .add("maxima", f_umidade_max)
    .toString();
}
String cc::controlador_solo::clinoSetMin(StreamString& entrada, void* pv_args)
{
  return "";
}
String cc::controlador_solo::clinoSetMax(StreamString& entrada, void* pv_args)
{
  return "";
}