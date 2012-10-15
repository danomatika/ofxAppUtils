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
#include "ofxAppUtils.h"

#include "testApp.h"

// a simple scene that displays some BIG TEXT in the center of the screen
class TextScene : public ofxScene {
	
    public:

		// takes a reference of the parent for data access,
		// set the scene name through the base class initializer
		TextScene(testApp &app, string name, string t) : app(app), ofxScene(name) {
			text = t;
			alpha = 255;
//			// we want setup to be called each time the scene is loaded
//			setSingleSetup(false);
		}

		// scene setup
		void setup() {
			// load the font if not already loaded
			if(!font.isLoaded()) {
				font.loadFont("vera.ttf", 300);
			}
			bbox = font.getStringBoundingBox(text, 0, 0);
		}

		// called when scene is entering
       	void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				setup();
				timer.set();
				timer.setAlarm(500);
				alpha = 0;
				ofLogNotice() << getName() << ": update enter";
			}
		
			// calc alpha amount based on alarm time diff
			alpha = 255*timer.getDiffN();
			
			update();
		
			// call finishedEntering() to indicate scene is done entering
			if(timer.alarm()) {
				finishedEntering();
				alpha = 255;
				ofLogNotice() << getName() << ": update enter done";
			}
		}

		// normal update
        void update() {}

		// called when scene is exiting
        void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				timer.setAlarm(500);
				alpha = 0;
				ofLogNotice() << getName() << ": update exit";
			}
			
			// calc alpha amount based on alarm time diff
			alpha = 255*abs(timer.getDiffN()-1.0);
			
			update();
		
			// call finishedExiting() to indicate scene is done exiting
			if(timer.alarm()) {
				finishedExiting();
				alpha = 0;
				ofLogNotice() << getName() << ": update exit done";
			}
		}

		// draw
        void draw() {
			ofEnableAlphaBlending();
			ofSetColor(255, 255, 255, alpha);	// alpha for fade in/out
			font.drawString(text,
				app.getRenderWidth()/2-bbox.width/2,
				app.getRenderHeight()/2+bbox.height/2);
			ofDisableAlphaBlending();
		}
		
		// cleanup
		void exit() {}

		// input callbacks
        void touchDown(ofTouchEventArgs & touch) {
			ofLogNotice() << getName() << ": touchDown, id " << touch.id
				<< " at " << touch.x << " " << touch.y;
		}
        void touchMoved(ofTouchEventArgs & touch) {
			ofLogNotice() << getName() << ": touchMoved, id " << touch.id
				<< " at " << touch.x << " " << touch.y;
		}
        void touchUp(ofTouchEventArgs & touch) {
			ofLogNotice() << getName() << ": touchUp, id " << touch.id
				<< " at " << touch.x << " " << touch.y;
		}
        void touchDoubleTap(ofTouchEventArgs & touch) {
			ofLogNotice() << getName() << ": touchDoubleTap, id " << touch.id
				<< " at " << touch.x << " " << touch.y;
		}
        void touchCancelled(ofTouchEventArgs & touch) {
			ofLogNotice() << getName() << ": touchCancelled, id " << touch.id
				<< " at " << touch.x << " " << touch.y;
		}

        void lostFocus() {
			ofLogNotice() << getName() << ": lostFocus";
		}
        void gotFocus() {
			ofLogNotice() << getName() << ": gotFocus";
		}
        void gotMemoryWarning() {
			ofLogNotice() << getName() << ": gotMemoryWarning";
        }
		void deviceOrientationChanged(int newOrientation) {
			ofLogNotice() << getName() << ": deviceOrientationChanged";
		}

		string text; // some text to show
		ofRectangle bbox; // bounding box of the text
		
		static ofTrueTypeFont font; // shared font

		// used for fade in and out
		ofxTimer timer;
		int alpha;
		
		// the parent
		testApp& app;
};

// statics
ofTrueTypeFont TextScene::font;
