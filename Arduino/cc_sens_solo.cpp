#include "cc_sens_solo.h"
#include "cc_pins.h"
#include <Arduino.h>

float cc::sens_solo::f_umidade = 0.0f;
char cc::sens_solo::ligado = 0;

cc::sens_solo cc::sens_solo::unico;
void cc::sens_solo::iniciar() {
  pinMode(PIN::SENS_SOLO, INPUT);
  pinMode(PIN::LIGA_SENS_SOLO, OUTPUT);
  xTaskCreate(tarefa_leitura, "leitura_sens_solo", 2048, nullptr, 1, nullptr);
}
float cc::sens_solo::umidade() {
  return f_umidade;
}
void cc::sens_solo::liga(int espera_n_amostras) {
  ligado++;
  if (espera_n_amostras)
    delay(espera_n_amostras * dt);
}
void cc::sens_solo::desliga() {
  ligado -= (ligado > 0);
}
cc::terminal::retorno cc::sens_solo::comandos(const cc::terminal::params& args) {
  static const terminal::map_str_cmd cmds = terminal::map_aux(
  ).cmd(
    "plot",
    "Exibe as leituras do sensor",
    [](const terminal::params& args) -> terminal::retorno {
      args.stream.println("Digite qualquer coisa para sair...");
      liga();
      while (!args.stream.available()) {
        args.stream.println(umidade());
        delay(100);
      }
      desliga();
      while (args.stream.available()) {
        args.stream.read();
        delay(10);
      }
      return {};
    }
  );
  return terminal::executar(args.rotulo, args.entrada, args.stream, cmds);
}
void cc::sens_solo::tarefa_leitura(void* pv_args) {
  while (1) {
    while (!ligado) delay(10);
    digitalWrite(PIN::LIGA_SENS_SOLO, HIGH);
    delay(1);
    f_umidade = (float)analogRead(PIN::SENS_SOLO) / 4095.0f;
    while (ligado) {
      delay(dt);
      f_umidade += ((float)analogRead(PIN::SENS_SOLO) / 4095.0f - f_umidade) * peso;
    }
    digitalWrite(PIN::LIGA_SENS_SOLO, LOW);
  }
}