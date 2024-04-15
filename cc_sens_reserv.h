#ifndef CC_SENS_RESERV_H
#define CC_SENS_RESERV_H
#include "cc_inicializacao.h"
#include "cc_mutex.h"

namespace cc {
  class sens_reserv : inicializavel {
  private:
    static constexpr float    peso    = 0.8f;   //peso do filtro
    static constexpr unsigned dt      = 100 ;   //intervalo de tempo entre amostras (ms)
  public:
    static float gatilho;
  private:
    static float f_valor;
    static char ligado;
    static cc::mutex mtx_ligado;

    static sens_reserv unico;
    sens_reserv() = default;
    void iniciar() override;
  public:
    static bool vazio();
    static float valor();
    static void liga(int espera_n_amostras = 0);
    static void desliga();
  private:
    static void tarefa_leitura(void* pv_args);
  };
};

#endif