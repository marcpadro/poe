#pragma once

#include "config.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        const char* url = 0;
		
};
