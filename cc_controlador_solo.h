#ifndef CC_CONTROLADOR_SOLO_H
#define CC_CONTROLDARO_SOLO_H
#include "cc_Modulo.h"

namespace cc {
  class controlador_solo : Modulo {
  public:
    static float f_umidade_max;
    static float f_umidade_min;
    static bool pausa;

  private:
    static controlador_solo unico;
    void aoIniciar() override;
    controlador_solo();

    static void tarefa_controle(void* pv_args);
  };
};

#endif