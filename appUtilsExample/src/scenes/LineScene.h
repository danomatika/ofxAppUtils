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

class LineScene : public ofxFadeScene {

	public:

		// set the scene name through the base class initializer
		LineScene() : ofxFadeScene("Lines"){
			setSingleSetup(false); // call setup each time the scene is loaded
			setFade(1000, 1000); // 1 second fade in/out
		}

		// scene setup
		void setup() {
			lines.push_back(new Line(Line::HORZ));
			lines.push_back(new Line(Line::VERT));
		}

		// called when scene is entering, this is just a demo and this
		// implementation is not required for this class
		void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				ofLogNotice("LineScene") << "update enter";
			}
		
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateEnter();
			
			// finished entering?
			if(!isEntering()) {
				ofLogNotice("LineScene") << "update enter done";
			}
		}

		// normal update
		void update() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->update();
			}
		}

		// called when scene is exiting, this is just a demo and this
		// implementation is not required for this class
		void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				ofLogNotice("LineScene") << "update exit";
			}
			
			// fade scene calculates normalized alpha value for us
			ofxFadeScene::updateExit();
			
			// finished exiting?
			if(!isExiting()) {
				ofLogNotice("LineScene") << "update exit done";
			}
		}

		// draw
		void draw() {
			ofEnableAlphaBlending();
			ofSetLineWidth(5);
			ofSetColor(255, 255, 255, 255*alpha);	// alpha member variable for fade in/out
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->draw();
			}
			ofSetLineWidth(1);
			ofDisableAlphaBlending();
		}
		
		// cleanup
		void exit() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				Line *l = lines[i];
				delete l;
			}
			lines.clear();
		}

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
					// TestApp *testApp = (TestApp*) (ofxGetAppPtr());
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
					ofDrawLine(pos1, pos2);
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
