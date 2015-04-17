/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#include "ofApp.h"

#include "TextScene.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// setup for nice jaggy-less rendering
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);

	// setup the render size (working area)
	transformer.setRenderSize(640, 480); // 4:3
	
	// setup the render transforms
	transformer.setScale(true); // scale to screen size
	transformer.setAspect(true); // keep aspect ratio when scaling
	transformer.setCentering(true); // center render area in screen
	
	// you could also do the same with the setTransforms() function
	//
	// turn off transform origin translation and scaling to screen size,
	// disable quad warping, and enable aspect ratio and centering when scaling
	//transformer.setTransforms(false, true, false, true, true);
	
	// apply the transformer automatically
	setTransformer(&transformer);
	
	// load scenes
	sceneManager.add(new TextScene("Scene One", "1"));
	sceneManager.add(new TextScene("Scene Two", "2"));
	sceneManager.add(new TextScene("Scene Three", "3"));
	sceneManager.add(new TextScene("Scene A", "A"));
	sceneManager.add(new TextScene("Scene B", "B"));
	sceneManager.add(new TextScene("Scene C", "C"));
	sceneManager.setup();
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	
	// start with a specific scene
	// set now to true in order to ignore the scene fade and change now
	sceneManager.gotoScene(0, true);
	
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
	
	// setup gui
	prevButton.setType(Button::LEFT);
	nextButton.setType(Button::RIGHT);
	prevButton.set(1, ofGetHeight()-51, 50, 50);
	nextButton.set(ofGetWidth()-51, ofGetHeight()-51, 50, 50);
}

//--------------------------------------------------------------
void ofApp::update(){
	// current scene is automatically updated before this function
}

//--------------------------------------------------------------
void ofApp::draw(){
	// the current scene is automatically drawn before this function
	
	// show the render area edges with a white rect
	ofNoFill();
	ofSetColor(127);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
	ofFill();
	
	// drop out of the auto transform space back to OF screen space
	transformer.pop();
	
	// draw the buttons
	prevButton.draw();
	nextButton.draw();
	
	// draw current scene info using the ofxBitmapString stream interface
	// to ofDrawBitmapString
	ofSetColor(200);
	ofxBitmapString(10, 22)
		<< "Current Scene: #" << sceneManager.getCurrentSceneIndex()
		<< " " << sceneManager.getCurrentSceneName();
	
	// go back to the auto transform space
	//
	// this is actually done automatically if the transforms were popped
	// before the control panel is drawn, but included here for completeness
	transformer.push();
}

// current scene input functions are called automatically before calling these
//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){	
	prevButton.inside(touch.x, touch.y);
	nextButton.inside(touch.x, touch.y);
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
	prevButton.inside(touch.x, touch.y);
	nextButton.inside(touch.x, touch.y);
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
	if(prevButton.isInside) {
		sceneManager.prevScene();
		prevButton.isInside = false;
	}
	if(nextButton.isInside) {
		sceneManager.nextScene();
		nextButton.isInside = false;
	}
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
	sceneManager.nextScene();
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
	
	// rotate graphics world ...
	ofSetOrientation((ofOrientation) newOrientation);
	
	// transformer.setNewScreenSize() is automatically called if the transformer is set
	
	// set button pos again
	prevButton.set(1, ofGetHeight()-51, 50, 50);
	nextButton.set(ofGetWidth()-51, ofGetHeight()-51, 50, 50);
}

