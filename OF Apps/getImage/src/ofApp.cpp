#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofPixels pixels;
    unsigned char* pix_p = 0;
    int size = 0;
    std::ofstream outputFile;
    bool is_random = false;
    
    std::srand((unsigned int)ofGetSystemTimeMicros());
    
    
    if (url != 0) {
        if (ofLoadImage(pixels, url)) {
            
            pixels.setImageType(OF_IMAGE_COLOR);
            
            size = pixels.size();
#if (MAX_SIZE > 0)
            if (size > MAX_SIZE) size = MAX_SIZE;
#endif
            int numpix = size/3;
            pix_p = pixels.getPixels();
            
            cout << size << "\n";
            
            
            for(int i = 0; i < size; i += 3) {
                int random = (std::rand() % numpix)*3;
                std::swap(pix_p[i], pix_p[random]);
                std::swap(pix_p[i+1], pix_p[random+1]);
                std::swap(pix_p[i+2], pix_p[random+2]);
            }
        } else ofExit();
        
    } else {
        
        cout << "random\n";
        
        is_random = true;
        
        size = RAND_BUFFER_SIZE;
        
        pix_p = new unsigned char[RAND_BUFFER_SIZE];
        
        int i = RAND_BUFFER_SIZE;
        while (i--) {
            pix_p[i] = std::rand();
        }
        
    }
    
#if (MAX_BUFFER > 0)
    for ( ; size > 0; size -= MAX_BUFFER, pix_p += MAX_BUFFER) {
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
            outputFile.write( (char*) pix_p, block);
            outputFile.close();
        }
#if (MAX_BUFFER > 0)
        ofSleepMillis(10);
    }
#endif
    
    
    delete [] pix_p;
    
    ofExit();

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}