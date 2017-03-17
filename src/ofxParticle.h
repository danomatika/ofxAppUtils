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
#pragma once

#include "ofxTimer.h"
#include "ofPoint.h"
#include "ofRectangle.h"

///	\class  Particle
///	\brief  a particle with a lifespan, inherit this to add vel, acc, & behavior
class ofxParticle : public ofRectangle {
	public:

		ofxParticle();
		ofxParticle(float x, float y, float w, float h);
		ofxParticle(ofPoint pos, float w, float h);
		ofxParticle(ofRectangle rect);
		virtual ~ofxParticle();

		/// copy constructor
		ofxParticle(const ofxParticle &from);

		/// copy operator
		ofxParticle& operator=(const ofxParticle &from);


	/// \section Main

		/// update the particles age, ignores time between frames longer then
		/// the frame timeout
		///
		/// note: make sure to call this in your update() or the age will not
		///       change
		///
		void updateAge();

		/// do the update calculations
		virtual void update() = 0;

		/// draw
		virtual void draw() = 0;


	/// \section Status

		/// bring this particle to life
		void reset() {bAlive = true; lifeTimer.set();}

		/// get the age in ms
		double getAge() {return age;}

		/// get the age normalized between 0 and 1: 0 is birth, 1 is death
		double getAgeN();

		/// get the remaining life in ms
		double getRemainingLife() {return lifespan - age;}

		/// get the remaing life normalized between 0 and 1, 1 being death
		double getRemainingLifeN();

		/// is this particle alive?
		inline bool isAlive() {return bAlive;}

		/// kill the particle
		inline void kill()  {bAlive = false; age = 0;}


	/// \section Util

		/// get/set the particles lifespan in ms
		inline unsigned int getLifespan()           {return lifespan;}
		inline void setLifespan(unsigned int span)  {lifespan = span;}

		/// set how long to wait between frames (ms) before throwing out a time
		/// calculation, default is 500 ms
		static void setFrameTimeout(unsigned int timeout) {_frameTimeout = timeout;}
		static unsigned int getFrameTimeout() {return _frameTimeout;}

	protected:

		bool bAlive;        //< is this particle alive?

		double lifespan;    //< how long this particle should live in ms
		double age;         //< how old the particle is

		ofxTimer lifeTimer; //< used to time the age between frames

	private:

		static unsigned int _frameTimeout; //< how long to wait between frames
};
