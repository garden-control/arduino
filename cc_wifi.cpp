#include "cc_wifi.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include "cc_util.h"
#include "cc_globais.h"
#include "cc_credenciais.h"

cc::wifi cc::wifi::unico;
String cc::wifi::ssid;
String cc::wifi::senha;

cc::wifi::wifi()
    :
    Modulo("wifi", {"Spiffs"})
{
    
}
void cc::wifi::aoIniciar()
{
    mtx_spiffs.capturar();
    File arq = SPIFFS.open(caminho);
    if (arq) {
      ssid = arq.readStringUntil('\n');
      senha = arq.readStringUntil('\n');
      WiFi.begin(ssid, senha);
    }
    arq.close();
    mtx_spiffs.liberar();
    
    WiFi.begin(WIFI_SSID, WIFI_SENHA);
}
bool cc::wifi::conectado()
{
    return WiFi.status() == WL_CONNECTED;
}