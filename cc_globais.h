#ifndef CC_GLOBAIS_H
#define CC_GLOBAIS_H

// Seriais
//#define USAR_SERIALBT
#define USAR_SERIAL


#include <Arduino.h>

#ifdef USAR_SERIALBT
#include <BluetoothSerial.h>
#endif

#include "cc_mutex.h"

namespace cc
{
    extern Stream *pDebugStream;

#ifdef USAR_SERIALBT
    extern BluetoothSerial SerialBT;
#endif

    extern mutex mtx_spiffs;
}

#endif