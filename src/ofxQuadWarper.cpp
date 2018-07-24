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
#include "ofxQuadWarper.h"

#include "matrix_funcs.h"
#include "ofConstants.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofXml.h"

//--------------------------------------------------------------
ofxQuadWarper::ofxQuadWarper() {
	reset();
	_bPushed = false;
}

//--------------------------------------------------------------
void ofxQuadWarper::setSize(float width, float height) {
	_width  = width;
	_height = height;
	updateMatrix();
}

//--------------------------------------------------------------
void ofxQuadWarper::push() {
	if(_bPushed) {
		return;
	}
	ofPushMatrix();
	ofMultMatrix(_glWarpMatrix);
	_bPushed = true;
}

//--------------------------------------------------------------
void ofxQuadWarper::pop() {
	if(!_bPushed) {
		return;
	}
	ofPopMatrix();
	_bPushed = false;
}

//--------------------------------------------------------------
bool ofxQuadWarper::isPushed() {
	return _bPushed;
}

//--------------------------------------------------------------
void ofxQuadWarper::drawPoints() {
	drawPoints(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofxQuadWarper::drawPoints(float width, float height) {
	//ofSetRectMode(OF_RECTMODE_CENTER);
	for(int i = 0; i < 4; i++) {
		ofDrawRectangle(_warpPoints[i].x * width, _warpPoints[i].y * height, 10, 10);
	}
	//ofSetRectMode(OF_RECTMODE_CORNER);
}

//--------------------------------------------------------------
void ofxQuadWarper::setPoint(unsigned int index, float x, float y) {
	if(index > 3) {
		return;
	}
	_warpPoints[index].x = x;
	_warpPoints[index].y = y;
	updateMatrix();
}

//--------------------------------------------------------------
void ofxQuadWarper::setPoint(unsigned int index, const ofVec2f &point) {
	if(index > 3) {
		return;
	}
	_warpPoints[index] = point;
	updateMatrix();
}

//--------------------------------------------------------------
const ofVec2f& ofxQuadWarper::getPoint(unsigned int index) {
	if(index > 3) {
		ofLogWarning("ofxQuadWarper") << "ignoring out of bounds index " << index << ", returning point 0";
		return _warpPoints[0];
	}
	return _warpPoints[index];
}

//--------------------------------------------------------------
void ofxQuadWarper::reset() {
	_width = 1;  // avoid /0
	_height = 1; // avoid /0
	_warpPoints[0].set(0.0, 0.0);
	_warpPoints[1].set(1.0, 0.0);
	_warpPoints[2].set(1.0, 1.0);
	_warpPoints[3].set(0.0, 1.0);
}

//--------------------------------------------------------------
bool ofxQuadWarper::loadSettings(const std::string &xmlFile) {
	ofXml xml;
	if(!xml.load(xmlFile)) {
		return false;
	}
	auto root = xml.getChild("quad");
	if(!root) {
		return false;
	}

    auto child = root.findFirst("upperLeft");
    if(child) {
		_warpPoints[0].x = child.getChild("x").getFloatValue();
		_warpPoints[0].y = child.getChild("y").getFloatValue();
	}

	child = root.findFirst("upperRight");
    if(child) {
		_warpPoints[1].x = child.getChild("x").getFloatValue();
		_warpPoints[1].y = child.getChild("y").getFloatValue();
	}

	child = root.findFirst("lowerRight");
    if(child) {
		_warpPoints[2].x = child.getChild("x").getFloatValue();
		_warpPoints[2].y = child.getChild("y").getFloatValue();
	}

	child = root.findFirst("lowerLeft");
    if(child) {
		_warpPoints[3].x = child.getChild("x").getFloatValue();
		_warpPoints[3].y = child.getChild("y").getFloatValue();
	}
	
	updateMatrix();
	
	return true;
}

//--------------------------------------------------------------
void ofxQuadWarper::saveSettings(const std::string &xmlFile) {

	ofXml xml;
	auto root = xml.appendChild("quad");

	auto child = root.appendChild("upperLeft");
	child.appendChild("x").set(_warpPoints[0].x);
	child.appendChild("y").set(_warpPoints[0].y);

	child = root.appendChild("upperRight");
	child.appendChild("x").set(_warpPoints[1].x);
	child.appendChild("y").set(_warpPoints[1].y);

	child = root.appendChild("lowerRight");
	child.appendChild("x").set(_warpPoints[2].x);
	child.appendChild("y").set(_warpPoints[2].y);

	child = root.appendChild("lowerLeft");
	child.appendChild("x").set(_warpPoints[3].x);
	child.appendChild("y").set(_warpPoints[3].y);

	xml.save(xmlFile);
}

// PROTECTED

//--------------------------------------------------------------
//
// projection warping from a modified Theo example on the OF forums:
// http://threeblindmiceandamonkey.com/?p=31
//
void ofxQuadWarper::updateMatrix() {
	
	// we set it to the default - 0 translation
	// and 1.0 scale for x y z and w
	for(int i = 0; i < 16; i++) {
		if(i % 5 != 0) {
			_glWarpMatrix[i] = 0.0;
		}
		else {
			_glWarpMatrix[i] = 1.0;
		}
	}

	// source and destination points
	double src[4][2];
	double dest[4][2];

	// we set the warp coordinates
	// source coordinates as the dimensions of our window
	src[0][0] = 0;
	src[0][1] = 0;
	src[1][0] = _width;
	src[1][1] = 0;
	src[2][0] = _width;
	src[2][1] = _height;
	src[3][0] = 0;
	src[3][1] = _height;

	// corners are in 0.0 - 1.0 range
	// so we scale up so that they are at the render scale
	for(int i = 0; i < 4; i++){
		dest[i][0] = _warpPoints[i].x * _width;
		dest[i][1] = _warpPoints[i].y * _height;
	}

	// perform the warp calculation
	mapQuadToQuad(src, dest, _warpMatrix);

	// copy the values
	_glWarpMatrix[0] = _warpMatrix[0][0];
	_glWarpMatrix[1] = _warpMatrix[0][1];
	_glWarpMatrix[3] = _warpMatrix[0][2];

	_glWarpMatrix[4] = _warpMatrix[1][0];
	_glWarpMatrix[5] = _warpMatrix[1][1];
	_glWarpMatrix[7] = _warpMatrix[1][2];

	_glWarpMatrix[12] = _warpMatrix[2][0];
	_glWarpMatrix[13] = _warpMatrix[2][1];
	_glWarpMatrix[15] = _warpMatrix[2][2];
}
