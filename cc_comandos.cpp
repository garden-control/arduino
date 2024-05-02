#include "cc_clino.h"
#include "cc_Modulo.h"

#include <map>
#include "cc_globais.h"
#include "cc_util.h"

#include "cc_controlador_solo.h"
#include "cc_sens_reserv.h"
#include "cc_sens_solo.h"
#include "cc_wifi.h"

namespace cc
{
    class Comandos : Modulo
    {
        Comandos() : Modulo("Comandos") {}
        static Comandos unico;
        void aoIniciar() override
        {
            Clino::comandos.insert({
                {"ctrlPausar", Clino::Comando(ctrlPausar, "Pausa/inicia o controlador")},
                {"ctrlStatus", Clino::Comando(ctrlStatus, "Checa o stauts do controlador")},
                {"ctrlSetMin", Clino::Comando(ctrlSetMin, "Configura a umidade minima permitida")},
                {"ctrlSetMax", Clino::Comando(ctrlSetMax, "Configura a umidade maxima permitida")},

                {"wifiConfig", Clino::Comando(wifiConfig, "Configura SSID e senha da rede wifi")}
            });
        }

        // COMANDOS
        static String ctrlPausar(StreamString &entrada)
        {
            cc::controlador_solo::pausa = !cc::controlador_solo::pausa;

            return String() + "Controle " + (cc::controlador_solo::pausa ? "pausado" : "retomado");
        }
        static String ctrlStatus(StreamString &entrada)
        {
            cc::sens_reserv::liga(10);
            bool reserv = !cc::sens_reserv::vazio();
            cc::sens_reserv::desliga();

            cc::sens_solo::liga(10);
            float umidade = cc::sens_solo::umidade();
            cc::sens_solo::desliga();

            String str;
            str += String() + "Controlador:  " + (cc::controlador_solo::pausa ? "pausado" : "ativo") + "\n";
            str += String() + "Reservatorio: " + (reserv ? "ok" : "vazio") + "\n";
            str += String() + "Umidade:      " + umidade + "\n";
            str += String() + "Minima:       " + controlador_solo::f_umidade_min + "\n";
            str += String() + "Maxima:       " + controlador_solo::f_umidade_max + "\n\n";

            return str;
        }
        static String ctrlSetMin(StreamString &entrada)
        {
            return "";
        }
        static String ctrlSetMax(StreamString &entrada)
        {
            return "";
        }
        static String wifiConfig(StreamString &entrada)
        {
            std::map<std::string, void*> args;
            String erros;

            if (analisarLinhaDeComando({
                {"--ssid", 's'},
                {"--senha", 's'}
            }, entrada, args, erros))
            {
                if (args.count("--ssid") && args.count("--senha"))
                {
                    const String& ssid = *((String*)args["--ssid"]);
                    const String& senha = *((String*)args["--senha"]);
                    
                    DEBUG_SERIAL("ssid: \"%s\"\nsenha:\"%s\"\n", ssid.c_str(), senha.c_str());

                    wifi::configurar(ssid, senha);
                }
            }

            for (auto& arg : args)
                delete arg.second;

            return erros;
        }
    } Comandos::unico;
}