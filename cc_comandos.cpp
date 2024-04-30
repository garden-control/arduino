#include "cc_clino.h"
#include "cc_Modulo.h"

#include "cc_controlador_solo.h"
#include "cc_sens_reserv.h"
#include "cc_sens_solo.h"

namespace cc
{
    class Comandos : Modulo
    {
        Comandos() :Modulo("Comandos") {}
        static Comandos unico;
        void aoIniciar() override
        {
            Clino::comandos.insert({
                {"ctrlPausar", Clino::Comando(ctrlPausar, "Pausa/inicia o controlador")},
                {"ctrlStatus", Clino::Comando(ctrlStatus, "Checa o stauts do controlador")},
                {"ctrlSetMin", Clino::Comando(ctrlSetMin, "Configura a umidade minima permitida")},
                {"ctrlSetMax", Clino::Comando(ctrlSetMax, "Configura a umidade maxima permitida")}
            });
        }

        // COMANDOS
        static String ctrlPausar(StreamString& entrada)
        {
            cc::controlador_solo::pausa = !cc::controlador_solo::pausa;
            
            return String() + "Controle " + (cc::controlador_solo::pausa ? "pausado" : "retomado");
        }
        static String ctrlStatus(StreamString& entrada)
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
        static String ctrlSetMin(StreamString& entrada)
        {
            return "";
        }
        static String ctrlSetMax(StreamString& entrada)
        {
            return "";
        }
    }
    Comandos::unico;
}