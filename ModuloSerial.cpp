#include "cc_globais.h"

#ifdef USAR_SERIAL
#include "cc_Modulo.h"

class ModuloSerial : cc::Modulo
{
    ModuloSerial() :Modulo("Serial") {}
    void aoIniciar() override
    {
        Serial.begin(115200);
        Serial.setTimeout(0);
    }
    static ModuloSerial unico;
};
ModuloSerial ModuloSerial::unico;

#endif