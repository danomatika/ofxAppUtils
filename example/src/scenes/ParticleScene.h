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

class ParticleScene : public ofxScene {

	public:

		// takes a reference of the parent app for data access,
		// set the scene name through the base class initializer
		ParticleScene(testApp &app) : ofxScene((ofxApp&) app, "Particles") {
			alpha = 255;
			particles.setAutoRemove(false);	// don't remove particles if dead
		}

		// scene setup
		void setup() {
			timer.set();
		
			for(unsigned int i = 0; i < 100; ++i) {
				particles.addParticle(new Particle(app));
			}
		}

		// called when scene is entering
       	void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				timer.setAlarm(2000);
				alpha = 0;
				ofLogNotice() << "ParticleScene: update enter";
			}
		
			// calc alpha amount based on alarm time diff
			alpha = 255*timer.getDiffN();
			
			update();
		
			// call finishedEntering() to indicate scne is done entering
			if(timer.alarm()) {
				finishedEntering();
				alpha = 255;
				ofLogNotice() << "ParticleScene: update enter done";
			}
		}

		// normal update
        void update() {
			particles.update();
		}

		// called when scene is exiting
        void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				timer.setAlarm(2000);
				alpha = 0;
				ofLogNotice() << "ParticleScene: update exit";
			}
			
			// calc alpha amount based on alarm time diff
			alpha = 255*abs(timer.getDiffN()-1.0);
			
			update();
		
			// call finishedExiting() to indicate scene is done exiting
			if(timer.alarm()) {
				finishedExiting();
				alpha = 0;
				ofLogNotice() << "ParticleScene: update exit done";
			}
		}

		// draw
        void draw() {
			ofEnableAlphaBlending();
			ofFill();
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofSetColor(255, 0, 0, alpha);
			
			particles.draw();
				
			ofDisableAlphaBlending();
		}
		
		// cleanup
		void exit() {
			particles.clear();
		}
		
		// used for fade in and out
		ofxTimer timer;
		int alpha;

		// particle class		
		class Particle : public ofxParticle {

			public:
			
				// takes a reference of the parent app for data access,
				// here used to get the size of the render area
				// you can also cast the ofxApp reference to your own derived
				// class to pass custom data:
				//
				// TestApp& testApp = static_cast<TestApp&> (app);
				//
				Particle(ofxApp& app) : app(app) {
				
					// ofxParticle is derived from ofRectangle
					// so these variables are built in
					width = ofRandom(10, 40);
					height = width;
					x = ofRandom(width/2, app.getRenderWidth());
					y = ofRandom(height/2, app.getRenderHeight());
					
					vel.set(ofRandom(-5, 5), ofRandom(-5, 5));
				}
				
				void update() {
				
					// calc new pos
					x += vel.x;
					y += vel.y;
					
					// bounce on x axis
					if(x < width/2) {
						x = width/2;
						vel.x = -vel.x;
					}
					else if(x > app.getRenderWidth()-width/2) {
						x = app.getRenderWidth()-width/2;
						vel.x = -vel.x;
					}
					
					// bounce on y axis
					if(y < height/2) {
						y = height/2;
						vel.y = -vel.y;
					}
					else if(y > app.getRenderHeight()-height/2) {
						y = app.getRenderHeight()-height/2;
						vel.y = -vel.y;
					}
				}
				
				void draw() {
					ofRect(*this);	// <- use this object as an ofRectangle
				}
				
				ofVec2f vel;
				ofxApp& app;
		};
		
		// particle manager to wrangle our little ones
		ofxParticleManager particles;
};
