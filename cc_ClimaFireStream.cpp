#include "cc_ClimaFireStream.h"
#include "cc_ClimaFire.h"

cc::ClimaFireStream::ClimaFireStream(const String& caminho)
    : Modulo("ClimaFireStream:" + caminho), caminho(caminho)
{
    streamSaida.setTimeout(0);
    streamEntrada.setTimeout(0);
}

void cc::ClimaFireStream::aoIniciar()
{
    ClimaFire::inscreverParaEventoRTDB(caminho + "/entrada", tratadorDeEventoRTDB, this);
    xTaskCreate(tarefa, id.c_str(), 8000, this, 1, nullptr);
}

void cc::ClimaFireStream::tarefa(void* pvArgs)
{
    ((ClimaFireStream*)pvArgs)->tarefa();
}

void cc::ClimaFireStream::tarefa()
{
    while (1)
    {
        if (streamSaida.available() && millis() - ultimaEscrita > 1000 && cc::ClimaFire::pronto())
        {
            mtxSaida.capturar();
            String saida;
            while (streamSaida.available()) 
                saida += (char)streamSaida.read();
            mtxSaida.liberar();

            cc::ClimaFire::set(caminho + "/saida", saida);
        }
        delay(1000);
    }
}

void cc::ClimaFireStream::tratadorDeEventoRTDB(JsonVariant var, void* pvArgs)
{
    ((ClimaFireStream*)pvArgs)->tratadorDeEventoRTDB(var);
}
void cc::ClimaFireStream::tratadorDeEventoRTDB(JsonVariant var)
{
    const char* str = var.as<const char*>();
    if (strlen(str))
    {
        //limpar a entrada no banco
        cc::ClimaFire::set(caminho + "/entrada", "");
        mtxEntrada.capturar();
        streamEntrada.print(str);
        mtxEntrada.liberar();
    }
}

int cc::ClimaFireStream::available()
{
    return streamEntrada.available();
}

int cc::ClimaFireStream::read()
{
    mtxEntrada.capturar();
    int temp = streamEntrada.read();
    mtxEntrada.liberar();
    return temp;
}

int cc::ClimaFireStream::peek()
{
    mtxEntrada.capturar();
    int temp = streamEntrada.peek();
    mtxEntrada.liberar();
    return temp;
}

size_t cc::ClimaFireStream::write(uint8_t byte)
{
    ultimaEscrita = millis();
    mtxSaida.capturar();
    size_t temp = streamSaida.write(byte);
    mtxSaida.liberar();
    return temp;
}

