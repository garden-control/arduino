#include "cc_wifi.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include "cc_util.h"

cc::wifi cc::wifi::unico;
String cc::wifi::ssid;
String cc::wifi::senha;

void cc::wifi::iniciar() {
  mtx_spiffs.capturar();
  File arq = SPIFFS.open(caminho);
  if (arq) {
    ssid = arq.readStringUntil('\n');
    senha = arq.readStringUntil('\n');
    WiFi.begin(ssid, senha);
  }
  arq.close();
  mtx_spiffs.liberar();
}
cc::terminal::retorno cc::wifi::comandos(const terminal::params& args) {
  static const terminal::map_str_cmd cmds = terminal::map_aux()
  .cmd(
    "def",
    "Define a rede wifi",
    [](const terminal::params& args) -> terminal::retorno {
      args.stream.print("SSID:  ");
      ssid = espera_linha(args.stream);
      args.stream.printf("%s\n", ssid.c_str());
      
      args.stream.print("Senha: ");
      senha = espera_linha(args.stream);
      args.stream.printf("%s\n", senha.c_str());

      WiFi.begin(ssid, senha);

      mtx_spiffs.capturar();
      File arq = SPIFFS.open(caminho, FILE_WRITE, true);
      if (arq) {
        arq.printf("%s\n%s\n", ssid.c_str(), senha.c_str());
        args.stream.print("Rede salva\n");
      }
      else {
        args.stream.print("Falha ao salvar a rede\n");
      }
      arq.close();

      mtx_spiffs.liberar();
      return {};
    }
  )
  .cmd(
    "status",
    "Exibe status da rede",
    [](const terminal::params& args) -> terminal::retorno {
      args.stream.printf(
        "Nome da rede: %s\n"
        "Status:       %s\n",
        ssid.c_str(),
        conectado() ? "conectado" : "desconectado"
      );
      return {};
    }
  );
  
  
  return terminal::executar(args.rotulo, args.entrada, args.stream, cmds);
}
bool cc::wifi::conectado() {
  return WiFi.status() == WL_CONNECTED;
}