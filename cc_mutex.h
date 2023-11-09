#ifndef CC_MUTEX_H
#define CC_MUTEX_H
#include <Arduino.h>

namespace cc {
  class mutex {
  private:
    SemaphoreHandle_t mtx = nullptr;
    bool ocupado = false;
  public:
    void capturar();
    void liberar();
    bool livre() const;
    class guarda {
    private:
      mutex& mtx;
    public:
      guarda(mutex& mtx);
      guarda(const guarda&) = delete;
      guarda& operator=(const guarda&) = delete;
      ~guarda();
    };
  };
  extern mutex mtx_spiffs;
};

#endif