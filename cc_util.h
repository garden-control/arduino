#ifndef CC_UTIL_H
#define CC_UTIL_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <StreamString.h>
#include <map>
#include <string>
#include "cc_globais.h"

#ifdef USAR_DEBUG_SERIAL
    #define DEBUG_SERIAL(formato, ...) Serial.printf(formato, ##__VA_ARGS__)
#else
    #define DEBUG_SERIAL(formato, ...) ((void)0)
#endif

namespace cc
{
    String espera_linha(Stream &stream);

    // trata strings entre aspas
    String pegarString(Stream& stream);

    size_t getLength(const __FlashStringHelper *ifsh);

    bool listarDir(Stream& stream, fs::FS &fs, const char * nomeDiretorio, uint8_t subniveis);
    bool lerArquivo(fs::FS &fs, const char * caminho, uint8_t* destino, size_t tamanhoMaximo);
    bool escreverArquivo(fs::FS &fs, const char * caminho, const uint8_t* dados, size_t tamanho);
    bool anexarAoArquivo(fs::FS &fs, const char * caminho, const uint8_t* dados, size_t tamanho);
    bool renomearArquivo(fs::FS &fs, const char * caminho1, const char * caminho2);
    bool apagarArquivo(fs::FS &fs, const char * caminho);

    /*
        Exemplo:
        
        analisarLinhaDeComando({
            {"--ssid", 's'},       // nome do parametro, formato
            {"--senha", 's'}
        }, stream, args, erros);

        Formatos disponíveis:
            s: String
            f: float
            i: int
            b: bool
        
    */
    bool analisarLinhaDeComando(const std::map<std::string, char>& params, Stream& stream, std::map<std::string, void*>& args, String& erros);
};

#endif