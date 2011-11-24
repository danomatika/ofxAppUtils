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
#include "testApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void testApp::setup() {

	// setup for nice jaggy-less rendering (if your machine/drivers dont suck)
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	// setup the render size (working area)
	setRenderSize(600, 400);
	
	// turn on transform origin translation and scaling to screen size
	// also apsect ratio and centering
	setTransforms(true, true, false, true, true);
	
	// the control panel is setup automatically, of course you can still change
	// all the settings manually here

	// add the built in transform control to control panel (adds new panel)
    //
    // loads and saves control panel settings to "controlPanelSettings.xml"
    // in the data folder 
	addTransformControls();
    
    // load saved quad warper settings
    // loads and saves to "quadWarper.xml" in the data folder
    loadWarpSettings();
	
	// load scenes
	sceneManager.add(new ParticleScene(*this));
	sceneManager.add(new LineScene(*this));
	sceneManager.setup(true);	// true = setup all the scenes now (not on the fly)
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	
	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.gotoScene("Lines", true);
	
	// attach scene manager to this ofxApp so it's called automatically,
	// you can also call the callbacks (update, draw, keyPressed, etc) manually
    // if you don't set it
	setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void testApp::update() {

	// the control panel and current scene are automatically updated
	
}

//--------------------------------------------------------------
void testApp::draw() {

	// the current scene is drawn before this function automatically

	ofBackground(0, 0, 0);

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
	
	// draw current scene info using ofxBitmapString stream interface
	// to ofDrawBitmapString
	ofSetColor(200);
	ofxBitmapString(12, ofGetHeight()-8)
		<< "Current Scene: #" << sceneManager.getCurrentSceneIndex()
		<< " " << sceneManager.getCurrentSceneName() << endl;
	
	// go back to the auto transform space
	pushTransforms();

	// the control panel is drawn automatically after this function automatically
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	switch (key) {
	
		case 'd':
			bDebug = !bDebug;
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
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}