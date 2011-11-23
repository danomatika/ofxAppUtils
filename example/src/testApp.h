/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxAppUtils.h"

/// make sure you check out main.cpp for how to run this app with app utils!
///
/// controls:
///     - L & R arrows to change scenes (note: wait for transition)
///     - 'd' key to toggle debug mode and enable control panel
///     - 'm' key to mirror the render area on the x axis
///     - 'n' key to mirror the render area on the y axis
///     - 'q' key to toggle to quad warper
///     - 'f' to toggle fullscreen mode
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
