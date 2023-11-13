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

String cc::consulta_banco(const String& sql) {
  WiFiClient cliente;
  HTTPClient http;
  http.begin(cliente, "http://gardeningprojectteste.000webhostapp.com/clima_care.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int http_resposta = http.POST("query=" + sql);
  String resultado = http.getString();
  http.end();
  return String(http_resposta) + " " + resultado;
}