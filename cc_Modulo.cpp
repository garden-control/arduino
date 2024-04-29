#include "cc_Modulo.h"
#include <map>

std::map<std::string, cc::Modulo *> *pModulos = nullptr;
void cc::iniciarModulos()
{
    if (pModulos)
    {
        for (auto [id, pModulo] : *pModulos)
        {
            pModulo->iniciar();
        }
        delete pModulos;
    }
}
cc::Modulo::Modulo(const String &id, const std::list<String> &dependencias)
    : dependencias(dependencias)
{
    if (!pModulos)
        pModulos = new std::map<std::string, cc::Modulo *>;

    (*pModulos)[id.c_str()] = this;
}
void cc::Modulo::iniciar()
{
    if (!bIniciado)
    {
        for (const auto &d : dependencias)
        {
            (*pModulos)[d.c_str()]->iniciar();
        }
        aoIniciar();
    }
}