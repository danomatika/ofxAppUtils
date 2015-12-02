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

// a simple scene that displays some BIG TEXT in the center of the screen
class TextScene : public ofxFadeScene {
	
	public:

		// takes a reference of the parent for data access,
		// set the scene name through the base class initializer
		TextScene(string name, string t) : ofxFadeScene(name) {
			text = t;
			setFade(500, 500); // half second fade in/out
		}

		// scene setup
		void setup() {
			// load the font if not already loaded
			if(!font.isLoaded()) {
				font.load("vera.ttf", 300);
			}
			bbox = font.getStringBoundingBox(text, 0, 0);
		}

		// called when scene is entering, this is just a demo and this
		// implementation is not required for this class
		void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				ofLogNotice() << getName() << ": update enter";
			}
		
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateEnter();
			
			// finished entering?
			if(!isEntering()) {
				ofLogNotice() << getName() << ": update enter done";
			}
		}

		// normal update
        void update() {}

		// called when scene is exiting, this is just a demo and this
		// implementation is not required for this class
		void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				ofLogNotice() << getName() << ": update exit";
			}
			
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateExit();
			
			// finished exiting?
			if(!isExiting()) {
				ofLogNotice() << getName() << ": update exit done";
			}
		}

		// draw
        void draw() {
		
			ofxApp *app = ofxGetAppPtr();
		
			ofEnableAlphaBlending();
			ofSetColor(255, 255, 255, 255*alpha); // alpha member variable for fade in/out
			font.drawString(text,
				app->getRenderWidth()/2-bbox.width/2,
				app->getRenderHeight()/2+bbox.height/2);
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
};

// statics
ofTrueTypeFont TextScene::font;
