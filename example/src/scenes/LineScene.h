#pragma once

#include <ofxAppUtils.h>

class LineScene : public ofxScene {

	public:

		// takes a reference of the parent for data access,
		// set the scene name through the base class initializer
		LineScene(testApp &app) : ofxScene((ofxApp&) app, "Lines") {}

		// scene setup
		void setupScene() {
			timer.set();
			
			lines.push_back(new Line(app, Line::HORZ));
			lines.push_back(new Line(app, Line::VERT));
		}

		// called when scene is entering
       	void updateSceneEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				timer.setAlarm(5000);
				alpha = 0;
			}
		
			alpha = 255*timer.getDiffN();
			updateScene();
		
			// call finishedEntering() to indicate scne is done entering
			if(timer.alarm()) {
				finishedEntering();
				alpha = 255;
			}
		}

		// normal update
        void updateScene() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->update();
			}
		}

		// called when scene is exiting
        void updateSceneExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				timer.setAlarm(5000);
				alpha = 0;
			}
			
			alpha = 255*abs(timer.getDiffN()-1.0);
			updateScene();
		
			// call finishedExiting() to indicate scene is done exiting
			if(timer.alarm()) {
				finishedExiting();
				alpha = 0;
			}
		}

		// draw
        void drawScene() {
			ofSetLineWidth(5);
			ofSetColor(255, 255, 255, alpha);
			for(unsigned int i = 0; i < lines.size(); ++i) {
				lines[i]->draw();
			}
			ofSetLineWidth(1);
		}
		
		// cleanup
		void cleanupScene() {
			for(unsigned int i = 0; i < lines.size(); ++i) {
				Line* l = lines[i];
				delete l;
			}
		}
		
		// used for fade in and out
		ofxTimer timer;
		int alpha;

		// line class		
		class Line {

			public:
			
				Line(ofxApp &app, int type) : app(app) {
					this->type = (Type) type;
				}
				
				void update() {
					switch(type) {
						case HORZ:
							pos1.set(0, ofNoise(ofGetElapsedTimef())*app.getRenderHeight());
							pos2.set(app.getRenderWidth(), ofNoise(ofGetElapsedTimef())*app.getRenderHeight());
							break;
						case VERT:
							pos1.set(ofNoise(ofGetElapsedTimef())*app.getRenderHeight(), 0);
							pos2.set(ofNoise(ofGetElapsedTimef())*app.getRenderHeight(), app.getRenderHeight());
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
				ofxApp& app;				
		};
		
		// lines
		vector<Line*> lines;
};
