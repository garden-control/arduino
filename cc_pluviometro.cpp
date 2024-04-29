#include "cc_pluviometro.h"
#include "cc_pins.h"

HX711 cc::pluv::sens_carga;
cc::pluv cc::pluv::unico;

cc::pluv::pluv()
    :
    Modulo("pluv")
{

}

void cc::pluv::aoIniciar()
{
    sens_carga.begin(PIN::DT, PIN::SCK);
    sens_carga.tare();
    sens_carga.set_scale(-1099696.62f); //valor de calibração
}

float cc::pluv::indice_mm()
{
    float peso_kg = sens_carga.get_units(5);
    float peso_g = peso_kg * 1000.0f;
    float vol_cm3 = peso_g / 0.9970474f;
    float altura_cm = vol_cm3 / area_funil_cm2;
    float altura_mm = altura_cm * 10.0f;
    return altura_mm;
}
