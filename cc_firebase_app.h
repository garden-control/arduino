#ifndef CC_FIREBASE_APP_H
#define CC_FIREBASE_APP_H
#include "cc_inicializacao.h"
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "cc_credenciais.h"

namespace cc
{
    class FirebaseApp : inicializavel
    {
        static constexpr char* apiKey = "AIzaSyCPDc7vDgHwq4ELUj8285P0l3FmmDy8sNY";
        static constexpr char* databaseUrl = "https://clima-care-default-rtdb.firebaseio.com/";
        static constexpr char* usuarioEmail = FIREBASE_ESP32_EMAIL;
        static constexpr char* usuarioSenha = FIREBASE_ESP32_SENHA;
        static FirebaseApp unico;

        WiFiClientSecure sslClient;
        DefaultNetwork defaultNetwork;
        AsyncClientClass asyncClient;

        FirebaseApp();

        void iniciar() override;
    };
}

#endif