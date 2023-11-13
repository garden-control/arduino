#include <Arduino.h>
#include "cc_controlador_solo.h"
#include "cc_pins.h"
#include "cc_sens_solo.h"
#include "cc_sens_reserv.h"
#include "cc_util.h"

cc::controlador_solo cc::controlador_solo::unico;
float cc::controlador_solo::f_umidade_max = 0.7f;
float cc::controlador_solo::f_umidade_min = 0.4f;
bool cc::controlador_solo::pausa = true;

void cc::controlador_solo::tarefa_controle(void* pv_args) {
  while (1) {
    while (pausa) delay(10);
    while (!pausa) {
      sens_solo::liga(10);
      sens_reserv::liga(10);
      if (sens_solo::umidade() < f_umidade_min && !sens_reserv::vazio()) {
        consulta_banco("Insert into SensorReservatorio (EstadoBomba, SituacaoReservatorio) values (1, 1)");
        digitalWrite(PIN::LIGA_BOMBA, HIGH);
        bool situacao_reservatorio = true;
        while (!pausa && sens_solo::umidade() < f_umidade_max && (situacao_reservatorio = !sens_reserv::vazio())) {
          delay(10);
        }
        consulta_banco("INSERT INTO SensorReservatorio (EstadoBomba, SituacaoReservatorio) VALUES (0, " + String(situacao_reservatorio ? '0' : '1') + ")");
        digitalWrite(PIN::LIGA_BOMBA, LOW);
      }
      sens_solo::desliga();
      sens_reserv::desliga();
      //espera 5 minutos checando para "pausa" a cada 100ms (para facilitar demonstração em bancada)
      for (int espera = 5 * 60 * 1000, parte = 100, cumprido = 0; cumprido < espera && !pausa; cumprido += parte)
        delay(parte);
    }
  }
}

void cc::controlador_solo::iniciar() {
  pinMode(PIN::LIGA_BOMBA, OUTPUT);
  xTaskCreate(tarefa_controle, "controle_solo", 2048, nullptr, 1, nullptr);
}