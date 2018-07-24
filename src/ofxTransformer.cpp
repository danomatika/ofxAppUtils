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
#include "ofxTransformer.h"

#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofFileUtils.h"

//--------------------------------------------------------------
ofxTransformer::ofxTransformer() {
	_bScale = false;
	_bMirrorX = false;
	_bMirrorY = false;
	_bTranslate = false;
	_bAspect = false;
	_bCenter = false;
	_bWarp = false;
	
	_renderWidth = 1;  // avoid /0
	_renderHeight = 1; // avoid /0
	_renderScaleX = 1;
	_renderScaleY = 1;
	_renderAspect = 1;
	_screenAspect = 1;
	
	_bEditingWarpPoints = false;
	_currentWarpPoint = -1;
	
	_bTransformsPushed = false;
}

//--------------------------------------------------------------
void ofxTransformer::clearTransforms() {
	_bScale = false;
	_bMirrorX = false;
	_bMirrorY = false;
	_bTranslate = false;
	_bAspect = false;
	_bCenter = false;
	_bWarp = false;
	
	_renderWidth = ofGetWidth();
	_renderHeight = ofGetHeight();
	_renderScaleX = 1;
	_renderScaleY = 1;
	_renderAspect = 1;
	_screenAspect = ofGetWidth()/ofGetHeight();
	
	_bTransformsPushed = false;
}

// RENDER SIZE

//--------------------------------------------------------------
void ofxTransformer::setRenderSize(float w, float h, float screenWidth, float screenHeight) {
	_renderAspect = w/h;
	_renderWidth = w;
	_renderHeight = h;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_renderScaleX = _screenWidth/_renderWidth;
	_renderScaleY = _screenHeight/_renderHeight;
	_screenAspect = _screenWidth/_screenHeight;
	_quadWarper.setSize(w, h);
}

//--------------------------------------------------------------
void ofxTransformer::setRenderSize(float w, float h) {
	setRenderSize(w, h, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofxTransformer::setRenderScale(float x, float y) {
	_renderScaleX = x;
	_renderScaleY = y;	
}

//--------------------------------------------------------------
float ofxTransformer::getRenderWidth()	{
	return _renderWidth;
}

//--------------------------------------------------------------
float ofxTransformer::getRenderHeight()	{
	return _renderHeight;
}

//--------------------------------------------------------------
float ofxTransformer::getScreenWidth()	{
	return _screenWidth;
}

//--------------------------------------------------------------
float ofxTransformer::getScreenHeight() {
	return _screenHeight;
}

//--------------------------------------------------------------
void ofxTransformer::applyRenderScale() {
	// adjust to screen dimensions?
	if(_bAspect && _renderAspect != _screenAspect) {
		if(_renderAspect > _screenAspect) {	// letter box
			if(_bCenter && !_bWarp) {
				ofTranslate(0, (_screenHeight-(_renderScaleX*_renderHeight))/2);
			}
			ofScale(_renderScaleX, _renderScaleX);
		}
		else { // pillar box
			if(_bCenter && !_bWarp) {
				ofTranslate((_screenWidth-(_renderScaleY*_renderWidth))/2, 0);
			}
			ofScale(_renderScaleY, _renderScaleY);
		}
	}
	else { // basic stretch
		ofScale(_renderScaleX, _renderScaleY);
	}
}

//--------------------------------------------------------------
void ofxTransformer::applyMirrorX() {
	ofRotateYDeg(180);
	ofTranslate(-_renderWidth, 0, 0);
}

//--------------------------------------------------------------
void ofxTransformer::applyMirrorY() {
	ofRotateXDeg(180);
	ofTranslate(0, -_renderHeight, 0);
}

//--------------------------------------------------------------
void ofxTransformer::applyTranslate() {
	ofTranslate(_position);
}

//--------------------------------------------------------------
void ofxTransformer::pushWarp() {
	_quadWarper.push();
}

//--------------------------------------------------------------
void ofxTransformer::popWarp() {
	_quadWarper.pop();
}

//--------------------------------------------------------------
void ofxTransformer::push(bool forceWarp) {
	if(_bTransformsPushed) {
		return; // don't push twice
	}
	ofPushMatrix();
	if(_bScale) {
		applyRenderScale();
	}
	if(_bTranslate) {
		applyTranslate();
	}
	if(_bWarp || forceWarp) {
		pushWarp();
	}
	if(_bMirrorX) {
		applyMirrorX();
	}
	if(_bMirrorY) {
		applyMirrorY();
	}
	_bTransformsPushed = true;
}

//--------------------------------------------------------------
void ofxTransformer::pop() {
	if(!_bTransformsPushed) {
		return; // avoid extra pops
	}
	if(_bWarp) {
		popWarp();
	}
	ofPopMatrix();
	_bTransformsPushed = false;
}

//--------------------------------------------------------------
bool ofxTransformer::isPushed() {
	return _bTransformsPushed;
}

//--------------------------------------------------------------
void ofxTransformer::setTransforms(bool translate, bool scale, bool warp, bool aspect, bool center) {
	_bTranslate = translate;
	_bScale = scale;
	_bWarp = warp;
	_bAspect = aspect;
	_bCenter = center;
}

//--------------------------------------------------------------
void ofxTransformer::setNewScreenSize(float screenWidth, float screenHeight) {
	setRenderSize(_renderWidth, _renderHeight, screenWidth, screenHeight);
}

/// SETTINGS

//--------------------------------------------------------------
float ofxTransformer::getRenderScaleX() {
	return _renderScaleX;
}

//--------------------------------------------------------------
float ofxTransformer::getRenderScaleY() {
	return _renderScaleY;
}

//--------------------------------------------------------------
void ofxTransformer::setScale(bool scale) {
	_bScale = scale;
}

//--------------------------------------------------------------
bool ofxTransformer::getScale() {
	return _bScale;
}

//--------------------------------------------------------------
void ofxTransformer::setAspect(bool aspect) {
	_bAspect = aspect;
}

//--------------------------------------------------------------
bool ofxTransformer::getAspect() {
	return _bAspect;
}

//--------------------------------------------------------------
void ofxTransformer::setCentering(bool center) {
	_bCenter = center;
}

//--------------------------------------------------------------
bool ofxTransformer::getCentering() {
	return _bCenter;
}

//--------------------------------------------------------------
void ofxTransformer::setMirror(bool mirrorX, bool mirrorY) {
	_bMirrorX = mirrorX;
	_bMirrorY = mirrorY;
}

//--------------------------------------------------------------
void ofxTransformer::setMirrorX(bool mirrorX) {
	_bMirrorX = mirrorX;
}

//--------------------------------------------------------------
void ofxTransformer::setMirrorY(bool mirrorY) {
	_bMirrorY = mirrorY;
}

//--------------------------------------------------------------
bool ofxTransformer::getMirrorX() {
	return _bMirrorX;
}

//--------------------------------------------------------------
bool ofxTransformer::getMirrorY() {
	return _bMirrorY;
}

//--------------------------------------------------------------
void ofxTransformer::setTranslate(bool translate) {
	_bTranslate = translate;
}

//--------------------------------------------------------------
bool ofxTransformer::getTranslate() {
	return _bTranslate;
}

//--------------------------------------------------------------
void ofxTransformer::setPosition(float x, float y, float z)	{
	_position.set(x, y, z);
}

//--------------------------------------------------------------
void ofxTransformer::setPosition(const ofPoint &point) {
	_position = point;
}

//--------------------------------------------------------------
const ofPoint& ofxTransformer::getPosition() {
	return _position;
}

//--------------------------------------------------------------
void ofxTransformer::setWarp(bool warp) {
	_bWarp = warp;
}

//--------------------------------------------------------------
bool ofxTransformer::getWarp() {
	return _bWarp;
}

// QUAD WARPER

//--------------------------------------------------------------
void ofxTransformer::resetWarp() {
	_quadWarper.reset();
}

//--------------------------------------------------------------
bool ofxTransformer::loadWarpSettings(const std::string &xmlFile) {
	if(ofFile::doesFileExist(ofToDataPath(xmlFile))) {
		return _quadWarper.loadSettings(xmlFile);
	}
	return false;
}

//--------------------------------------------------------------
void ofxTransformer::saveWarpSettings(const std::string &xmlFile) {
	return _quadWarper.saveSettings(xmlFile);
}

//--------------------------------------------------------------
void ofxTransformer::setEditWarp(bool edit) {
	_bEditingWarpPoints = edit;
}

//--------------------------------------------------------------
bool ofxTransformer::getEditWarp() {
	return _bEditingWarpPoints;
}

//--------------------------------------------------------------
void ofxTransformer::setWarpPoint(unsigned int index, const ofVec2f &point) {
	_quadWarper.setPoint(index, point);
}

//--------------------------------------------------------------
const ofVec2f& ofxTransformer::getWarpPoint(unsigned int index) {
	return _quadWarper.getPoint(index);
}

//--------------------------------------------------------------
void ofxTransformer::drawWarpBounds() {
	if(!_bEditingWarpPoints) {
		return;
	}

	ofPushStyle();
	
	// push transforms if needed (for manual mode)
	bool doTransform = !_bTransformsPushed;
	if(doTransform) {
		push(true);
	}
	
	ofNoFill();
	ofSetColor(ofColor::green);
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofDrawRectangle(0.35, 0.35, _renderWidth-1.35, _renderHeight-1.35);
	
	if(doTransform) {
		pop();
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxTransformer::drawWarpEditor() {
	if(!_bEditingWarpPoints) {
		return;
	}

	ofPushStyle();

	// draw the quad warper editor
	std::stringstream text;
	text << "Quad Warper Edit Mode" << std::endl
		 << "Drag the corner control points" << std::endl
		 << "Click center rectangle to exit";
	ofDrawBitmapStringHighlight(text.str(), 28, 28, ofColor::black, ofColor::green);
		
	// draw center exit box
	ofSetColor(ofColor::green);
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofDrawRectangle(ofGetWidth()/2, ofGetHeight()/2, 100, 100);

	ofFill();
	_quadWarper.drawPoints();

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxTransformer::mousePressed(int x, int y, int button) {
	if(!_bEditingWarpPoints) {
		return;
	}
	
	// check if middle of the screen was pressed to exit edit mode
	if((x > ofGetWidth()/2  - 50 && (x < ofGetWidth()/2  + 50) &&
	   (y > ofGetHeight()/2 - 50 && (y < ofGetHeight()/2 + 50))))
	{
		_bEditingWarpPoints = false;
		return;
	}
	
	// check if the screen corners are being clicked
	float smallestDist = 1.0;
	_currentWarpPoint = -1;
	for(int i = 0; i < 4; i++) {
		float distx = _quadWarper.getPoint(i).x - (float) (x)/ofGetWidth();
		float disty = _quadWarper.getPoint(i).y - (float) (y)/ofGetHeight();
		float dist  = sqrt(distx * distx + disty * disty);
		if(dist < smallestDist && dist < 0.1) {
			_currentWarpPoint = i;
			smallestDist = dist;
		}
	}
}

//--------------------------------------------------------------
void ofxTransformer::mouseDragged(int x, int y, int button) {
	if(!_bEditingWarpPoints) {
		return;
	}
	if(_currentWarpPoint > -1) {
		_quadWarper.setPoint(_currentWarpPoint, (float)x/ofGetWidth(), (float)y/ofGetHeight());
	}
}

//--------------------------------------------------------------
void ofxTransformer::mouseReleased(int x, int y, int button) {
	if(!_bEditingWarpPoints) {
		return;
	}
	_currentWarpPoint = -1;
}

