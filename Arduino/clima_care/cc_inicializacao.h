#ifndef CC_INICIALIZACAO_H
#define CC_INICIALIZACAO_H

namespace cc {
  void iniciar();
  class inicializavel {
    friend void iniciar();
  private:
    bool b_iniciado = false;
  protected:
    inicializavel(bool iniciar = true);
    
    virtual void iniciar() = 0;
    bool iniciado() const;
  };
};

#endif