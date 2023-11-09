#include "cc_util.h"
#include <WiFi.h>
#include <HTTPClient.h>

String cc::espera_linha(Stream& stream) {
  String linha;
  while (!stream.available()) delay(10);
  while (stream.available() && stream.peek() != '\n' && stream.peek() != '\r') {
    linha += (char)stream.read();
    delay(10);
  }
  while (stream.available()) {
    stream.read();
    delay(10);
  }
  return linha;
}

int cc::consulta_banco(const String& expressao_sql, String* resposta) {
  WiFiClient cliente;
  HTTPClient http;
  http.begin(cliente, "http://192.168.1.8:80/clima_care/clima_care.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int http_codigo_resposta = http.POST("query=" + expressao_sql);
  if (resposta) {
    *resposta = http.getString();
  }
  return http_codigo_resposta;
}