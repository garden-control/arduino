#ifndef CC_REQUISICAO_HTTP_H
#define CC_REQUISICAO_HTTP_H

#include <WiFiClient.h>

namespace cc
{
    class RequisicaoHttp
    {
    public:
        RequisicaoHttp(WiFiClient& cliente);
        ~RequisicaoHttp();

        const char* pegarCorpo() const { return corpo; }

        String cabecalho;
        String metodo;
        String rota;
        String contentType;
        int contentLength = 0;
    private:
        char* corpo = nullptr;
    };
};

#endif