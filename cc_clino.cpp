#include "cc_clino.h"
#include <StreamString.h>
#include "cc_util.h"
#include <map>
#include <string>

using namespace cc;

std::map<std::string, Clino::Comando> Clino::comandos = {};

Clino::Clino(Stream& stream, bool echoInput)
    :stream(stream), echoInput(echoInput)
{

}

void Clino::iniciar()
{
    xTaskCreate(tarefa, "", 2048, this, 1, nullptr);
}

void Clino::tarefa(void* pv_args)
{
    Clino& clino = *(Clino*)pv_args;

    clino.executar();
}

void Clino::executar()
{
    StreamString entrada;

    while (1)
    {
        //ler nova entrada
        if (entrada.length() == 0)
        {
            while (entrada.length() == 0) 
            {
                entrada.print(espera_linha(stream));
                entrada.trim();
            }
            if (echoInput)
                stream.printf("> %s\n", entrada.c_str());
        }
        
        String identificador = entrada.readString();
        
        auto it = comandos.find(identificador.c_str());

        if (it != comandos.end())
        {
            String saida = it->second.func(entrada, it->second.pv_args);
            stream.printf("%s\n\n", saida.c_str());
        }
        else if (identificador == "ajuda")
        {
            for (auto& cmd : comandos)
            {
                stream.printf("%-20s%s\n", cmd.first.c_str(), cmd.second.descricao.c_str());
            }
            stream.printf(
                "\najuda\n"
                "manual\n\n"
            );
        }
    }
}