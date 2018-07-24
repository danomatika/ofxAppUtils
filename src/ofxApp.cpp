/*
 * Copyright (c) 2011-2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#include "ofxApp.h"

#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofLog.h"
#include "ofxSceneManager.h"

// APP

//--------------------------------------------------------------
ofxApp::ofxApp() {
	bDebug = false;
	_bAutoTransforms = true;
	_bDrawFramerate = true;
	_framerateColor.set(255);	// white
	_transformer = NULL;
	_sceneManager = NULL;
	_bSceneManagerUpdate = true;
	_bSceneManagerDraw = true;
}

// TRANSFORMER

//--------------------------------------------------------------
void ofxApp::setTransformer(ofxTransformer *transformer) {
	if(transformer == NULL) {
		ofLogWarning("ofxApp") << "cannot add NULL transformer";
		return;
	}
	_transformer = transformer;
}

//--------------------------------------------------------------
ofxTransformer* ofxApp::getTransformer() {
	return _transformer;
}

//--------------------------------------------------------------
void ofxApp::clearTransformer() {
	_transformer = NULL;
}

//--------------------------------------------------------------
float ofxApp::getRenderWidth() {
	return (_transformer == NULL) ? ofGetWidth() : _transformer->getRenderWidth();
}

//--------------------------------------------------------------
float ofxApp::getRenderHeight() {
	return (_transformer == NULL) ? ofGetHeight() : _transformer->getRenderHeight();
}

// SCENE MANAGER

//--------------------------------------------------------------
void ofxApp::setSceneManager(ofxSceneManager *manager) {
	if(manager == NULL) {
		ofLogWarning("ofxApp") << "cannot add NULL scene manager";
		return;
	}
	_sceneManager = manager;
}

//--------------------------------------------------------------
ofxSceneManager* ofxApp::getSceneManager() {
	return _sceneManager;
}
		
//--------------------------------------------------------------
void ofxApp::clearSceneManager() {
	_sceneManager = NULL;
}

// DRAW FRAMERATE

//--------------------------------------------------------------
void ofxApp::drawFramerate(float x, float y) {
	ofSetColor(_framerateColor);
	ofDrawBitmapStringHighlight("fps: "+ofToString(ceil(ofGetFrameRate())), x, y);
}

// RUNNER APP

//--------------------------------------------------------------
ofxApp::RunnerApp::RunnerApp(ofxApp *app) {
	this->app = app;
}

//--------------------------------------------------------------
ofxApp::RunnerApp::~RunnerApp() {
	delete app;
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::setup() {
	
	// set transform sizes here, since width/height aren't set yet in main.cpp
	if(app->getTransformer()) {
		app->getTransformer()->clearTransforms();
		app->getTransformer()->setRenderSize(ofGetWidth(), ofGetHeight());
	}
	app->setup();
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::update() {
	app->mouseX = mouseX;
	app->mouseY = mouseY;
	if(app->_sceneManager && app->_bSceneManagerUpdate) {
		app->_sceneManager->update();
	}
	app->update();
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::draw() {
	
	// push transforms
	if(app->_bAutoTransforms && app->_transformer) {
		app->_transformer->push(app->_transformer->getEditWarp());
	}
	
	// draw scene
	if(app->_sceneManager && app->_bSceneManagerDraw) {
		app->_sceneManager->draw();
	}
		
	// do the user callback
	app->draw();

	// pop transforms & draw the quad warper bounds (if editing)
	if(app->_transformer) {
		app->_transformer->popWarp();
		//app->_transformer->drawWarpBounds();
		app->_transformer->pop();
	}
	
	// draw debug quad warper editor (if editing) & framerate
	if(app->bDebug) {
		if(app->_transformer) {
			app->_transformer->drawWarpEditor();
		}
		if(app->_bDrawFramerate) {
			app->drawFramerate(ofGetWidth()-60, ofGetHeight()-6);
		}
	}
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::exit() {
	app->exit();
	if(app->_sceneManager) {
		app->_sceneManager->clear();
	}
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::windowResized(int w, int h) {
	if(app->_sceneManager) {
		app->_sceneManager->windowResized(w, h);
	}
	if(app->getTransformer()) {
		app->_transformer->setNewScreenSize(w, h);
	}
	app->windowResized(w, h);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::keyPressed(int key) {
	if(app->_sceneManager) {
		app->_sceneManager->keyPressed(key);
	}
	app->keyPressed(key);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::keyReleased(int key) {
	if(app->_sceneManager) {
		app->_sceneManager->keyReleased(key);
	}
	app->keyReleased(key);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mouseMoved(int x, int y) {
	if(app->_sceneManager) {
		app->_sceneManager->mouseMoved(x, y);
	}
	app->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mousePressed(int x, int y, int button) {
	if(app->_sceneManager) {
		app->_sceneManager->mousePressed(x, y, button);
	}
	if(app->bDebug && app->getTransformer()) {
		app->getTransformer()->mousePressed(x, y, button);
	}
	else {
		app->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mouseDragged(int x, int y, int button) {
	if(app->_sceneManager) {
		app->_sceneManager->mouseDragged(x, y, button);
	}
	if(app->bDebug && app->getTransformer()) {
		app->getTransformer()->mouseDragged(x, y, button);
	}
	else {
		app->mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mouseReleased(int x, int y, int button) {
	if(app->_sceneManager) {
		app->_sceneManager->mouseReleased(x, y, button);
	}
	if(app->bDebug && app->getTransformer()) {
		app->getTransformer()->mouseReleased(x, y, button);
	}
	else {
		app->mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	if(app->_sceneManager) {
		app->_sceneManager->mouseScrolled(x, y, scrollX, scrollY);
	}
	app->mouseScrolled(x, y, scrollX, scrollY);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mouseEntered(int x, int y) {
	if(app->_sceneManager) {
		app->_sceneManager->mouseEntered(x, y);
	}
	app->mouseEntered(x, y);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::mouseExited(int x, int y) {
	if(app->_sceneManager) {
		app->_sceneManager->mouseExited(x, y);
	}
	app->mouseExited(x, y);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::dragEvent(ofDragInfo dragInfo) {
	if(app->_sceneManager) {
		app->_sceneManager->dragEvent(dragInfo);
	}
	app->dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::gotMessage(ofMessage msg){
	if(app->_sceneManager) {
		app->_sceneManager->gotMessage(msg);
	}
	app->gotMessage(msg);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::touchDown(int x, int y, int id) {
	if(app->_sceneManager) {
		app->_sceneManager->touchDown(x, y, id);
	}
	app->touchDown(x, y, id);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::touchMoved(int x, int y, int id) {
	if(app->_sceneManager) {
		app->_sceneManager->touchMoved(x, y, id);
	}
	app->touchMoved(x, y, id);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::touchUp(int x, int y, int id) {
	if(app->_sceneManager) {
		app->_sceneManager->touchUp(x, y, id);
	}
	app->touchUp(x, y, id);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::touchDoubleTap(int x, int y, int id) {
	if(app->_sceneManager) {
		app->_sceneManager->touchDoubleTap(x, y, id);
	}
	app->touchDoubleTap(x, y, id);
}

//--------------------------------------------------------------
void ofxApp::RunnerApp::touchCancelled(int x, int y, int id) {
	if(app->_sceneManager) {
		app->_sceneManager->touchCancelled(x, y, id);
	}
	app->touchCancelled(x, y, id);
}

// ofBaseSoundInput
//--------------------------------------------------------------
void ofxApp::RunnerApp::audioIn(ofSoundBuffer& buffer) {
	if(app->_sceneManager) {
		app->_sceneManager->audioIn(buffer);
	}
	app->audioIn(buffer);
}

void ofxApp::RunnerApp::audioIn(float *input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
	if(app->_sceneManager) {
		app->_sceneManager->audioIn(input, bufferSize, nChannels, deviceID, tickCount);
	}
	app->audioIn(input, bufferSize, nChannels, deviceID, tickCount);
}

void ofxApp::RunnerApp::audioIn(float *input, int bufferSize, int nChannel ) {
	if(app->_sceneManager) {
		app->_sceneManager->audioIn(input, bufferSize, nChannel);
	}
	app->audioIn(input, bufferSize, nChannel);
}
void ofxApp::RunnerApp::audioReceived(float *input, int bufferSize, int nChannels) {
	if(app->_sceneManager) {
		app->_sceneManager->audioIn(input, bufferSize, nChannels);
	}
	app->audioIn(input, bufferSize, nChannels);
}

// ofBaseSoundOutput
//--------------------------------------------------------------
void ofxApp::RunnerApp::audioOut(ofSoundBuffer& buffer) {
	if(app->_sceneManager) {
		app->_sceneManager->audioOut(buffer);
	}
	app->audioOut(buffer);
}

void ofxApp::RunnerApp::audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
	if(app->_sceneManager) {
		app->_sceneManager->audioOut(output, bufferSize, nChannels, deviceID, tickCount);
	}
	app->audioOut(output, bufferSize, nChannels, deviceID, tickCount);
}

void ofxApp::RunnerApp::audioOut(float *output, int bufferSize, int nChannels) {
	if(app->_sceneManager) {
		app->_sceneManager->audioOut(output, bufferSize, nChannels);
	}
	app->audioOut(output, bufferSize, nChannels);
}

void ofxApp::RunnerApp::audioRequested(float *output, int bufferSize, int nChannels) {
	if(app->_sceneManager) {
		app->_sceneManager->audioOut(output, bufferSize, nChannels);
	}
	app->audioOut(output, bufferSize, nChannels);
}

#ifdef TARGET_OF_IOS
// ofxiOSApp
//--------------------------------------------------------------
void ofxApp::RunnerApp::lostFocus() {
	if(app->_sceneManager) {
		app->_sceneManager->lostFocus();
	}
	app->lostFocus();
}

void ofxApp::RunnerApp::gotFocus() {
	if(app->_sceneManager) {
		app->_sceneManager->gotFocus();
	}
	app->gotFocus();
}

void ofxApp::RunnerApp::gotMemoryWarning() {
	if(app->_sceneManager) {
		app->_sceneManager->gotMemoryWarning();
	}
	app->gotMemoryWarning();
}

void ofxApp::RunnerApp::deviceOrientationChanged(int newOrientation) {
	if(app->_sceneManager) {
		app->_sceneManager->deviceOrientationChanged(newOrientation);
	}
	if(app->_transformer) {
		app->_transformer->setNewScreenSize(ofGetWidth(), ofGetHeight());
	}
	app->deviceOrientationChanged(newOrientation);
}
#endif
