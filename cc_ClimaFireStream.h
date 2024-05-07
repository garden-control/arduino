#ifndef CC_CLIMAFIRESTREAM_H
#define CC_CLIMAFIRESTREAM_H
#include "cc_Modulo.h"
#include "cc_mutex.h"
#include <ArduinoJson.h>
#include <Stream.h>
#include <StreamString.h>

namespace cc
{
    class ClimaFireStream : Modulo, public Stream
    {
    public:
        ClimaFireStream(const String& caminho);

        void aoIniciar() override;
    
    private:
        static void tarefa(void* pvArgs);
        void tarefa();
        static void tratadorDeEventoRTDB(JsonVariant var, void* pvArgs);
        void tratadorDeEventoRTDB(JsonVariant var);
    public:
        int available() override;
        int read() override;
        int peek() override;
        size_t write(uint8_t byte) override;

    public:
        const String caminho;

    private:
        StreamString streamEntrada, streamSaida;
        mutex mtxEntrada, mtxSaida;
        unsigned long long ultimaEscrita = 0;
    };
};

#endif