#pragma once
#include <emscripten/bind.h>
#include <emscripten/val.h>



class CVManager
{
public:
    CVManager();

    void ImportByArray(emscripten::val array, int width, int height, int channel);
    void ImportByFile(std::string filename);
    void ImportByBase64(std::string base64string);

    

protected:
    
};



EMSCRIPTEN_BINDINGS(CVManager) {
    emscripten::class_<CVManager>("CVManager")
        .constructor()
        .function("ImportByArray", &CVManager::ImportByArray)
        .function("ImportByFile", &CVManager::ImportByFile)        
        .function("ImportByBase64", &CVManager::ImportByBase64);

};