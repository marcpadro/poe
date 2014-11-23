#include "ofMain.h"
#include "ofApp.h"
#include "ofAppNoWindow.h"  

//========================================================================
int main(int argc, const char* argv[] ){
    
    
    ofAppNoWindow window;
    ofSetupOpenGL(&window,1024,768,OF_WINDOW);
    ofApp* app = new ofApp;
    if (argc > 1) app->url = argv[1];
    ofRunApp(app);
}
