#include "cc_Modulo.h"
#include "cc_wifi.h"
#include "cc_sens_dht.h"
#include "cc_sens_solo.h"
#include "cc_pluviometro.h"
#include "cc_clino.h"
#include "cc_globais.h"
#include "cc_util.h"
#include "cc_ClimaFire.h"
#include "cc_ClimaFireStream.h"
#include "cc_util.h"

#include <list>

//cc::Clino cliSerial(Serial, "cliSerial");

cc::ClimaFireStream cliStream("cli");

void setup()
{
    cc::iniciarModulos();
}

struct leitura
{
    float temperatura, umidade, umidade_solo, indice_pluv;
};
std::list<leitura> leituras;
long long int tp1 = 0, tp2 = 0;


void loop()
{
    /*
    cc::sens_solo::liga(10);
    leituras.push_back({
        cc::sens_dht::temperatura(),
        cc::sens_dht::umidade(),
        cc::sens_solo::umidade(),
        cc::pluv::indice_mm() // fazer...
    });
    cc::sens_solo::desliga();

    if (cc::wifi::conectado())
    {
        char sql[512];
        for (auto it = leituras.begin(); it != leituras.end();)
        {
            const auto &l = *it;
            sprintf(
                sql,
                "insert into SensorData (localizacao, temperatura, umidade, SensorSolo, alturaReservPluv) values (\"Sala\", \"%.2f\", \"%.2f\", \"%.2f\", \"%.2f\")",
                l.temperatura,
                l.umidade,
                l.umidade_solo * 100.0f,
                l.indice_pluv);
            // enviar dados
        }
    }
    tp2 = millis();
    delay(max(0LL, 5 * 1000 - (tp2 - tp1)));
    tp1 = millis();
    */

   
   if (cliStream.available())
   {
        while (cliStream.available())
            Serial.print((char)cliStream.read());    
        Serial.println();
   }

    while (Serial.available())
        cliStream.print((char)Serial.read());
  
    
    delay(1000);
}
