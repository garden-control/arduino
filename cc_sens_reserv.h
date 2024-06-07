#ifndef CC_SENS_RESERV_H
#define CC_SENS_RESERV_H
#include "cc_Modulo.h"
#include "cc_mutex.h"

namespace cc {
  class sens_reserv : Modulo {
  private:
    static sens_reserv unico;
    sens_reserv();
    void aoIniciar() override;
  public:
    static int nivel();
  };
};

#endif