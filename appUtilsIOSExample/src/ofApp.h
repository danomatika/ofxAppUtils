/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "ofxAppUtils.h"

#include "Button.h"

/// make sure you check out main.cpp for how to run this app with app utils!
///
/// this example mainly demonstrates using scenes, see appUtilsExample for
/// for info on using the transforms, simple particles, etc
///
/// it lets you cycle through scenes whose smaller render areas are scaled
/// and placed at the center of the screen
///
/// this example also handles rotation
///
class ofApp : public ofxApp{
	
	public:

		void setup();
		void update();
		void draw();
		void exit();

		void touchDown(ofTouchEventArgs & touch);
		void touchMoved(ofTouchEventArgs & touch);
		void touchUp(ofTouchEventArgs & touch);
		void touchDoubleTap(ofTouchEventArgs & touch);
		void touchCancelled(ofTouchEventArgs & touch);

		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		void deviceOrientationChanged(int newOrientation);
		
		// handles the scenes
		ofxSceneManager sceneManager;
		
		// simple scene change gui
		Button prevButton, nextButton;
};

