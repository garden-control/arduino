#ifndef CC_CLIMAFIRE_H
#define CC_CLIMAFIRE_H
#include "cc_inicializacao.h"
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "cc_credenciais.h"

namespace cc
{
    class ClimaFire : inicializavel
    {
        // conta firebase de teste
        static constexpr char* apiKey = "AIzaSyBBjB1jujY5_5UFUAXuteksVzAyGAcgZLU";
        static constexpr char* databaseUrl = "https://clima-care-default-rtdb.firebaseio.com/";
        static constexpr char* usuarioEmail = FIREBASE_ESP32_EMAIL;
        static constexpr char* usuarioSenha = FIREBASE_ESP32_SENHA;

        static ClimaFire unico;

        WiFiClientSecure sslClient;
        DefaultNetwork defaultNetwork;
        AsyncClientClass asyncClient;
        UserAuth userAuth;
        FirebaseApp firebaseApp;

        RealtimeDatabase database;

        ClimaFire();

        void iniciar() override;

        static void tarefa(void* pvArgs);
        void tarefa();

        static void firebaseCallback(AsyncResult& result);

    public:
        static bool pronto();

        template <typename T>
        static void set(const String& caminho, const T& val)
        {
            unico.database.set(unico.asyncClient, caminho, val, firebaseCallback);
        }
    };
}

#endif