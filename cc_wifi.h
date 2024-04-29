#ifndef CC_WIFI_H
#define CC_WIFI_H
#include "cc_mutex.h"
#include <Arduino.h>
#include "cc_Modulo.h"

namespace cc {
  class wifi : Modulo {
  private:
    static constexpr char* caminho = "/wifi.txt";
    
    static wifi unico;
    
    static String ssid;
    static String senha;

    wifi();
    void aoIniciar() override;
  
  public:
    static bool conectado();
  };
};

#endif