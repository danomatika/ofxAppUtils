#include "ofxScene.h"


/// SCENE

//--------------------------------------------------------------
void ofxScene::setup() {
	if(!_bSetup){
		setupScene();
		_bSetup = true;
	}
}

//--------------------------------------------------------------
void ofxScene::update() {
 	if(!_bSetup || !_bRunning)
		return;

	if(_bEntering) {
		updateSceneEnter();
		_bEnteringFirst = false;
	}
	else if(_bExiting) {
		updateSceneExit();
		_bExitingFirst = false;
	}
	else {
		updateScene();
	}
}
		
//--------------------------------------------------------------
void ofxScene::draw() {
	if(!_bSetup)
		return;
	drawScene();			
}

//--------------------------------------------------------------
void ofxScene::exit() {
	cleanupScene();			
}
