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
#pragma once

#include <ofxAppUtils.h>

class LineScene : public ofxScene {

	public:

		// set the scene name through the base class initializer
		LineScene() : ofxScene("Lines"){
			alpha = 255;
			
			// we want setup to be called each time the scene is loaded
			setSingleSetup(false);
		}

		// scene setup
		void setup() {
			timer.set();
			
			lines.push_back(new Line(Line::HORZ));
			lines.push_back(new Line(Line::VERT));
		}

		// called when scene is entering
		void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				timer.setAlarm(2000);
				alpha = 0;
				ofLogNotice("LineScene") << "update enter";
			}
		
			// calc alpha amount based on alarm time diff
			alpha = 255*timer.getDiffN();
			
			update();
		
			// call finishedEntering() to indicate scene is done entering
			if(timer.alarm()) {
				finishedEntering();
				alpha = 255;
				ofLogNotice("LineScene") << "update enter done";
			}
		}

		// normal update
		void update() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->update();
			}
		}

		// called when scene is exiting
		void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				timer.setAlarm(2000);
				alpha = 0;
				ofLogNotice("LineScene") << "update exit";
			}
			
			// calc alpha amount based on alarm time diff
			alpha = 255*abs(timer.getDiffN()-1.0);
			
			update();
		
			// call finishedExiting() to indicate scene is done exiting
			if(timer.alarm()) {
				finishedExiting();
				alpha = 0;
				ofLogNotice("LineScene") << "update exit done";
			}
		}

		// draw
		void draw() {
			ofEnableAlphaBlending();
			ofSetLineWidth(5);
			ofSetColor(255, 255, 255, alpha);	// alpha for fade in/out
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->draw();
			}
			ofSetLineWidth(1);
			ofDisableAlphaBlending();
		}
		
		// cleanup
		void exit() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				Line* l = lines[i];
				delete l;
			}
			lines.clear();
		}
		
		// used for fade in and out
		ofxTimer timer;
		int alpha;

		// line class		
		class Line {

			public:
			
				Line(int type) {
					this->type = (Type) type;
				}
				
				void update() {
				
					// get a pointer to the parent app for data access,
					// here used to get the size of the render area
					// you can also cast the ofxApp reference to your own derived
					// class to pass custom data:
					//
					// TestApp* testApp = (TestApp*) (ofxGetAppPtr());
					//
					// NOTE: you must use "ofxGetAppPtr()" <-- note the "x",
					// this is a replacement for "ofGetAppPtr()" which does not
					// return the pointer to the correct app instance
					//
					ofxApp *app = ofxGetAppPtr();
					
					switch(type) {
						case HORZ:
							pos1.set(0, ofNoise(ofGetElapsedTimef())*app->getRenderHeight());
							pos2.set(app->getRenderWidth(), ofNoise(ofGetElapsedTimef())*app->getRenderHeight());
							break;
						case VERT:
							pos1.set(ofNoise(ofGetElapsedTimef())*app->getRenderHeight(), 0);
							pos2.set(ofNoise(ofGetElapsedTimef())*app->getRenderHeight(), app->getRenderHeight());
							break;
					}
				}
				
				void draw() {
					ofLine(pos1, pos2);
				}
				
				enum Type {
					HORZ,
					VERT
				};
				int type;
				
				ofVec2f pos1, pos2;				
		};
		
		// lines
		vector<Line*> lines;
};
