#include "cc_ClimaFire.h"
#include "cc_wifi.h"
#include "cc_credenciais.h"
#include "cc_util.h"

#include <ArduinoJson.h>

cc::ClimaFire cc::ClimaFire::unico;

cc::ClimaFire::ClimaFire()
    :
    Modulo("ClimaFire", {"wifi", "Serial"}),
    defaultNetwork(false),
    userAuth(apiKey, usuarioEmail, usuarioSenha, 3000),
    asyncClient(sslClient, getNetwork(defaultNetwork))
{
    sslClient.setInsecure();
}

void cc::ClimaFire::aoIniciar()
{
    xTaskCreatePinnedToCore(tarefa, "ClimaFire", 8000, this, 3, NULL, 1 /* must be core 1 for network task */); 
}

void cc::ClimaFire::tarefa(void* pvArgs)
{
    ((ClimaFire*)pvArgs)->tarefa();
}
void cc::ClimaFire::tarefa()
{
    while (!cc::wifi::conectado()) delay(500);
    
    firebaseApp.setCallback(firebaseCallback);
    initializeApp(asyncClient, firebaseApp, getAuth(userAuth));

    long long unsigned int ms = millis();
    while (firebaseApp.isInitialized() && !firebaseApp.ready() && millis() - ms < 120 * 1000);

    firebaseApp.getApp<RealtimeDatabase>(database);
    database.url(databaseUrl);

    database.get(asyncClient, "/", firebaseCallback, true);

    while (1)
    {
        firebaseApp.loop();
        database.loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void cc::ClimaFire::firebaseCallback(AsyncResult& result)
{
    if (result.appEvent().code() > 0)
    {
        Serial.printf("Event msg: %s, code: %d\n", result.appEvent().message().c_str(), result.appEvent().code());
    }

    if (result.isDebug())
    {
        Serial.printf("Debug msg: %s\n", result.debug().c_str());
    }

    if (result.isError())
    {
        Serial.printf("Error msg: %s, code: %d\n", result.error().message().c_str(), result.error().code());
        /*
        if (result.error().code() == -1)
        {
            Serial.println("Reiniciando Firebase...");
            initializeApp(unico.asyncClient, unico.firebaseApp, getAuth(unico.userAuth));
        }
        */
    }

    if (result.available())
    {
        String json = result.payload();
        json = json.substring(json.indexOf('{'), json.lastIndexOf('}') + 1);

        JsonDocument doc;
        deserializeJson(doc, json);

        if (doc.containsKey("data") && doc.containsKey("path"))
        {
            const char* caminho = doc["path"];
            
            unico.encaminharEventoRTDB(unico.tratadoresDeEventoRTDB[caminho], doc["data"]);
        }
    }
}

bool cc::ClimaFire::pronto()
{
    return unico.firebaseApp.ready();
}

void cc::ClimaFire::inscreverParaEventoRTDB(String caminho, TratadorDeEventoRTDB tratador)
{
    unico.tratadoresDeEventoRTDB[caminho.c_str()].valor = tratador;
}

void cc::ClimaFire::imprimirArv()
{
    unico.tratadoresDeEventoRTDB.imprimir();
}

void cc::ClimaFire::encaminharEventoRTDB(ArvoreDeCaminho<TratadorDeEventoRTDB>& arv, JsonVariant jsonVar)
{
    if (!arv.mapa.empty())
    {
        for (auto& [chave, valor] : arv.mapa)
        {
            if (jsonVar.containsKey(chave.c_str()))
            {
                encaminharEventoRTDB(valor, jsonVar[chave.c_str()]);
            }
        }
    }
    if (arv.valor)
    {
        arv.valor(jsonVar);
    }
}