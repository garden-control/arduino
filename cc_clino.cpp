#include "cc_clino.h"
#include <StreamString.h>
#include "cc_util.h"
#include <map>
#include <string>

using namespace cc;

std::map<std::string, Clino::Comando> Clino::comandos = {};

Clino::Clino(Stream& stream, const String& idModulo)
    :Modulo(idModulo), stream(stream)
{

}

void Clino::aoIniciar()
{
    xTaskCreate(tarefa, id.c_str(), 8000, this, 1, nullptr);
}

void Clino::tarefa(void* pv_args)
{
    Clino& clino = *(Clino*)pv_args;

    clino.executar();
}

void Clino::executar()
{
    StreamString entrada;
    entrada.setTimeout(0);
    
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

            stream.printf("> %s\n", entrada.c_str());
        }
        
        String identificador = entrada.readStringUntil(' ');
        
        auto it = comandos.find(identificador.c_str());

        if (it != comandos.end())
        {
            stream.print(it->second.func(entrada));
        }
        else if (identificador == "ajuda")
        {
            for (auto& cmd : comandos)
            {
                stream.printf("%-20s%s\n", cmd.first.c_str(), cmd.second.descricao.c_str());
            }
            stream.printf(
                "ajuda\n"
                "manual\n\n"
            );
        }
        else
        {
            stream.printf("Comando \"%s\" nao existe\n\n", identificador.c_str());
        }
    }
}