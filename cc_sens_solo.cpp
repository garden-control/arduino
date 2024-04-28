#include "cc_sens_solo.h"
#include "cc_pins.h"
#include <Arduino.h>

float cc::sens_solo::f_umidade = 0.0f;
char cc::sens_solo::ligado = 0;
cc::mutex cc::sens_solo::mtx_ligado;

cc::sens_solo cc::sens_solo::unico;
void cc::sens_solo::iniciar() {
  pinMode(PIN::SENS_SOLO, INPUT);
  pinMode(PIN::LIGA_SENS_SOLO, OUTPUT);
  xTaskCreate(tarefa_leitura, "leitura_sens_solo", 2048, nullptr, 1, nullptr);
}
float cc::sens_solo::umidade() {
  return 1.0f - f_umidade;
}
void cc::sens_solo::liga(int espera_n_amostras) {
  mtx_ligado.capturar();
  ligado++;
  mtx_ligado.liberar();
  if (espera_n_amostras)
    delay(espera_n_amostras * dt);
}
void cc::sens_solo::desliga() {
  mtx_ligado.capturar();
  ligado -= (ligado > 0);
  mtx_ligado.liberar();
}

void cc::sens_solo::tarefa_leitura(void* pv_args) {
  while (1) {
    while (!ligado) delay(10);
    digitalWrite(PIN::LIGA_SENS_SOLO, HIGH);
    delay(1);
    f_umidade = (float)analogRead(PIN::SENS_SOLO) / 4095.0f;
    while (ligado) {
      delay(dt);
      f_umidade += ((float)analogRead(PIN::SENS_SOLO) / 4095.0f - f_umidade) * peso;
    }
    digitalWrite(PIN::LIGA_SENS_SOLO, LOW);
  }
}