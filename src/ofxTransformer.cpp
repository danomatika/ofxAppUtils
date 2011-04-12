#include "ofxTransformer.h"

/// TRANSFORMER

//--------------------------------------------------------------
ofxTransformer::ofxTransformer() : _origin(0, 0, 0) {

	_bScale = false;
	_bMirrorX = false;
	_bMirrorY = false;
	_bTranslate = false;
	_bHandleAspect = false;
	_bWarp = false;
	
	_renderWidth = ofGetWidth();
	_renderHeight = ofGetHeight();
	_renderScaleX = 1;
	_renderScaleY = 1;
	_renderAspect = 1;
	_screenAspect = ofGetWidth()/ofGetHeight();
}

//--------------------------------------------------------------
void ofxTransformer::setRenderSize(float w, float h,
	float screenWidth, float screenHeight) {
	_renderAspect = w/h;
	_renderWidth = w;
	_renderHeight = h;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_renderScaleX = _screenWidth/_renderWidth;
	_renderScaleY = _screenHeight/_renderHeight;
	_screenAspect = _screenWidth/_screenHeight;
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
void ofxTransformer::setTransforms(bool translate, bool scale, bool warp, bool handleAspect) {
	_bTranslate = translate;
	_bScale = scale;
	_bWarp = warp;
	_bHandleAspect = handleAspect;
}

//--------------------------------------------------------------

void ofxTransformer::setMirror(bool mirrorX, bool mirrorY) {
	_bMirrorX = mirrorX;
	_bMirrorY = mirrorY;
}

void ofxTransformer::setMirrorX(bool mirrorX) {
	_bMirrorX = mirrorX;
}

void ofxTransformer::setMirrorY(bool mirrorY) {
	_bMirrorY = mirrorY;
}

void ofxTransformer::setOrigin(float x, float y, float z)	{
	_origin.set(x, y, z);
}

void ofxTransformer::setWarp(bool warp) {
	_bWarp = warp;
}

//--------------------------------------------------------------
void ofxTransformer::resetWarp() {
	_quadWarper.reset();
}

bool ofxTransformer::loadWarpSettings(const string xmlFile) {
	return _quadWarper.loadSettings(xmlFile);
}

void ofxTransformer::saveWarpSettings(const string xmlFile) {
	return _quadWarper.saveSettings(xmlFile);
}

//--------------------------------------------------------------
void ofxTransformer::setWarpPoint(unsigned int index, ofVec2f point) {
	_quadWarper.setPoint(index, point);
}

ofVec2f ofxTransformer::getWarpPoint(unsigned int index) {
	return _quadWarper.getPoint(index);
}

//--------------------------------------------------------------
void ofxTransformer::applyRenderScale() {
	if(_renderScaleX != 1.0 || _renderScaleY != 1.0) {
		
		// adjust to screen dimensions?
		if(_bHandleAspect && _renderAspect != _screenAspect) {
			
			if(_renderAspect == 1.0) { // square, windowbox
				ofTranslate((_screenWidth-(_renderScaleY*_renderWidth))/4,
							(_screenHeight+(_renderScaleX*_renderHeight))/4);
			}
			else if(_renderAspect < 1.0) { // w < h, letterbox
				//ofScale(_renderScaleX, _renderScaleX);
//				ofTranslate((0, _screenHeight-(_renderScaleX*_renderHeight))/2);
				ofScale(_renderScaleY, _renderScaleY);
				ofTranslate((_screenWidth-(_renderScaleY*_renderWidth))/2, 0);
			}
			else { // w > h, pillarbox
				//ofScale(_renderScaleY, _renderScaleY);
//				ofTranslate((_screenWidth-(_renderScaleY*_renderWidth))/2, 0);
				ofScale(_renderScaleX, _renderScaleX);
				ofTranslate((0, _screenHeight-(_renderScaleX*_renderHeight))/2);
			}
		
		} else { // basic stretch
			ofScale(_renderScaleX, _renderScaleY);
		}
	}
}

//--------------------------------------------------------------
void ofxTransformer::applyMirrorX() {
	ofRotateY(180);
	ofTranslate(-_renderWidth, 0, 0);
}

//--------------------------------------------------------------
void ofxTransformer::applyMirrorY() {
	ofRotateX(180);
	ofTranslate(0, -_renderHeight, 0);
}

//--------------------------------------------------------------
void ofxTransformer::applyOriginTranslate() {
	ofTranslate(_origin);
}

//--------------------------------------------------------------
void ofxTransformer::applyWarp() {
	_quadWarper.apply(_renderWidth, _renderHeight);
}

