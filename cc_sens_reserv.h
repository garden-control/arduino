#ifndef CC_SENS_RESERV_H
#define CC_SENS_RESERV_H
#include "cc_Modulo.h"
#include "cc_ClimaFire.h"


namespace cc
{
    class sens_reserv : Modulo
    {
    private:
        static sens_reserv unico;
        int nivelAgua = 0;

        sens_reserv();
        void aoIniciar() override;
        static void tarefa(void*);
        static void aoEventoRTDB(JsonVariant var, void*);

    public:
        static int nivel();
    };
};

#endif