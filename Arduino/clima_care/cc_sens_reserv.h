#ifndef CC_SENS_RESERV_H
#define CC_SENS_RESERV_H
#include "cc_inicializacao.h"
#include "cc_terminal.h"

namespace cc {
  class sens_reserv : inicializavel {
  private:
    static constexpr float    peso    = 0.8f;   //peso do filtro
    static constexpr unsigned dt      = 100 ;   //intervalo de tempo entre amostras (ms)
    static constexpr float    gatilho = 0.45f;   //acima desse valor, o reservatorio esta vazio 

    static float f_valor;
    static char ligado;

    static sens_reserv unico;
    sens_reserv() = default;
    void iniciar() override;
  public:
    static bool vazio();
    static void liga(int espera_n_amostras = 0);
    static void desliga();
    static terminal::retorno plot(const terminal::params& args);
  private:
    static void tarefa_leitura(void* pv_args);
  };
};

#endif