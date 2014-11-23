#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"

//========================================================================
int main(int argc, const char* argv[] ){
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    
    
    ofAppNoWindow window;
    ofSetupOpenGL(&window,1024,768,OF_WINDOW);
    ofApp* app = new ofApp;
    if (argc > 1) app->url = argv[1];
    ofRunApp(app);
}
