#include "cc_sens_dht.h"
#include "cc_pins.h"

cc::sens_dht cc::sens_dht::unico;
DHT cc::sens_dht::dht(PIN::SENS_DHT, DHT11);

void cc::sens_dht::iniciar() {
  dht.begin();
}
float cc::sens_dht::umidade() {
  return dht.readHumidity();
}
float cc::sens_dht::temperatura() {
  return dht.readTemperature();
}