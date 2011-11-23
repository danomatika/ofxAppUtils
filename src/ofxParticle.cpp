#include "ofxParticle.h"

unsigned int ofxParticle::_frameTimeout = 500;

ofxParticle::ofxParticle() : ofRectangle(), bAlive(false), lifespan(5000), age(0) {
    reset();
}

ofxParticle::ofxParticle(float x, float y, float w, float h) :
    ofRectangle(x, y, w, h), bAlive(false), lifespan(0), age(0) {
    reset();
}

ofxParticle::~ofxParticle() {}

ofxParticle::ofxParticle(const ofxParticle& from) {
    this->ofxParticle::operator=(from);
}

ofxParticle& ofxParticle::operator=(const ofxParticle& from) {
    (ofRectangle&) *this = from;    // call ofRectangle copy operator
	bAlive = from.bAlive;
	lifespan = from.lifespan;
	age = from.age;
    return *this;
}

void ofxParticle::updateAge() {
    if(!bAlive)
        return;

    // ignore if time from last frame was too long
    if(lifeTimer.getDiff() < _frameTimeout) {
        // increase age by diff in time from last frame
        age += lifeTimer.getDiff();

        // check if too old
        if(age >= lifespan)
            kill();
    }
    lifeTimer.set();
}

double ofxParticle::getAge() {
    if(lifespan == 0)
        return 0;
    return age/lifespan;
}

double ofxParticle::getRemaingLife() {
    return ((getAge()*-1.0) + 1.0);
}
