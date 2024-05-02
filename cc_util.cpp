#include "cc_util.h"

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

String cc::pegarString(Stream& stream)
{
    String str;

    while (stream.available() && stream.peek() == ' ') stream.read();
    
    if (stream.peek() == '"')
    {
        stream.read();
        str = stream.readStringUntil('"');
        stream.read();
    }
    else
    {
        str = stream.readStringUntil(' ');
    }

    return str;
}
bool cc::analisarLinhaDeComando(const std::map<std::string, char>& params, Stream& stream, std::map<std::string, void*>& args, String& erros)
{
    while (stream.available())
    {
        String param = pegarString(stream);
        if (param.length() > 0)
        {
            if (param[0] == '-')
            {
                if (params.count(param.c_str()))
                {
                    switch (params.at(param.c_str()))
                    {
                    case 's':
                        args[param.c_str()] = new String(pegarString(stream));
                        break;
                    case 'f':
                        args[param.c_str()] = new float(stream.parseFloat());
                        break;
                    case 'i':
                        args[param.c_str()] = new int(stream.parseInt());
                        break;
                    case 'b':
                        args[param.c_str()] = new bool(true);
                        break;
                    }
                }
                else
                {
                    erros += "Parametro \"" + param + "\" nao existe\n";
                }
            }
            else
            {
                erros += "Nomes de parametros devem comecar com traco (-)\n";
            }
        }
    }

    return erros.isEmpty();
}