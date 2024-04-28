#ifndef CC_WIFI_H
#define CC_WIFI_H
#include "cc_mutex.h"
#include <Arduino.h>
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
    static bool conectado();
  };
};

#endif