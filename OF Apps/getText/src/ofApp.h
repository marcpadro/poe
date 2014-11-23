#pragma once

#include "config.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    size_t charToWchar(unsigned long* dest, const char* source);
    size_t wcharToChar(char* dest, const unsigned long* source);
    size_t numBytes(unsigned long* wide);
    
    const char* url = 0;
    
};
