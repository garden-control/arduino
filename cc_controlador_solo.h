#ifndef CC_CONTROLADOR_SOLO_H
#define CC_CONTROLDARO_SOLO_H
#include "cc_inicializacao.h"

namespace cc {
  class controlador_solo : inicializavel {
  public:
    static float f_umidade_max;
    static float f_umidade_min;
    static bool pausa;

  private:
    static controlador_solo unico;
    void iniciar() override;
    controlador_solo() = default;

    static void tarefa_controle(void* pv_args);
  };
};

#endif