#include "cc_inicializacao.h"
#include "cc_terminal.h"
#include "cc_wifi.h"
#include "cc_util.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

cc::terminal term_serial(Serial);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SerialBT.begin("ClimaCare");
  delay(2000);

  cc::iniciar();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}
