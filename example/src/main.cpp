#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800, 600, OF_WINDOW); // <-------- setup the GL context

	// this replaces ofRunApp and kicks off the running of the app
    // with the app utils
	ofRunAppWithAppUtils(new testApp());
}
