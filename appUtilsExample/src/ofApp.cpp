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
#include "ofApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// setup for nice jaggy-less rendering
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);

	// setup the render size (working area)
	setRenderSize(600, 400);

	// turn on transform origin translation and scaling to screen size,
	// disable quad warping, and enable aspect ratio and centering when scaling
	setTransforms(true, true, false, true, true);

	// the control panel is setup automatically, of course you can still change
	// all the settings manually here

	// add the built in transform control to control panel (adds new panel)
	//
	// loads and saves control panel settings to "controlPanelSettings.xml"
	// in the data folder 
	addTransformControls();
	
	// load saved control panel settings
	// loads and saves to "controlPanelSettings.xml" in the data folder
	// or use your own filename
	// note: this may override what was set with setTransforms
	loadControlSettings();
	
	// load saved quad warper settings
	// loads and saves to "quadWarper.xml" in the data folder
	// or use your own filename
	loadWarpSettings();
	
	// load scenes
	particleScene = (ParticleScene*) sceneManager.add(new ParticleScene()); // save pointer
	sceneManager.add(new LineScene());
	sceneManager.setup(true); // true = setup all the scenes now (not on the fly)
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	
	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.gotoScene("Lines", true);
	lastScene = sceneManager.getCurrentSceneIndex();
	
	// attach scene manager to this ofxApp so it's called automatically,
	// you can also call the callbacks (update, draw, keyPressed, etc) manually
	// if you don't set it
	//
	// you can also turn off the auto sceneManager update and draw calls with:
	// setSceneManagerUpdate(false);
	// setSceneManagerDraw(false);
	//
	// the input callbacks in your scenes will be called if they are implemented
	//
	setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void ofApp::update() {

	// the control panel and current scene are automatically updated before
	// this function
}

//--------------------------------------------------------------
void ofApp::draw() {

	// the current scene is automatically drawn before this function

	// show the render area edges with a white rect
	if(bDebug) {
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
	}
	
	// drop out of the auto transform space back to OF screen space
	popTransforms();
	
	// draw current scene info using the ofxBitmapString stream interface
	// to ofDrawBitmapString
	ofSetColor(200);
	ofxBitmapString(12, ofGetHeight()-8)
		<< "Current Scene: " << sceneManager.getCurrentSceneIndex()
		<< " " << sceneManager.getCurrentSceneName() << endl;
	
	// go back to the auto transform space
	//
	// this is actually done automatically if the transforms were popped
	// before the control panel is drawn, but included here for completeness
	pushTransforms();

	// the control panel and warp editor are drawn automatically after this
	// function
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	switch (key) {
	
		case 'd':
			bDebug = !bDebug;
			break;
			
		case 'a':
			setAspect(!getAspect());
			break;
			
		case 'c':
			setCentering(!getCentering());
			break;
			
		case 'm':
			setMirrorX(!getMirrorX());
			break;
			
		case 'n':
			setMirrorY(!getMirrorY());
			break;
			
		case 'q':
			setWarp(!getWarp());
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
	
		case OF_KEY_LEFT:
			sceneManager.prevScene();
			break;
			
		case OF_KEY_RIGHT:
			sceneManager.nextScene();
			break;
			
		case OF_KEY_DOWN:
			if(sceneManager.getCurrentScene()) { // returns NULL if no scene selected
				lastScene = sceneManager.getCurrentSceneIndex();
			}
			sceneManager.noScene();
			break;
			
		case OF_KEY_UP:
			sceneManager.gotoScene(lastScene);
			break;
			
		case '-':
			if(sceneManager.getCurrentScene() == particleScene) {
				particleScene->removeOneParticle();
			}
			break;
			
		case '=':
			if(sceneManager.getCurrentScene() == particleScene) {
				particleScene->addOneParticle();
			}
			break;
			
		case 'o':
			sceneManager.setOverlap(!sceneManager.getOverlap());
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

	// set up transforms with new screen size
	setNewScreenSize(w, h);
}
