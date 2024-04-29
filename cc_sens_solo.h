#ifndef CC_SENS_SOLO_H
#define CC_SENS_SOLO_H
#include "cc_Modulo.h"
#include "cc_mutex.h"

namespace cc {
  class sens_solo : Modulo {
  private:
    static constexpr float peso = 0.8f;   //peso do filtro
    static constexpr unsigned dt = 10;   //intervalo de tempo entre amostras (ms)

    static float f_umidade;                //valor filtrado
    static char ligado;
    static cc::mutex mtx_ligado;

    static sens_solo unico;
    sens_solo();
    void aoIniciar() override;
  public:
    static float umidade();
    static void liga(int espera_n_amostras = 0);
    static void desliga();

  private:
    static void tarefa_leitura(void* pv_args);
  };
};

#endif