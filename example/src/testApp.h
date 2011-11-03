#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include <ofxAppUtils.h>

/// make sure you check out main.cpp for how to run this app with app utils!
///
class testApp : public ofxApp {
	
	public:
	
        // setup the scene manager with a reference to the parent app
		testApp() : sceneManager(*this) {}
	
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		// handles the scenes
		ofxSceneManager sceneManager;
};

#endif
