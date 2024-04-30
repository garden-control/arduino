#include "cc_globais.h"

#ifdef USAR_SERIALBT
BluetoothSerial cc::SerialBT;
#endif

cc::mutex cc::mtx_spiffs;
