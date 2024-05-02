#include "cc_Modulo.h"
#include <SPIFFS.h>
#include "cc_globais.h"
#include "cc_util.h"

using namespace cc;

class ModuloSpiffs : Modulo
{
    ModuloSpiffs() :Modulo("Spiffs"){}
    void aoIniciar() override
    {
        if (!SPIFFS.begin(true))
        {
            DEBUG_SERIAL("[SPIFFS] Montagem do sistema de arquivos falhou. Memória formatada.\n");
        }
    }

    static ModuloSpiffs unico;
} ModuloSpiffs::unico;