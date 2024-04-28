#include <Arduino.h>
#include "cc_controlador_solo.h"
#include "cc_pins.h"
#include "cc_sens_solo.h"
#include "cc_sens_reserv.h"
#include "cc_util.h"

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

        //atualizar status

        digitalWrite(PIN::LIGA_BOMBA, HIGH);
        bool situacao_reservatorio = true;
        while (!pausa && sens_solo::umidade() < f_umidade_max && (situacao_reservatorio = !sens_reserv::vazio())) {
          delay(10);
        }
        digitalWrite(PIN::LIGA_BOMBA, LOW);
        
        //atualizar status
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
}