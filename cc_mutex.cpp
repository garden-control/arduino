#include "cc_mutex.h"

void cc::mutex::capturar() {
  if (!mtx) {
    mtx = xSemaphoreCreateMutex();
  }
  xSemaphoreTake(mtx, portMAX_DELAY);
  ocupado = true;
}
void cc::mutex::liberar() {
  if (mtx)
    xSemaphoreGive(mtx);
  ocupado = false;
}
bool cc::mutex::livre() const {
  return !ocupado;
}

cc::mutex::guarda::guarda(cc::mutex& mtx) 
  :mtx(mtx) {
  mtx.capturar();
}
cc::mutex::guarda::~guarda() {
  mtx.liberar();
}