#pragma once

#include <math.h>

#include "config.h"
#include "ofMain.h"
#include "nanoflann.hpp"
#include "ImageDataSet.h"


class ofApp : public ofBaseApp{

	public:
    
    ofApp();
    ofApp(ImageDataSet & a_dataset);

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
    
    void fade(unsigned char* data, int size);
    void feed(unsigned char* result);
    
    int getBuffer();
    void updateTemplate();
    bool loadTemplate(const string path);
    
    string getLatestFile( ofDirectory& dir, const string ext );
    string getFirstFile( ofDirectory& dir, const string ext );
    bool deletePrevious( ofDirectory& directory, string filename);
    
    string latestResult;
    string previousResult;
    string templateFile;
    
    
    ofDirectory dir;
    ofDirectory result_dir;
    ofDirectory template_dir;
    ofDirectory queue_dir;
    
        ofPixels templateImagePixels;
        ofImage resultImage;
    
    ofBuffer imageData;
    unsigned char* data_ptr;
    
    ImageDataSet  dataset;
    ImageKDTree kdtree;
    
    int remaining = 0;
    int to_feed;
    
    int count = 0;
    
    bool there_is_buffer = false;
    bool there_was_buffer = false;
    bool is_random = false;
    
    size_t rand_pos;
    unsigned char rand_value[3];
    
};
