#include "cc_sens_reserv.h"
#include "cc_pins.h"
#include <Arduino.h>

cc::sens_reserv cc::sens_reserv::unico;

cc::sens_reserv::sens_reserv()
    :Modulo("sens_reserv")
{

}
void cc::sens_reserv::aoIniciar() {
  pinMode(PIN::SENS_RESERV, INPUT);
  pinMode(PIN::LIGA_SENS_RESERV, OUTPUT);
  digitalWrite(PIN::LIGA_SENS_RESERV, HIGH);
}
int cc::sens_reserv::nivel()
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
  return 4 - i;
}
