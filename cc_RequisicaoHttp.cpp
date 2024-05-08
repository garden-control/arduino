#include "cc_RequisicaoHttp.h"

namespace cc
{
    RequisicaoHttp::RequisicaoHttp(WiFiClient &cliente)
    {
        while (cliente.connected())
        {
            if (cliente.available())
            {
                char ch = cliente.read();
                if (ch != '\r')
                    cabecalho += ch;
                if (cabecalho.endsWith("\n\n"))
                    break;
            }
        }

        metodo = cabecalho.substring(0, cabecalho.indexOf(' '));

        rota = cabecalho.substring(cabecalho.indexOf('/'), cabecalho.indexOf(' ', cabecalho.indexOf('/')));

        int idxContentLength = cabecalho.indexOf("Content-Length");
        if (idxContentLength != -1)
        {
            contentType =
                cabecalho.substring(
                    cabecalho.indexOf(':', cabecalho.indexOf("Content-Type")) + 2,
                    cabecalho.indexOf('\n', cabecalho.indexOf("Content-Type")));

            contentLength =
                cabecalho.substring(
                             cabecalho.indexOf(':', idxContentLength) + 2,
                             cabecalho.indexOf('\n', idxContentLength))
                    .toInt();

            corpo = new char[contentLength + 1];
            corpo[contentLength] = 0;

            for (int i = 0; cliente.connected() && i < contentLength;)
            {
                if (cliente.available())
                {
                    corpo[i] = cliente.read();
                    i++;
                }
            }
        }
    }
    RequisicaoHttp::~RequisicaoHttp()
    {
        if (corpo)
            delete corpo;
    }
};