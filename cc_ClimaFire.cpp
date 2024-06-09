#include "cc_ClimaFire.h"
#include "cc_wifi.h"
#include "cc_credenciais.h"
#include "cc_util.h"

#include <ArduinoJson.h>

cc::ClimaFire cc::ClimaFire::unico;

cc::ClimaFire::ClimaFire()
    :
    Modulo("ClimaFire", {"wifi", "Serial"})
{
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
    transmitirEvento("PREPARAR_GET");
    pPacoteFirebase = new PacoteFirebase();
    pPacoteFirebase->iniciar();
    while (1)
    {
        if (bReiniciarWiFi)
        {
            bReiniciarWiFi = false;
            WiFi.reconnect();
        }
        pPacoteFirebase->firebaseApp.loop();
        pPacoteFirebase->database.loop();
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
        
        
        if (result.error().code() == -1)
        {
            Serial.println("Reiniciando WiFi...");
            unico.bReiniciarWiFi = true;
        }
    }

    if (result.available())
    {
        String json = result.payload();
        json = json.substring(json.indexOf('{'), json.lastIndexOf('}') + 1);

        JsonDocument doc;
        deserializeJson(doc, json);

        if (doc.containsKey("data") && doc.containsKey("path"))
        {
            JsonDocument doc2;
            String path = doc["path"];
            path = path.substring(1);

            if (!path.isEmpty()) doc2[path.c_str()] = doc["data"];
            else doc2 = doc["data"];

            for (auto& tratador : unico.tratadoresDeEventoRTDB)
            {
                tratador->metodo(doc2, tratador->pvArgs);
            }
        }
    }
}

bool cc::ClimaFire::pronto()
{
    unico.mtxPacoteFirebase.capturar();
    bool res = unico.pPacoteFirebase && unico.pPacoteFirebase->firebaseApp.ready();
    unico.mtxPacoteFirebase.liberar();
    return res;
}

void cc::ClimaFire::inscreverParaEventoRTDB(TratadorDeEventoRTDB::Metodo tratador, void* pvArgs)
{
    unico.tratadoresDeEventoRTDB.push_back(std::unique_ptr<TratadorDeEventoRTDB>(new TratadorDeEventoRTDB()));
    unico.tratadoresDeEventoRTDB.back()->metodo = tratador;
    unico.tratadoresDeEventoRTDB.back()->pvArgs = pvArgs;
}

cc::ClimaFire::PacoteFirebase::PacoteFirebase()
    :
    defaultNetwork(false),
    userAuth(apiKey, usuarioEmail, usuarioSenha, 3000),
    asyncClient(sslClient, getNetwork(defaultNetwork)),
    asyncClientGet(sslClientGet, getNetwork(defaultNetwork))
{
    sslClient.setInsecure();
    sslClientGet.setInsecure();
}
void cc::ClimaFire::PacoteFirebase::iniciar()
{
    DEBUG_SERIAL("[Pacote Firebase] Aguardando Wi-Fi para iniciar...\n");
    while (!cc::wifi::conectado()) delay(500);
    
    firebaseApp.setCallback(firebaseCallback);
    initializeApp(asyncClient, firebaseApp, getAuth(userAuth));

    long long unsigned int ms = millis();
    while (firebaseApp.isInitialized() && !firebaseApp.ready() && millis() - ms < 120 * 1000);

    firebaseApp.getApp<RealtimeDatabase>(database);
    database.url(databaseUrl);

    database.get(asyncClientGet, "/usuarios/TzLMoL0JrcMkX1JNVCXHSds8x3J2/estacoes/0/variaveis", firebaseCallback, true);
}