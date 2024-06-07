#ifndef CC_SERVIDOR_HTTP_H
#define CC_SERVIDOR_HTTP_H

#include "cc_Modulo.h"
#include "cc_util.h"
#include "cc_mutex.h"
#include "cc_RequisicaoHttp.h"
#include <queue>
#include <WiFi.h>

namespace cc
{
    class ServidorHttp : Modulo
    {
    private:
        static constexpr char* caminhoConfig = "/wifiAp.txt";
        String ssid = "Estacao ClimaCare", senha = "administrador";

        ServidorHttp();

        void aoIniciar() override;

        static void tarefa(void* pvArgs);

        void tarefa();

    public: 
        static void configPontoAcesso(const String& ssid, const String& senha);
    private: 
        static void reiniciarPontoAcesso();

        void enviarRespostaHttp(WiFiClient& cliente, const RequisicaoHttp& req);

    private:
        static ServidorHttp unico;
        WiFiServer servidor;
        typedef void (*TarefaDeFila)();
        std::queue<TarefaDeFila> filaDeTarefas;
        mutex mtxFilaDeTarefas;
    };
};

#endif