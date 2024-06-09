#ifndef CC_CLIMAFIRE_H
#define CC_CLIMAFIRE_H
#include "cc_Modulo.h"
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "cc_credenciais.h"
#include "cc_util.h"
#include "cc_mutex.h"
#include <map>
#include <string>
#include <ArduinoJson.h>
#include <memory>
#include <vector>

typedef void (*FirebaseClientCallback)(AsyncResult&);

namespace cc
{
    class ClimaFire : Modulo
    {
        // projeto firebase gardening-control
        static constexpr char* apiKey = "AIzaSyCPDc7vDgHwq4ELUj8285P0l3FmmDy8sNY";
        static constexpr char* databaseUrl = "https://gardening-control-default-rtdb.firebaseio.com/";
        static constexpr char* usuarioEmail = FIREBASE_ESP32_EMAIL;
        static constexpr char* usuarioSenha = FIREBASE_ESP32_SENHA;

    public:
        static ClimaFire unico;

        class PacoteFirebase
        {
        public:
            PacoteFirebase();

            void iniciar();

            DefaultNetwork defaultNetwork;
            
            UserAuth userAuth;

            FirebaseApp firebaseApp;
            WiFiClientSecure sslClient;
            AsyncClientClass asyncClient;

            RealtimeDatabase database;
            WiFiClientSecure sslClientGet;
            AsyncClientClass asyncClientGet;
        } *pPacoteFirebase = nullptr;
        mutex mtxPacoteFirebase;

        bool bReiniciarWiFi = false;

        struct TratadorDeEventoRTDB
        {
            typedef void (*Metodo)(JsonVariant doc, void* pvArgs);
            Metodo metodo;
            void* pvArgs = nullptr;
        };

        std::vector<std::unique_ptr<TratadorDeEventoRTDB>> tratadoresDeEventoRTDB;
    
    private:
        ClimaFire();

        void aoIniciar() override;

        static void tarefa(void* pvArgs);
        void tarefa();

        static void firebaseCallback(AsyncResult& result);

    public:
        static bool pronto();

        template <typename T>
        static void set(const String& caminho, const T& val)
        {
            if (unico.pPacoteFirebase)
            {
                unico.mtxPacoteFirebase.capturar();
                unico.pPacoteFirebase->database.set(unico.pPacoteFirebase->asyncClient, "/usuarios/TzLMoL0JrcMkX1JNVCXHSds8x3J2/estacoes/0/" + caminho, val, firebaseCallback);
                unico.mtxPacoteFirebase.liberar();
            }
        }

        template <typename T>
        static void pushLeituras(const T& val, AsyncResultCallback cb = firebaseCallback)
        {
            if (unico.pPacoteFirebase)
            {
                unico.mtxPacoteFirebase.capturar();
                unico.pPacoteFirebase->database.push(unico.pPacoteFirebase->asyncClient, "/usuarios/TzLMoL0JrcMkX1JNVCXHSds8x3J2/estacoes/0/leituras", val, cb);
                unico.mtxPacoteFirebase.liberar();
            }
        }

        static void inscreverParaEventoRTDB(TratadorDeEventoRTDB::Metodo tratador, void* pvArgs = nullptr);
    };
}

#endif