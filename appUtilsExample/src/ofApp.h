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

#include "ofMain.h"

#include "ofxAppUtils.h"

// optional ofxGUI control panel for transformer,
// uncomment this if you're using ofxGui in this project
//#define HAVE_OFX_GUI

#ifdef HAVE_OFX_GUI
	#include "ofxGui.h"
	#include "ofxTransformPanel.h"
#endif

// we only specify a pointer to the Particle Scene here, so tell the compiler
// that it exists as a class and we can include the class header in ofApp.cpp
class ParticleScene;

/// make sure you check out main.cpp for how to run this app with app utils!
///
/// controls:
///     - L & R arrows to change scenes (note: wait for 2s transition)
///     - D arrow to change to "no scene" & U arrow to return to last scene
///     - 'd' key to toggle debug mode and enable control panel & warp editor
///     - 'm' key to mirror the render area on the x axis
///     - 'n' key to mirror the render area on the y axis
///     - 'a' key to enable aspect ratio scaling
///     - 'c' key to enable auto centering, only applied if apsect ratio scaling
///           is on and the quad warper is off
///     - 'q' key to toggle to quad warper
///     - 'f' to toggle fullscreen mode
///     - '-' & '=': remove/add particles when the particle scene is running
///     - 'o' to toggle scene overlap
///
class ofApp : public ofxApp { // <-- inherits ofxApp instead of ofBaseApp
	
	public:
	
		ofApp() {}
	
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		void windowResized(int w, int h);

		// rendering transformer
		ofxTransformer transformer;

		// handles the scenes
		ofxSceneManager sceneManager;
		int lastScene;
		
		// keep a pointer to the Particle Scene, so we can modify it more easily
		ParticleScene *particleScene;
	
		#ifdef HAVE_OFX_GUI
			ofxTransformPanel panel;
		#endif
};
