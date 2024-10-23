#include "cvmanager.h"
#include <iostream>


CVManager::CVManager(){

}

void CVManager::ImportByArray(emscripten::val array, int width, int height, int channel){

    // std::vector<char> array_in_vector = emscripten::convertJSArrayToNumberVector<char>(array); // crash
    std::cout << "Array Importe : " << width << "x" << height << "x" << channel << std::endl;
}   

void CVManager::ImportByFile(std::string filename){

}

void CVManager::ImportByBase64(std::string base64string){

    std::cout << "BASE64 String Converted" << std::endl;

}
