#ifndef CC_WIFI_H
#define CC_WIFI_H
#include "cc_mutex.h"
#include <String.h>
#include "cc_terminal.h"
#include "cc_inicializacao.h"

namespace cc {
  class wifi : inicializavel {
  private:
    static constexpr char* caminho = "/wifi.txt";
    
    static wifi unico;
    
    static String ssid;
    static String senha;

    wifi() = default;
    void iniciar() override;
  
  public:
    static terminal::retorno comandos(const terminal::params& args);
    static bool conectado();
  };
};

#endif