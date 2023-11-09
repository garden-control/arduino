#ifndef CC_SENS_SOLO_H
#define CC_SENS_SOLO_H
#include "cc_inicializacao.h"
#include "cc_terminal.h"

namespace cc {
  class sens_solo : inicializavel {
  private:
    static constexpr float peso = 0.8f;   //peso do filtro
    static constexpr unsigned dt = 10;   //intervalo de tempo entre amostras (ms)

    static float f_umidade;                //valor filtrado
    static char ligado;

    static sens_solo unico;
    sens_solo() = default;
    void iniciar() override;
  public:
    static float umidade();
    static void liga(int espera_n_amostras = 0);
    static void desliga();

    static terminal::retorno comandos(const cc::terminal::params& args);
  private:
    static void tarefa_leitura(void* pv_args);
  };
};

#endif