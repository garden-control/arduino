#include "cc_wifi.h"
#include <SPIFFS.h>
#include "cc_util.h"
#include "cc_globais.h"
#include "cc_credenciais.h"

cc::wifi cc::wifi::unico;
String cc::wifi::ssid;
String cc::wifi::senha;

cc::wifi::wifi()
    : Modulo("wifi", {"Spiffs"})
{
}
void cc::wifi::aoIniciar()
{
    WiFi.onEvent(aoEventoWifi);

    mtx_spiffs.capturar();
    File arq = SPIFFS.open(caminho);
    if (arq)
    {
        ssid = arq.readStringUntil('\n');
        senha = arq.readStringUntil('\n');
        if (!ssid.isEmpty())
        {
            DEBUG_SERIAL("[WiFi] Conectando a rede \"%s\"...\n", ssid.c_str());
            
            WiFi.begin(ssid, senha);
        }
        else
        {
            DEBUG_SERIAL("[WiFi] SSID vazio.\n");
        }
    }
    else
    {
        DEBUG_SERIAL("[WiFi] Nao foi possivel abrir o aqruivo de configuracao wifi.\n");
    }
    arq.close();
    mtx_spiffs.liberar();
}
void cc::wifi::aoEventoWifi(WiFiEvent_t evento)
{
    switch (evento)
    {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        DEBUG_SERIAL("[WiFi] Conectado a rede \"%s\"\n", ssid.c_str());

        unico.transmitirEvento("CONECTADO");
        break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        DEBUG_SERIAL("[WiFi] Conexao perdida. Reconectando a rede \"%s\"\n", ssid.c_str());

        unico.transmitirEvento("DESCONECTADO");

        WiFi.begin(ssid, senha);
        break;
    }
}
bool cc::wifi::conectado()
{
    return WiFi.status() == WL_CONNECTED;
}
void cc::wifi::configurar(const String &ssid, const String &senha)
{
    unico.ssid = ssid;
    unico.senha = senha;

    mtx_spiffs.capturar();
    File arq = SPIFFS.open(caminho, FILE_WRITE);
    if (arq)
    {
        arq.printf("%s\n%s\n", ssid.c_str(), senha.c_str());

        DEBUG_SERIAL("[WiFi] Configuracoes wifi salvas.\n");
    }
    else
    {
        DEBUG_SERIAL("[WiFi] Nao foi possivel abrir o arquivo para salvar as configuracoes wifi.\n");
    }
    mtx_spiffs.liberar();

    
    if (conectado())
    {
        WiFi.disconnect();
    }
    else
    {
        WiFi.begin(ssid, senha);
    }    
}