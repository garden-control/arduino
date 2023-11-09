#include "cc_inicializacao.h"
#include <list>
#include <SPIFFS.h>

std::list<cc::inicializavel*>* p_inicializaveis = nullptr;

void cc::iniciar() {
  if (!SPIFFS.begin(true)) {
    Serial.print("[Inicializador] Montagem do sistema de arquivos falhou. Memória formatada.\n\r");
  }
  if (p_inicializaveis) {
    for (auto& i : *p_inicializaveis) {
      i->iniciar();
      i->b_iniciado = true;
    }
    delete p_inicializaveis;
    p_inicializaveis = nullptr;
  }
}

cc::inicializavel::inicializavel(bool iniciar) {
  if (iniciar) {
    if (!p_inicializaveis) {
      p_inicializaveis = new std::list<inicializavel*>;
    }
    p_inicializaveis->push_back(this);
  }
}
bool cc::inicializavel::iniciado() const {
  return b_iniciado;
}