#include "cc_globais.h"

Stream* cc::pDebugStream = &Serial;

#ifdef USAR_SERIALBT
BluetoothSerial cc::SerialBT;
#endif

cc::mutex cc::mtx_spiffs;
