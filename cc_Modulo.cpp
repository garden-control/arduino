#include "cc_Modulo.h"
#include "cc_globais.h"
#include "cc_util.h"
#include <map>

std::map<std::string, cc::Modulo *>* pModulos = nullptr;
void cc::iniciarModulos()
{
    if (pModulos)
    {
        #ifdef USAR_DEBUG_SERIAL
        
        if (pModulos->count("Serial")) pModulos->at("Serial")->iniciar();
        
        Serial.println("Modulos carregados:");
        for (auto& [id, pModulo] : (*pModulos))
            Serial.printf("  %s\n", id.c_str());

        #endif
        
        for (auto& [id, pModulo] : (*pModulos))
        {
            pModulo->iniciar();
        }
    }
}
cc::Modulo::Modulo(const String &id, const std::list<String> &dependencias)
    :id(id), dependencias(dependencias)
{
    if (!pModulos)
        pModulos = new std::map<std::string, Modulo*>;

    (*pModulos)[id.c_str()] = this;
}
void cc::Modulo::iniciar()
{
    if (!bIniciado)
    {
        if (id != "Serial")
        {
            DEBUG_SERIAL("Iniciando %s...\n", id.c_str());
        }

        for (const auto &d : dependencias)
        {
            if (pModulos->count(d.c_str()))
            {
                if (id != "Serial")
                {
                    DEBUG_SERIAL("Iniciando dependencia %s de %s...\n", d.c_str(), id.c_str());
                }

                (*pModulos)[d.c_str()]->iniciar();
            }
        }

        aoIniciar();
        bIniciado = true;

        if (id != "Serial")
        {
            DEBUG_SERIAL("%s inicializado.\n", id.c_str());
        }
    }
}
void cc::Modulo::transmitirEvento(const String& evento)
{
    for (auto [idModulo, pModulo] : (*pModulos))
    {
        pModulo->aoReceberEvento(id, evento);
    }
}