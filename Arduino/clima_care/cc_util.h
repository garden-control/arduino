#ifndef CC_UTIL_H
#define CC_UTIL_H
#include <Arduino.h>

namespace cc {
  String espera_linha(Stream& stream);
  int consulta_banco(const String& expressao_sql, String* resposta = nullptr);
};

#endif