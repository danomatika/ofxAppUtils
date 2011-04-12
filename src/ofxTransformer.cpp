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
void ofxTransformer::setRenderSize(float w, float h, bool handleAspectRatio,
								   bool translate, bool useWarp) {
	_bHandleAspect = handleAspectRatio;
	_bTranslate = translate;
	_bWarp = useWarp;
	_renderAspect = w/h;
	_renderWidth = w;
	_renderHeight = h;
	_renderScaleX = (float) ofGetWidth()/_renderWidth;
	_renderScaleY = (float) ofGetHeight()/_renderHeight;
}

//--------------------------------------------------------------
void ofxTransformer::setRenderScale(float x, float y) {
	_renderScaleX = x;
	_renderScaleY = y;	
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
		if(_bHandleAspect) {
			
			if(_renderAspect == 1.0) { // square, windowbox
				ofTranslate((ofGetWidth()-(_renderScaleY*_renderWidth))/4,
							(ofGetHeight()+(_renderScaleX*_renderHeight))/4);
			}
			else if(_renderAspect < 1.0) { // w < h, letterbox
				ofScale(_renderScaleX, _renderScaleX);
				ofTranslate((0, ofGetHeight()-(_renderScaleX*_renderHeight))/4);
			}
			else { // w > h, pillarbox
				ofScale(_renderScaleY, _renderScaleY);
				ofTranslate((ofGetWidth()-(_renderScaleY*_renderWidth))/2, 0);
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

