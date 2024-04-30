#ifndef CC_CLINO_H
#define CC_CLINO_H
#include "cc_Modulo.h"

#include <Arduino.h>
#include <Stream.h>
#include <StreamString.h>

#include <string>
#include <map>

namespace cc
{
    class Clino : public Modulo
    {
    public:
        struct Comando
        {
            typedef String (*Func)(StreamString&);

            Comando(Func func, String descricao = "", String manual = "")
                :func(func), descricao(descricao), manual(manual)
            {

            }

            const Func func;
            const String descricao = "";
            const String manual = "";
        };

    public:
        Clino(Stream& stream, const String& idModulo);

    private:
        void aoIniciar() override;
        static void tarefa(void* pv_args);
        void executar();

    public:
        static std::map<std::string, Comando> comandos;

        Stream& stream;
    };
}

#endif