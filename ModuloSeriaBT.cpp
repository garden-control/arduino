#include "cc_globais.h"
#ifdef USAR_SERIALBT

#include "cc_Modulo.h"

class ModuloSerialBT : cc::Modulo
{
    ModuloSerialBT() :Modulo("SerialBT") {}
    void aoIniciar() override
    {
        cc::SerialBT.begin("ClimaCare");
    }
    static ModuloSerialBT unico;
};
ModuloSerialBT ModuloSerialBT::unico;

#endif