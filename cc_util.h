#ifndef CC_UTIL_H
#define CC_UTIL_H
#include <Arduino.h>

namespace cc {
  
  class Json
  {
  private:
    String json;
  public:
    Json& add(const String& nome, const String& str);
    Json& add(const String& nome, const char* str);
    Json& add(const String& nome, int n);
    Json& add(const String& nome, float f);
    Json& add(const String& nome, bool b);
    Json& add(const String& nome, const Json& json);
    String toString() const;
  };

  String espera_linha(Stream& stream);

  bool consulta_banco(const String& sql, String* resposta = nullptr);
};

#endif