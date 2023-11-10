#include "cc_sens_reserv.h"
#include "cc_pins.h"
#include <Arduino.h>

float cc::sens_reserv::f_valor = 0.0f;
char cc::sens_reserv::ligado = 0;
cc::sens_reserv cc::sens_reserv::unico;

void cc::sens_reserv::iniciar() {
  pinMode(PIN::SENS_RESERV, INPUT);
  pinMode(PIN::LIGA_SENS_RESERV, OUTPUT);
  xTaskCreate(tarefa_leitura, "leitura_sens_reserv", 2048, nullptr, 1, nullptr);
}
bool cc::sens_reserv::vazio() {
  return f_valor > gatilho;
}
void cc::sens_reserv::ligar(int espera_n_amostras) {
  ligado++;
  if (espera_n_amostras)
    delay(espera_n_amostras * dt);
}
void cc::sens_reserv::desligar() {
  ligado -= (ligado > 0);
}
void cc::sens_reserv::tarefa_leitura(void* pv_args) {
  while (1) {
    while (!ligado) delay(10);

    digitalWrite(PIN::LIGA_SENS_RESERV, HIGH);
    delay(1);
    f_valor = (float)analogRead(PIN::SENS_RESERV) / 4095.0f;
    while (ligado) {
      delay(dt);
      f_valor += ((float)analogRead(PIN::SENS_RESERV) / 4095.0f - f_valor) * peso;
    }
    digitalWrite(PIN::LIGA_SENS_RESERV, LOW);
  }
}