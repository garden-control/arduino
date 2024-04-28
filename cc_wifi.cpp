#include "cc_wifi.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include "cc_util.h"

cc::wifi cc::wifi::unico;
String cc::wifi::ssid;
String cc::wifi::senha;

void cc::wifi::iniciar() {
  mtx_spiffs.capturar();
  File arq = SPIFFS.open(caminho);
  if (arq) {
    ssid = arq.readStringUntil('\n');
    senha = arq.readStringUntil('\n');
    WiFi.begin(ssid, senha);
  }
  arq.close();
  mtx_spiffs.liberar();
}
bool cc::wifi::conectado() {
  return WiFi.status() == WL_CONNECTED;
}