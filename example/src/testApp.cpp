#include "testApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void testApp::setupApp(){

	// setup for nice jaggy-less rendering (if your machine/drivers dont suck)
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	// setup the render size (working area)
	setRenderSize(600, 400);
	
	// turn on transform origin translation and scaling to screen size
	setTransforms(true, true);
	
	// the control panel is setup automatically, of course you can still change
	// all the settings manually here

	// add the built in transform control to control panel (adds new panel)
	addTransformControls();
	
	// load scenes
	sceneManager.add(new ParticleScene(*this));
	sceneManager.add(new LineScene(*this));
	sceneManager.setup(true);	// true = setup all the scenes now (not on the fly)
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	
	// start with a specific scene
	sceneManager.gotoScene("Lines", true);
	
	// attach scene manager to this ofxApp so it's called automatically,
	// you can also call the callbacks (update, draw, keyPressed, etc) manually
	setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void testApp::updateApp(){

	// the control panel and current scene are automatically updated
	
}

//--------------------------------------------------------------
void testApp::drawApp(){

	ofBackground(0, 0, 0);

	// show the render area edges with a white rect
	if(bDebug) {
		ofNoFill();
		ofSetColor(255);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
	}
	
	// the current scene and control panel are automatically drawn
}

//--------------------------------------------------------------
void testApp::keyPressedApp(int key) {
	
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
void testApp::mouseMovedApp(int x, int y) {
}

//--------------------------------------------------------------
void testApp::mouseDraggedApp(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressedApp(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleasedApp(int x, int y, int button){
}