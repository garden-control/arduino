#include "cc_sens_dht.h"
#include "cc_pins.h"

cc::sens_dht cc::sens_dht::unico;
DHT cc::sens_dht::dht(PIN::SENS_DHT, DHT11);

cc::sens_dht::sens_dht()
    :
    Modulo("sens_dht")
{

}
void cc::sens_dht::aoIniciar() {
  dht.begin();
}
float cc::sens_dht::umidade() {
  return dht.readHumidity();
}
float cc::sens_dht::temperatura() {
  return dht.readTemperature();
}
float cc::sens_dht::indice_calor() {
  return dht.computeHeatIndex();
}