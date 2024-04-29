#include "cc_Modulo.h"
#include "cc_wifi.h"
#include "cc_util.h"
#include "cc_sens_dht.h"
#include "cc_sens_solo.h"
#include "cc_pluviometro.h"
#include <BluetoothSerial.h>
#include <list>

BluetoothSerial SerialBT;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    // SerialBT.begin("ClimaCare");
    delay(2000);

    cc::iniciarModulos();
}

struct leitura
{
    float temperatura, umidade, umidade_solo, indice_pluv;
};
std::list<leitura> leituras;
long long int tp1 = 0, tp2 = 0;

#include "cc_ClimaFire.h"
#include "cc_util.h"
void callback(AsyncResult& result);

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
    cc::ClimaFire::get("teste/stream", callback);
}

void callback(AsyncResult& result)
{
    if (result.available())
    {
        RealtimeDatabaseResult& rtdb = result.to<RealtimeDatabaseResult>();
        Firebase.printf("event: \"%s\"\n", rtdb.event().c_str());
        Firebase.printf("path: \"%s\"\n", rtdb.dataPath().c_str());
        Firebase.printf("data: \"%s\"\n", rtdb.to<const char *>());
        Firebase.printf("type: \"%d\"\n", rtdb.type());
    }
}
