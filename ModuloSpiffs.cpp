#include "cc_Modulo.h"
#include <SPIFFS.h>
#include "cc_globais.h"

using namespace cc;

class ModuloSpiffs : Modulo
{
    ModuloSpiffs() :Modulo("Spiffs"){}
    void aoIniciar() override
    {
        #ifdef DEBUG_SERIAL
        Serial.printf("[SPIFFS] Montagem do sistema de arquivos falhou. Memória formatada.\n");
        #endif
    }
};