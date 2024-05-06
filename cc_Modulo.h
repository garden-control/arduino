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
    
    protected:
        const String id;
    private:
        std::list<String> dependencias;
        bool bIniciado = false;

    protected:
        Modulo(const String& id, const std::list<String>& dependencias = {});

    public:
        void iniciar();
    
    protected:
        virtual void aoIniciar() {};

        void transmitirEvento(const String& evento);
        virtual void aoReceberEvento(const String& idModulo, const String& evento) {}
    };
};

#endif