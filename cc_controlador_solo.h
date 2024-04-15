#ifndef CC_CONTROLADOR_SOLO_H
#define CC_CONTROLDARO_SOLO_H
#include "cc_inicializacao.h"
#include <StreamString.h>

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

  private:
    static String clinoPausar(StreamString& entrada, void* pv_args);
    static String clinoStatus(StreamString& entrada, void* pv_args);
    static String clinoSetMin(StreamString& entrada, void* pv_args);
    static String clinoSetMax(StreamString& entrada, void* pv_args);
  };
};

#endif