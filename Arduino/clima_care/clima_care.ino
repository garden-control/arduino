#include "cc_inicializacao.h"
#include "cc_terminal.h"
#include "cc_wifi.h"
#include "cc_util.h"
#include "cc_sens_dht.h"
#include "cc_sens_solo.h"
#include <list>

cc::terminal term_serial(Serial);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);

  cc::iniciar();
}

struct leitura {
  float temperatura, umidade, umidade_solo, indice_pluv;
};
std::list<leitura> leituras;
void loop() {
  // put your main code here, to run repeatedly:
  cc::sens_solo::liga(10);
  leituras.push_back({
    cc::sens_dht::temperatura(),
    cc::sens_dht::umidade(),
    cc::sens_solo::umidade(),
    0.0f //fazer...
  });
  cc::sens_solo::desliga();
  if (cc::wifi::conectado()) {
    char sql[512];
    for (auto it = leituras.begin(); it != leituras.end(); ) {
      const auto& l = *it;
      sprintf(
        sql, 
        "insert into SensorData (temperatura, umidade, SensorSolo, alturaReservPluv) values (%f, %f, %f, %f)",
        l.temperatura,
        l.umidade,
        l.umidade_solo,
        l.indice_pluv
      );
      try {
        cc::consulta_banco(sql);
        it = leituras.erase(it);
      }
      catch (String erro) {
        Serial.println(String("[Leituras] ") + erro);
        break;
      }
    }
  }
  delay(5000);
}
