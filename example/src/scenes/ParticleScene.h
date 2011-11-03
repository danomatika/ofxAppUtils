#pragma once

#include <ofxAppUtils.h>

class ParticleScene : public ofxScene {

	public:

		// takes a reference of the parent app for data access,
		// set the scene name through the base class initializer
		ParticleScene(testApp &app) : ofxScene((ofxApp&) app, "Particles")
		{
			alpha = 255;
		}

		// scene setup
		void setup() {
			timer.set();
		
			for(unsigned int i = 0; i < 100; ++i) {
				particles.push_back(new Particle(app));
			}
		}

		// called when scene is entering
       	void updateEnter() {
		
			// called on first enter update
			if(isEnteringFirst()) {
				timer.setAlarm(5000);
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
			for(unsigned int i = 0; i < particles.size(); ++i)
				particles[i]->update();
		}

		// called when scene is exiting
        void updateExit() {
		
			// called on first exit update
			if(isExitingFirst()) {
				timer.setAlarm(5000);
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
			
			for(unsigned int i = 0; i < particles.size(); ++i)
				particles[i]->draw();
				
			ofDisableAlphaBlending();
		}
		
		// cleanup
		void exit() {
			for(unsigned int i = 0; i < particles.size(); ++i) {
				Particle* p = particles[i];
				delete p;
			}
		}
		
		// used for fade in and out
		ofxTimer timer;
		int alpha;

		// particle class		
		class Particle {

			public:
			
				// takes a reference of the parent app for data access,
				// here used to get the size of the render area
				// you can also cast the ofxApp reference to your own derived
				// class to pass custom data:
				//
				// TestApp& testApp = static_cast<TestApp&> (app);
				//
				Particle(ofxApp& app) : app(app) {
					size = ofRandom(10, 40);
					pos.set(ofRandom(size/2, app.getRenderWidth()),
							ofRandom(size/2, app.getRenderHeight()));
					vel.set(ofRandom(-5, 5), ofRandom(-5, 5));
				}
				
				void update() {
					pos += vel;
					
					// bounce on x axis
					if(pos.x < size/2) {
						pos.x = size/2;
						vel.x = -vel.x;
					}
					else if(pos.x > app.getRenderWidth()-size/2) {
						pos.x = app.getRenderWidth()-size/2;
						vel.x = -vel.x;
					}
					
					// bounce on y axis
					if(pos.y < size/2) {
						pos.y = size/2;
						vel.y = -vel.y;
					}
					else if(pos.y > app.getRenderHeight()-size/2) {
						pos.y = app.getRenderHeight()-size/2;
						vel.y = -vel.y;
					}
				}
				
				void draw() {
					ofRect(pos, size, size);
				}
				
				ofVec2f pos;
				ofVec2f vel;
				int size;
				ofxApp& app;
		};
		
		// the particles
		vector<Particle*> particles;
};
