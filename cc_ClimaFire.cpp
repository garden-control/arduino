#include "cc_ClimaFire.h"
#include "cc_wifi.h"
#include "cc_credenciais.h"
#include "cc_util.h"

cc::ClimaFire cc::ClimaFire::unico;

cc::ClimaFire::ClimaFire()
    :
    userAuth(apiKey, usuarioEmail, usuarioSenha, 3000),
    asyncClient(sslClient, getNetwork(defaultNetwork)),
    asyncClientGet(sslClientGet, getNetwork(defaultNetwork))
{
    sslClient.setInsecure();
    sslClientGet.setInsecure();
}

void cc::ClimaFire::iniciar()
{
    xTaskCreatePinnedToCore(tarefa, "ClimaFire", 8000, this, 3, NULL, 1 /* must be core 1 for network task */); 
}

void cc::ClimaFire::tarefa(void* pvArgs)
{
    ((ClimaFire*)pvArgs)->tarefa();
}
void cc::ClimaFire::tarefa()
{
    while (!wifi::conectado()) delay(100);
    
    firebaseApp.setCallback(firebaseCallback);
    initializeApp(asyncClient, firebaseApp, getAuth(userAuth));

    long long unsigned int ms = millis();
    while (firebaseApp.isInitialized() && !firebaseApp.ready() && millis() - ms < 120 * 1000);

    firebaseApp.getApp<RealtimeDatabase>(database);
    database.url(databaseUrl);

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
        if (result.error().code() == -1)
        {
            Serial.println("Reiniciando Firebase...");
            initializeApp(unico.asyncClient, unico.firebaseApp, getAuth(unico.userAuth));
        }
    }

    if (result.available())
    {
        Serial.printf("payload: %s\n", result.c_str());
    }
}

bool cc::ClimaFire::pronto()
{
    return unico.firebaseApp.ready();
}

void cc::ClimaFire::get(const String& caminho, FirebaseClientCallback callback)
{
    unico.database.get(unico.asyncClientGet, caminho, callback, true);
}