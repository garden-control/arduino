#include "cc_Modulo.h"
#include "cc_globais.h"
#include <map>

std::map<std::string, cc::Modulo *> *pModulos = nullptr;
void cc::iniciarModulos()
{
    if (pModulos)
    {
        #ifdef DEBUG_SERIAL
        if (pModulos->count("Serial")) pModulos->at("Serial")->iniciar();
        #endif
        for (auto [id, pModulo] : *pModulos)
        {
            pModulo->iniciar();
        }
        delete pModulos;
    }
}
cc::Modulo::Modulo(const String &id, const std::list<String> &dependencias)
    :id(id), dependencias(dependencias)
{
    if (!pModulos)
        pModulos = new std::map<std::string, cc::Modulo *>;

    (*pModulos)[id.c_str()] = this;
}
void cc::Modulo::iniciar()
{
    if (!bIniciado)
    {
        #ifdef DEBUG_SERIAL
        if (id != "Serial")
        {
            Serial.printf("Iniciando %s...\n", id.c_str());
        }
        #endif

        for (const auto &d : dependencias)
        {
            if ((*pModulos).count(d.c_str()))
            {
                #ifdef DEBUG_SERIAL
                if (id != "Serial")
                {
                    Serial.printf("Iniciando dependencia %s de %s...\n", d.c_str(), id.c_str());
                }
                #endif

                (*pModulos)[d.c_str()]->iniciar();
            }
        }

        aoIniciar();
        bIniciado = true;

        #ifdef DEBUG_SERIAL
        if (id != "Serial")
        {
            Serial.printf("%s inicializado.\n", id.c_str());
        }
        #endif
    }
}