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
#include "ofxParticle.h"

unsigned int ofxParticle::_frameTimeout = 500;

//--------------------------------------------------------------
ofxParticle::ofxParticle() : ofRectangle(), bAlive(false), lifespan(0), age(0) {
	reset();
}

//--------------------------------------------------------------
ofxParticle::ofxParticle(float x, float y, float w, float h) :
	ofRectangle(x, y, w, h), bAlive(false), lifespan(0), age(0) {
	reset();
}

//--------------------------------------------------------------
ofxParticle::ofxParticle(ofPoint pos, float w, float h) : 
	ofRectangle(pos.x, pos.y, w, h), bAlive(false), lifespan(0), age(0) {
	reset();
}
		
//--------------------------------------------------------------
ofxParticle::ofxParticle(ofRectangle rect) : 
	ofRectangle(rect), bAlive(false), lifespan(0), age(0) {
	reset();
}

//--------------------------------------------------------------
ofxParticle::~ofxParticle() {}

//--------------------------------------------------------------
ofxParticle::ofxParticle(const ofxParticle &from) {
	this->ofxParticle::operator=(from);
}

//--------------------------------------------------------------
ofxParticle& ofxParticle::operator=(const ofxParticle &from) {
	(ofRectangle &)*this = from;    // call ofRectangle copy operator
	bAlive = from.bAlive;
	lifespan = from.lifespan;
	age = from.age;
	return *this;
}

//--------------------------------------------------------------
void ofxParticle::updateAge() {
	if(!bAlive) {
		return;
	}

	// ignore if time from last frame was too long
	if(lifeTimer.getDiff() < _frameTimeout) {
		// increase age by diff in time from last frame
		age += lifeTimer.getDiff();

		// check if too old
		if(age >= lifespan) {
			kill();
		}
	}
	lifeTimer.set();
}

//--------------------------------------------------------------
double ofxParticle::getAgeN() {
	if(lifespan == 0) {
		return 0;
	}
	return age/lifespan;
}

//--------------------------------------------------------------
double ofxParticle::getRemainingLifeN() {
	return ((getAgeN()*-1.0) + 1.0);
}
