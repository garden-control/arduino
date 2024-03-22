#ifndef CC_SENSORES_H
#define CC_SENSORES_H
#include "cc_inicializacao.h"
#include <DHT.h>

namespace cc {
  class sens_dht : inicializavel {
  private:
    static sens_dht unico;
    static DHT dht;

    void iniciar() override;
  public:
    static float umidade();
    static float temperatura();
    static float indice_calor();
    
  };
};

#endif