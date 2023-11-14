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
  "ctrl",
  "Acesso as configuracoes de controle",
  [](const cc::terminal::params& args) -> cc::terminal::retorno {
    static cc::terminal::map_str_cmd sub = cc::terminal::map_aux(
    ).cmd(
      "liga",
      "Liga/desliga controlador",
      [](const cc::terminal::params& args) -> cc::terminal::retorno {
        cc::controlador_solo::pausa = !cc::controlador_solo::pausa;
        args.stream.println(!cc::controlador_solo::pausa ? "Controle ligado" : "Controle desligado");
        return {};
      }
    ).cmd(
      "status",
      "Checa status de controle",
        [](const cc::terminal::params& args) -> cc::terminal::retorno {
          args.stream.println(!cc::controlador_solo::pausa ? "Controle ligado" : "Controle desligado");
          
          cc::sens_reserv::liga(10);
          args.stream.println(cc::sens_reserv::vazio() ? "Reservatorio vazio" : "Reservatorio ok");
          cc::sens_reserv::desliga();
          
          args.stream.print("Umidade do solo:\n  Atual: ");
          cc::sens_solo::liga(10);
          args.stream.println(cc::sens_solo::umidade());
          cc::sens_solo::desliga();

          args.stream.print("  Minima: ");
          args.stream.println(cc::controlador_solo::f_umidade_min);
          
          args.stream.print("  Maxima: ");
          args.stream.println(cc::controlador_solo::f_umidade_max);
          return {};
        }
    ).cmd(
      "min",
      "Configura a umidade minima",
      [](const cc::terminal::params& args) -> cc::terminal::retorno {
        args.stream.println("Digite um valor entre 0 e 1...");
        float min = args.stream.parseFloat();
        if (min >= 0.0f && min < 1.0f) {
          cc::controlador_solo::f_umidade_min = min;
          args.stream.print("Umidade minima configurada para ");
          args.stream.println(min);
        }
        else {
          args.stream.println("Entrada invalida");
        }
        return {};
      }
    ).cmd(
      "max",
      "Configura a umidade maxima",
      [](const cc::terminal::params& args) -> cc::terminal::retorno {
        args.stream.println("Digite um valor entre 0 e 1...");
        float max = args.stream.parseFloat();
        if (max >= 0.0f && max < 1.0f) {
          cc::controlador_solo::f_umidade_max = max;
          args.stream.print("Umidade maxima configurada para ");
          args.stream.println(max);
        }
        else {
          args.stream.println("Entrada invalida");
        }
        return {};
      }
    );
    return cc::terminal::executar(args.rotulo, args.entrada, args.stream, sub);
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
      "Acesso as opcoes do sensor de solo",
      [](const cc::terminal::params& args) -> cc::terminal::retorno {
        static const terminal::map_str_cmd cmds = terminal::map_aux(
        ).cmd(
          "plot",
          "Exibe as leituras do sensor",
          [](const terminal::params& args) -> terminal::retorno {
            args.stream.println("Digite qualquer coisa para sair...");
            cc::sens_solo::liga();
            while (!args.stream.available()) {
              args.stream.println(cc::sens_solo::umidade());
              delay(100);
            }
            cc::sens_solo::desliga();
            while (args.stream.available()) {
              args.stream.read();
              delay(10);
            }
            return {};
          }
        );
        return terminal::executar(args.rotulo, args.entrada, args.stream, cmds);
      }
    ).cmd(
      "reserv",
      "Acesso as configuracoes do sensor do reservatorio",
      [](const cc::terminal::params& args) -> cc::terminal::retorno {
        static const terminal::map_str_cmd cmds = terminal::map_aux(
        ).cmd(
          "status",
          "Informa status do reservatorio",
          [](const cc::terminal::params& args) -> cc::terminal::retorno {
            args.stream.print("Gatilho: ");
            args.stream.println(cc::sens_reserv::gatilho);
            cc::sens_reserv::liga(10);
            args.stream.print("Valor: ");
            args.stream.println(cc::sens_reserv::valor());
            args.stream.print(cc::sens_reserv::vazio() ? "Vazio\n" : "Cheio\n");
            cc::sens_reserv::desliga();
            return {};
          }
        ).cmd(
          "plot",
          "Plota leitura do sensor do reservatorio",
          cc::sens_reserv::plot
        ).cmd(
          "def",
          "Configura gatilho para deteccao de agua",
          [](const cc::terminal::params& args) -> cc::terminal::retorno {
            args.stream.println("Digite um valor entre 0 (cheio) e 1 (vazio)...");
            float gatilho = args.stream.parseFloat();
            if (gatilho > 0.0f && gatilho < 1.0f) {
              cc::sens_reserv::gatilho = gatilho;
              args.stream.print("Gatilho configurado para ");
              args.stream.println(gatilho);
            }
            else {
              args.stream.println("Entrada invalida");
            }
            return {};
          }
        );
        return terminal::executar(args.rotulo, args.entrada, args.stream, cmds);
      }
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
