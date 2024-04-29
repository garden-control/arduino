#ifndef CC_SENSORES_H
#define CC_SENSORES_H
#include "cc_Modulo.h"
#include <DHT.h>

namespace cc {
  class sens_dht : Modulo {
  private:
    static sens_dht unico;
    static DHT dht;

    sens_dht();

    void aoIniciar() override;
  public:
    static float umidade();
    static float temperatura();
    static float indice_calor();
    
  };
};

#endif