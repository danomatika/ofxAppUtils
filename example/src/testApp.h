#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include <ofxAppUtils.h>

class testApp : public ofxApp {
	
	public:
	
		testApp() : sceneManager(*this) {}
	
		void setup();
		void exit();
		
		void updateApp();
		void drawApp();

		void keyPressedApp(int key);
		void keyReleasedApp(int key);
		void mouseMovedApp(int x, int y);
		void mouseDraggedApp(int x, int y, int button);
		void mousePressedApp(int x, int y, int button);
		void mouseReleasedApp(int x, int y, int button);

		/// handles the scenes
		ofxSceneManager sceneManager;
};

#endif
