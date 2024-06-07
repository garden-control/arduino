#ifndef CC_CONTROLADOR_SOLO_H
#define CC_CONTROLDARO_SOLO_H
#include "cc_Modulo.h"
#include <ArduinoJson.h>

namespace cc {
  class controlador_solo : Modulo {
  public:
    static float f_umidade_max;
    static float f_umidade_min;
    static bool pausa;
    static bool bomba;

  private:
    static controlador_solo unico;
    void aoIniciar() override;
    void aoReceberEvento(const String& idModulo, const String& evento) override;
    static void aoEventoRTDB(JsonVariant var, void* pvArgs);
    void aoEventoRTDB(JsonVariant var);

    controlador_solo();

    static void tarefa_controle(void* pv_args);
  };
};

#endif