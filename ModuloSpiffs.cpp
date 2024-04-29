#include "cc_Modulo.h"
#include <SPIFFS.h>
#include "cc_globais.h"

using namespace cc;

class ModuloSpiffs : Modulo
{
    ModuloSpiffs() :Modulo("Spiffs"){}
    void aoIniciar() override
    {
        if (!SPIFFS.begin(true) && pDebugStream)
            pDebugStream->printf("[SPIFFS] Montagem do sistema de arquivos falhou. Memória formatada.\n");
    }
};