#include "cc_terminal.h"
#include <Arduino.h>
#include "cc_wifi.h"
#include "cc_mutex.h"
#include <SPIFFS.h>
#include "cc_util.h"
#include "cc_sens_solo.h"
#include "cc_sens_reserv.h"
#include "cc_controlador_solo.h"


const cc::terminal::map_str_cmd cc::terminal::raiz = cc::terminal::map_aux(
).cmd(
  "sql",
  "Teste de sql",
  [](const cc::terminal::params& args) -> cc::terminal::retorno {
    String resposta;
    int http_codigo = cc::consulta_banco(
      "insert into SensorData (localizacao, umidade, temperatura, indice_calor, SensorSolo) values"
      "(\"sala\", \"37%\", \"36C\", \"sla\", \"umido\")",
      &resposta
    );
    args.stream.println(String("retorno: ") + http_codigo);
    args.stream.println("resposta: " + resposta);
    return {};
  }
).cmd(
  "ctrl",
  "Inicia/pausa controle",
  [](const cc::terminal::params& args) -> cc::terminal::retorno {
    cc::controlador_solo::pausa = !cc::controlador_solo::pausa;
    Serial.println(!cc::controlador_solo::pausa ? "Controle ligado" : "Controle desligado");
    return {};
  }
).cmd(
  "ctrl_status",
  "Checa status de controle",
    [](const cc::terminal::params& args) -> cc::terminal::retorno {
      Serial.println(!cc::controlador_solo::pausa ? "Controle ligado" : "Controle desligado");
      return {};
    }
).cmd(
  "wifi",
  "Acesso as opcoes de wifi",
  cc::wifi::comandos
).cmd(
  "sens",
  "Acesso aos sensores",
  [](const cc::terminal::params& args) -> cc::terminal::retorno {
    static const cc::terminal::map_str_cmd cmds = cc::terminal::map_aux(
    ).cmd(
      "solo",
      "Acesso as configuracoes do sensor de solo",
      cc::sens_solo::comandos
    ).cmd(
      "resev",
      "Exibe status do reservatorio",
      [](const cc::terminal::params& args) -> cc::terminal::retorno {
        cc::sens_reserv::liga(10);
        args.stream.print(cc::sens_reserv::vazio() ? "Vazio\n" : "Cheio\n");
        cc::sens_reserv::desliga();
        return {};
      }
    ).cmd(
      "reserv_plot",
      "Plota leitura do sensor do reservatorio",
      cc::sens_reserv::plot
    );
    return cc::terminal::executar(args.rotulo, args.entrada, args.stream, cmds);
  }
).cmd(
  "form",
  "Formata a memoria",
  [](const cc::terminal::params&) -> cc::terminal::retorno {
    mtx_spiffs.capturar();
    SPIFFS.format();
    mtx_spiffs.liberar();
    return {};
  }
);

cc::terminal::retorno::retorno(valor v) :v(v) {}
cc::terminal::retorno::operator cc::terminal::retorno::valor() const {
  return v;
}

cc::terminal::map_aux::operator cc::terminal::map_str_cmd&() {
  return m;
}
cc::terminal::map_aux& cc::terminal::map_aux::cmd(const std::string& identificador, const String& descricao, const std::function<retorno(const params&)>& func) {
  m[identificador].descricao = descricao;
  m[identificador].func = func;
  return *this;
}

cc::terminal::terminal(Stream& stream)
  :
  stream(stream) {
}
cc::terminal::retorno cc::terminal::executar(
  const String& rotulo, 
  StreamString& entrada, 
  Stream& stream, 
  const map_str_cmd& comandos
) {
  while (true) {
    //ler nova entrada
    if (entrada.length() == 0) {
      stream.print(rotulo + "> ");
      while (entrada.length() == 0) {
        entrada.print(espera_linha(stream));
        entrada.trim();
      }
      stream.printf("%s\n", entrada.c_str());
    }
    
    String identificador = entrada.readString();
    
    auto it = comandos.find(identificador.c_str());

    if (it != comandos.end()) {
      auto r = it->second.func({ entrada, stream, rotulo + '>' + it->first.c_str() });
      if (r.v == r.INICIO) {
        return r;
      }
      else {
        entrada.clear();
      }
    }
    else if (identificador == "ajuda") {
      //imprimir identificador e descrição do comando
      for (const auto& cmd : comandos)
        stream.printf("%-13s%s\n", cmd.first.c_str(), cmd.second.descricao.c_str());
      stream.print(
        "ajuda        Lista todos os comandos disponiveis\n"
        "voltar       Volta um sub-menu\n"
        "inicio       Volta ao menu principal\n"
      );
    }
    else if (identificador == "voltar") 
      return retorno(retorno::MANTER);
    else if (identificador == "inicio")
      return retorno(retorno::INICIO);
    else 
      stream.printf("Comando \"%s\" desconhecido.\n\n", identificador.c_str());
    delay(10);
  }
  return {};
}
void cc::terminal::iniciar() {
  static unsigned int id = 0;
  xTaskCreate(executar_raiz, (String("terminal") + id++).c_str(), 8192, (void*)this, 1, nullptr);
}
void cc::terminal::executar_raiz(void* pv_terminal) {
  StreamString entrada;
  entrada.setTimeout(0);
  ((terminal*)pv_terminal)->stream.setTimeout(0);
 
  while (true) {
    executar("", entrada, ((terminal*)pv_terminal)->stream, raiz);
    entrada.clear();
    delay(10);
  }
}
