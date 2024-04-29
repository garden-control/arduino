#ifndef CC_MODULO_H
#define CC_MODULO_H
#include <Arduino.h>
#include <list>

namespace cc
{
    void iniciarModulos();
    class Modulo
    {
        friend void iniciarModulos();
    
    private:
        std::list<String> dependencias;
        bool bIniciado = false;

    protected:
        Modulo(const String& id, const std::list<String>& dependencias = {});

    private:
        void iniciar();
    
    protected:
        virtual void aoIniciar() = 0;

        void transmitirEvento(const String& evento);
        virtual void aoReceberEvento(const String& idModulo, const String& evento) {}
    };
};

#endif