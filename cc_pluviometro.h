#ifndef CC_PLUVIOMETRO_H
#define CC_PLUVIOMETRO_H
#include "cc_Modulo.h"
#include <HX711.h>

namespace cc {
    class pluv : Modulo {
    private:
        static constexpr float area_funil_cm2 = 100.0f;
        static HX711 sens_carga;
        static pluv unico;

        pluv();
        
        void aoIniciar() override;
    public:
        static float indice_mm();
    };
};

#endif