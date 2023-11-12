#include "cc_sens_dht.h"
#include "cc_pins.h"
#include "cc_util.h"

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
float cc::sens_dht::indice_calor(){
  return dht.computeHeatIndex();
}
void cc::sens_dht::envia_banco() {
  float umidade_atual = umidade();
  float temperatura_atual = temperatura();

  if (!isnan(umidade_atual) && !isnan(temperatura_atual)) {
    String consulta_sql = "INSERT INTO SensorData (umidade, temperatura) VALUES (" + String(umidade_atual) + ", " + String(temperatura_atual) + ")";
    consulta_banco(consulta_sql);
    delay(30000);
  } else {
    Serial.println("Erro: Leituras do sensor DHT nulas.");
  }
}