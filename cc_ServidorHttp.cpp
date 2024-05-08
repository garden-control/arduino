#include "cc_ServidorHttp.h"
#include "cc_RecursosServidorHttp.h"

namespace cc
{
    ServidorHttp ServidorHttp::unico;

    ServidorHttp::ServidorHttp()
        : Modulo("ServidorHttp", {"wifi", "Spiffs"}), servidor(80)
    {
    }

    void ServidorHttp::aoIniciar()
    {
        StreamString arq;
        char buffer[1024];
        buffer[1023] = 0;
        if (cc::lerArquivo(SPIFFS, caminhoConfig, (uint8_t*)buffer, 1023))
        {
            arq.print(buffer);
            ssid = arq.readStringUntil('\n');
            senha = arq.readStringUntil('\n');
        }

        WiFi.softAP(ssid, senha);

        servidor.begin();

        xTaskCreatePinnedToCore(tarefa, id.c_str(), 8000, this, 2, NULL, 1 /* deve ser core 1 para lidar com rede wifi */);
    }

    void ServidorHttp::tarefa(void *pvArgs)
    {
        ((ServidorHttp *)pvArgs)->tarefa();
    }

    void ServidorHttp::tarefa()
    {
        while (1)
        {
            delay(1);

            mtxFilaDeTarefas.capturar();
            while (!filaDeTarefas.empty())
            {
                filaDeTarefas.front()();
                filaDeTarefas.pop();
            }
            mtxFilaDeTarefas.liberar();

            WiFiClient cliente = servidor.available();
            
            if (cliente)
            {        
                Serial.println("Novo cliente");
                
                while (1)
                {
                    if (cliente.available())
                    {
                        RequisicaoHttp req(cliente);

                        enviarRespostaHttp(cliente, req);

                        DEBUG_SERIAL("Cabecalho %i ===============");
                        DEBUG_SERIAL("Cru:\n\n");
                        DEBUG_SERIAL("%s\n", req.cabecalho.c_str());
                        DEBUG_SERIAL("Metodo: \"%s\"\n", req.metodo.c_str());
                        DEBUG_SERIAL("Rota:   \"%s\"\n", req.rota.c_str());
                        if (req.contentLength)
                        {
                            DEBUG_SERIAL("contentType:   \"%s\"\n", req.contentType.c_str());
                            DEBUG_SERIAL("contentLength: \"%s\"\n\n", req.contentLength);
                            DEBUG_SERIAL("Corpo =====================");
                            DEBUG_SERIAL("%s\n", req.pegarCorpo());
                        }
                        else Serial.println("Requisicao sem corpo.");

                        break;
                    }
                }
                cliente.stop();
            }
        }
    }

    void ServidorHttp::configPontoAcesso(const String& ssid, const String& senha)
    {
        unico.ssid = ssid;
        unico.senha = senha;
        
        unico.mtxFilaDeTarefas.capturar();
        unico.filaDeTarefas.push(reiniciarPontoAcesso);
        unico.mtxFilaDeTarefas.liberar();

        //salvar configuracoes
        StreamString config;
        config.println(ssid);
        config.println(senha);

        cc::escreverArquivo(SPIFFS, caminhoConfig, (uint8_t*)config.c_str(), config.length());
    }

    void ServidorHttp::reiniciarPontoAcesso()
    {
        unico.servidor.stop();
        WiFi.softAP(unico.ssid, unico.senha);
        unico.servidor.begin();
    }


    void ServidorHttp::enviarRespostaHttp(WiFiClient& cliente, const RequisicaoHttp& req)
    {
        if (req.metodo == "GET" && (req.rota == "/" || req.rota == "/index.html"))
        {
            cliente.printf(
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %i\r\n"
                "Connection: close\r\n\r\n",
                getLength(indexHtml)
            );
            cliente.print(indexHtml);
        }
        else if (req.metodo == "POST" && req.rota == "/comando")
        {
            // TODO
        }
        else
        {
            cliente.print(
                "HTTP/1.1 404 Not Found\r\n"
                "accept-ranges: bytes\r\n"
                "Connection: close\r\n"
                "Content-Length: 0\r\n"
            );
        }
    }

};