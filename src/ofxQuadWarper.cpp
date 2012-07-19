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
#include "ofxQuadWarper.h"

#ifndef OFX_APP_UTILS_NO_XML
	#include <ofxXmlSettings.h>
#endif
#include "matrix_funcs.h"

/// QUAD WARPER

//--------------------------------------------------------------
ofxQuadWarper::ofxQuadWarper() {
	reset();
}

//--------------------------------------------------------------
//
// projection warping from a modified Theo example on the OF forums:
// http://threeblindmiceandamonkey.com/?p=31
//
void ofxQuadWarper::apply(float width, float height) {

	// we set it to the default - 0 translation
	// and 1.0 scale for x y z and w
	for(int i = 0; i < 16; i++) {
		if(i % 5 != 0) _glWarpMatrix[i] = 0.0;
		else _glWarpMatrix[i] = 1.0;
	}

	// source and destination points
	double src[4][2];
	double dest[4][2];

	// we set the warp coordinates
	// source coordinates as the dimensions of our window
	src[0][0] = 0;
	src[0][1] = 0;
	src[1][0] = width;
	src[1][1] = 0;
	src[2][0] = width;
	src[2][1] = height;
	src[3][0] = 0;
	src[3][1] = height;

	// corners are in 0.0 - 1.0 range
	// so we scale up so that they are at the render scale
	for(int i = 0; i < 4; i++){
		dest[i][0] = _warpPoints[i].x * (float) width;
		dest[i][1] = _warpPoints[i].y * (float) height;
	}

	// perform the warp calculation
	mapQuadToQuad(src, dest, _warpMatrix);

	// copy the values
	_glWarpMatrix[0]	= _warpMatrix[0][0];
	_glWarpMatrix[1]	= _warpMatrix[0][1];
	_glWarpMatrix[3]	= _warpMatrix[0][2];

	_glWarpMatrix[4]	= _warpMatrix[1][0];
	_glWarpMatrix[5]	= _warpMatrix[1][1];
	_glWarpMatrix[7]	= _warpMatrix[1][2];

	_glWarpMatrix[12]	= _warpMatrix[2][0];
	_glWarpMatrix[13]	= _warpMatrix[2][1];
	_glWarpMatrix[15]	= _warpMatrix[2][2];

	// finally lets multiply our matrix
	glMultMatrixf(_glWarpMatrix);
}

//--------------------------------------------------------------
void ofxQuadWarper::setPoint(unsigned int index, ofVec2f point) {
	if(index > 3)
		return;
	_warpPoints[index] = point;
}

ofVec2f ofxQuadWarper::getPoint(unsigned int index) {
	if(index > 3)
		return ofVec2f();
	return _warpPoints[index];
}

//--------------------------------------------------------------
void ofxQuadWarper::reset() {
	_warpPoints[0].set(0.0, 0.0);
	_warpPoints[1].set(1.0, 0.0);
	_warpPoints[2].set(1.0, 1.0);
	_warpPoints[3].set(0.0, 1.0);
}

//--------------------------------------------------------------
#ifndef OFX_APP_UTILS_NO_XML

bool ofxQuadWarper::loadSettings(const string xmlFile) {
	
	ofxXmlSettings xml;
	if(!xml.loadFile(xmlFile))
		return false;
		
	_warpPoints[0].x	= xml.getValue("quad:upperLeft:x", 0.0);
	_warpPoints[0].y	= xml.getValue("quad:upperLeft:y", 0.0);
	
	_warpPoints[1].x	= xml.getValue("quad:upperRight:x", 1.0);
	_warpPoints[1].y	= xml.getValue("quad:upperRight:y", 0.0);
	
	_warpPoints[2].x	= xml.getValue("quad:lowerRight:x", 1.0);
	_warpPoints[2].y	= xml.getValue("quad:lowerRight:y", 1.0);
	
	_warpPoints[3].x	= xml.getValue("quad:lowerLeft:x", 0.0);
	_warpPoints[3].y	= xml.getValue("quad:lowerLeft:y", 1.0);
	
	return true;
}

void ofxQuadWarper::saveSettings(const string xmlFile) {

	ofxXmlSettings xml;
	
	xml.addTag("quad");
	xml.pushTag("quad");
	
	xml.addTag("upperLeft");
	xml.pushTag("upperLeft");
	xml.addValue("x", _warpPoints[0].x);
	xml.addValue("y", _warpPoints[0].y);
	xml.popTag();
	
	xml.addTag("upperRight");
	xml.pushTag("upperRight");
	xml.addValue("x", _warpPoints[1].x);
	xml.addValue("y", _warpPoints[1].y);
	xml.popTag();
	
	xml.addTag("lowerRight");
	xml.pushTag("lowerRight");
	xml.addValue("x", _warpPoints[2].x);
	xml.addValue("y", _warpPoints[2].y);
	xml.popTag();
	
	xml.addTag("lowerLeft");
	xml.pushTag("lowerLeft");
	xml.addValue("x", _warpPoints[3].x);
	xml.addValue("y", _warpPoints[3].y);
	xml.popTag();
	
	xml.saveFile(xmlFile);
}

#endif
