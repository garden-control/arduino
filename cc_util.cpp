#include "cc_util.h"

String cc::espera_linha(Stream& stream) {
  String linha;
  while (!stream.available()) delay(10);
  while (stream.available() && stream.peek() != '\n' && stream.peek() != '\r') {
    linha += (char)stream.read();
    delay(10);
  }
  while (stream.available()) {
    stream.read();
    delay(10);
  }
  return linha;
}

String cc::pegarString(Stream& stream)
{
    String str;

    while (stream.available() && stream.peek() == ' ') stream.read();
    
    if (stream.peek() == '"')
    {
        stream.read();
        str = stream.readStringUntil('"');
        stream.read();
    }
    else
    {
        str = stream.readStringUntil(' ');
    }

    return str;
}

bool cc::analisarLinhaDeComando(const std::map<std::string, char>& params, Stream& stream, std::map<std::string, void*>& args, String& erros)
{
    while (stream.available())
    {
        String param = pegarString(stream);
        if (param.length() > 0)
        {
            if (param[0] == '-')
            {
                if (params.count(param.c_str()))
                {
                    switch (params.at(param.c_str()))
                    {
                    case 's':
                        args[param.c_str()] = new String(pegarString(stream));
                        break;
                    case 'f':
                        args[param.c_str()] = new float(stream.parseFloat());
                        break;
                    case 'i':
                        args[param.c_str()] = new int(stream.parseInt());
                        break;
                    case 'b':
                        args[param.c_str()] = new bool(true);
                        break;
                    }
                }
                else
                {
                    erros += "Parametro \"" + param + "\" nao existe\n";
                }
            }
            else
            {
                erros += "Nomes de parametros devem comecar com traco (-)\n";
            }
        }
    }

    return erros.isEmpty();
}

size_t cc::getLength(const __FlashStringHelper *ifsh)
{
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    size_t n = 0;
    while (1) {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0) break;
        n++;
    }
    return n;
}

bool cc::listarDir(Stream& stream, fs::FS &fs, const char * dirname, uint8_t levels)
{    
    DEBUG_SERIAL("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        DEBUG_SERIAL("- failed to open directory");
        return false;
    }
    if(!root.isDirectory()){
        DEBUG_SERIAL(" - not a directory");
        return false;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            stream.print("  DIR : ");
            stream.println(file.name());
            if(levels){
                listarDir(stream, fs, file.path(), levels -1);
            }
        } else {
            stream.print("  FILE: ");
            stream.print(file.name());
            stream.print("\tSIZE: ");
            stream.println(file.size());
        }
        file = root.openNextFile();
    }

    return true;
}

bool cc::lerArquivo(fs::FS &fs, const char * path, uint8_t* dest, size_t destMax)
{
    DEBUG_SERIAL("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        DEBUG_SERIAL("- failed to open file for reading");
        return false;
    }

    DEBUG_SERIAL("- read from file:");
    for (int i = 0; file.available() && i < destMax; i++){
        dest[i] = file.read();
    }
    file.close();
    return true;
}

bool cc::escreverArquivo(fs::FS &fs, const char * path, const uint8_t* message, size_t size){
    DEBUG_SERIAL("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        DEBUG_SERIAL("- failed to open file for writing");
        return false;
    }
    
    if(file.write(message, size) == size){
        DEBUG_SERIAL("- file written");
    } else {
        DEBUG_SERIAL("- write failed");
        return false;
    }
    file.close();
    return false;
}

bool cc::anexarAoArquivo(fs::FS &fs, const char * path, const uint8_t* message, size_t size){
    DEBUG_SERIAL("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        DEBUG_SERIAL("- failed to open file for appending");
        return false;
    }
    if(file.write(message, size) == size){
        DEBUG_SERIAL("- message appended");
    } else {
        DEBUG_SERIAL("- append failed");
        return false;
    }
    file.close();
    return true;
}

bool cc::renomearArquivo(fs::FS &fs, const char * path1, const char * path2){
    DEBUG_SERIAL("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        DEBUG_SERIAL("- file renamed");
    } else {
        DEBUG_SERIAL("- rename failed");
        return false;
    }
    return true;
}

bool cc::apagarArquivo(fs::FS &fs, const char * path){
    DEBUG_SERIAL("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        DEBUG_SERIAL("- file deleted");
    } else {
        DEBUG_SERIAL("- delete failed");
        return false;
    }
    return true;
}

