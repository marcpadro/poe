#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBuffer buffer;
    char* buffer_ptr;
    int size = 0;
    std::ofstream outputFile;
    bool is_random = false;
    
    std::srand((unsigned int)ofGetSystemTimeMicros());
    
    
    if (url != 0) {
        
        ofHttpResponse response = ofLoadURL(url);
        buffer_ptr = response.data.getBinaryBuffer();
        
        size = response.data.size();
        
#if (MAX_SIZE > 0)
        if (size > MAX_SIZE) size = MAX_SIZE;
#endif
        
        cout << size << "\n";
        
        unsigned long* wide = new unsigned long [size];
        
        buffer_ptr[size-1] = 0;
        size_t numChars = charToWchar(wide, buffer_ptr);
            
            for(size_t i = 0; i < numChars; i++) {
                size_t random = (std::rand() % numChars);
                std::swap(wide[i], wide[random]);
            }
        
        wcharToChar(buffer_ptr, wide);
        delete [] wide;
        
    } else {
        is_random = true;
        size = 1;
    }
    
    int i = 0;
#if (MAX_BUFFER > 0)
    for ( ; size > 0; size -= MAX_BUFFER, i += MAX_BUFFER) {
#endif
        ostringstream stream;
        stream << SAVE_DIR;
        stream << ofGetUnixTime();
        stream << ofGetSystemTimeMicros();
        if (is_random) stream << "R";
        stream << ".ebb";
        string path = stream.str();
        int block = ((size > MAX_BUFFER) && (MAX_BUFFER > 0)) ? MAX_BUFFER : size;
        outputFile.open( path.c_str(), ios::binary);
        if (outputFile.is_open()) {
            if (!is_random) outputFile.write( &buffer_ptr[i], block);
            outputFile.put(0);
            outputFile.close();
        }
#if (MAX_BUFFER > 0)
        ofSleepMillis(10);
    }
#endif
    
    ofExit();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
size_t ofApp::charToWchar(unsigned long* dest, const char* source) {
    size_t i = 0;
    size_t wi = 0;
    while ( source[i] != 0 ) {
        unsigned long current = (unsigned char)source[i];
        i++;
        if ( (char)current < 0 ) {
            while ( (source[i] < (char)0300) ) {
                current = (current << 8) | (source[i] & 0xff);
                i++;
            }
        }
        dest[wi] = current;
        wi++;
    }
    dest[wi] = 0;
    
    return wi;
}

//--------------------------------------------------------------
size_t ofApp::wcharToChar(char* dest, const unsigned long* source){
    
    size_t i = 0;
    size_t di = 0;
    while ( source[i] != 0 ) {
        if (source[i] & 0x80000000) dest[di++] = (source[i] >> 24) & 0xff;
        if (source[i] & 0x00800000) dest[di++] = (source[i] >> 16) & 0xff;
        if (source[i] & 0x00008000) dest[di++] = (source[i] >> 8) & 0xff;
        dest[di++] = source[i] & 0xff;
        i++;
    }
    dest[di] = 0;
    
    return di;
}

//--------------------------------------------------------------
size_t ofApp::numBytes(unsigned long* wide){
    
    size_t n = 1;
    
    for (size_t i = 0; wide[i] != 0; i++) {
        n++;
        if (wide[i] & 0x00008000) n++;
        if (wide[i] & 0x00800000) n++;
        if (wide[i] & 0x80000000) n++;
    }
    
    return n;
}

