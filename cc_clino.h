#ifndef CC_CLINO_H
#define CC_CLINO_H
#include "cc_inicializacao.h"
#include <Arduino.h>
#include <Stream.h>
#include <StreamString.h>
#include <string>
#include <map>

namespace cc
{
    class Clino : public inicializavel
    {
    public:
        struct Comando
        {
            typedef String (*Func)(StreamString&, void*);

            Comando(Func func, void* pv_args = nullptr, String descricao = "", String manual = "")
                :func(func), pv_args(pv_args), descricao(descricao), manual(manual)
            {

            }

            const Func func;
            void *const pv_args = nullptr;
            const String descricao = "";
            const String manual = "";
        };

    public:
        Clino(Stream& stream, bool echoInput = true);

    private:
        void iniciar() override;
        static void tarefa(void* pv_args);
        void executar();

    public:
        static std::map<std::string, Comando> comandos;

    private:


    public:
        Stream& stream;
        bool echoInput;

    private:

    };
}

#endif