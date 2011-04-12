#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include <ofxAppUtils.h>

class testApp : public ofxApp {
	
	public:
	
		testApp() : sceneManager(*this) {}
	
		// note: these are the ofxApp callbacks ...
		// do not implement the regular ofBaseApp callbacks
		// (setup, draw, etc) as they are used by ofxApp which then calls these
	
		void setupApp();
		void updateApp();
		void drawApp();

		void keyPressedApp(int key);
		void keyReleasedApp(int key);
		void mouseMovedApp(int x, int y);
		void mouseDraggedApp(int x, int y, int button);
		void mousePressedApp(int x, int y, int button);
		void mouseReleasedApp(int x, int y, int button);

		// handles the scenes
		ofxSceneManager sceneManager;
};

#endif
