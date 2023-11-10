#ifndef CC_TERMINAL_H
#define CC_TERMINAL_H
#include <Stream.h>
#include <StreamString.h>
#include <String.h>
#include <unordered_map>
#include <string> //para usar como chave em unordered_map
#include <functional>
#include "cc_inicializacao.h"

namespace cc {
  class terminal : inicializavel {
  public:

    class retorno {
      friend terminal;
    public:
      enum valor {
        INICIO, VOLTAR, MANTER
      };
    private:
      const valor v = MANTER;
      retorno(valor v);
    public:
      retorno() = default;
      operator valor() const;
    };

    struct params {
      StreamString& entrada;
      Stream& stream;
      const String& rotulo;
    };

    struct comando {
      String descricao;
      std::function<retorno(const params&)> func;
      String manual;
    };

    typedef std::unordered_map<std::string, comando> map_str_cmd;

    class map_aux {
    public:
      map_str_cmd m;
      operator map_str_cmd&();
      map_aux& cmd(const std::string& identificador, const String& descricao, const std::function<retorno(const params&)>& func, const String& manual = "");
    };

  private:
    static const map_str_cmd raiz;
    
    Stream& stream;

  public:    
    //lança novo processo para executar a raiz do terminal quando inicializado
    terminal(Stream& stream);
    terminal(const terminal&) = delete;

    //executa uma sub-árvore
    static retorno executar(
      const String& rotulo, 
      StreamString& entrada, 
      Stream& stream, 
      const map_str_cmd& comandos
    );
    
  private:
    void iniciar() override;
    static void executar_raiz(void* pv_terminal);
  };
};

#endif