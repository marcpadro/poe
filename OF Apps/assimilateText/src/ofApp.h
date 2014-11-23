#pragma once

#include "ofMain.h"
#include "config.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void feed();
    inline long long compareDistances(unsigned long origin, unsigned long dest1, unsigned long dest2);
    
    int getBuffer();
    void updateTemplate();
    void loadTemplate(const string path);
    
    string getLatestFile( ofDirectory& dir, const string ext );
    string getFirstFile( ofDirectory& dir, const string ext );
    bool deletePrevious( ofDirectory& directory, string filename);
    
    string latestResult = "";
    string previousResult = "";
    string templateFile = "";
    
    size_t charToWchar(unsigned long* dest, const char* source);
    size_t wcharToChar(char* dest, const unsigned long* source);
    size_t numBytes(unsigned long* wide);
		
    ofDirectory dir;
    ofDirectory result_dir;
    ofDirectory template_dir;
    ofDirectory queue_dir;
    
    unsigned long templateText [TEXT_SIZE] = {};
    
    size_t template_length;
    
    unsigned long resultText [TEXT_SIZE] = {};
    
    ofBuffer textData;
    
    unsigned long* wide_data;
    
    size_t to_feed;
    size_t remaining = 0;
    
    int count = 0;
    
    bool there_is_buffer = false;
    bool there_was_buffer = false;
    bool is_random = false;
};
