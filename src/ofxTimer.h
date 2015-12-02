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

#include "ofUtils.h"

///	\class	Timer
///	\brief	a simple millis timer/alarm
class ofxTimer {
	public:
	
		ofxTimer() : alarmMS(0), alarmstamp(0) {set();}
		ofxTimer(const unsigned int alarmTime) {setAlarm(alarmTime);}
		virtual ~ofxTimer() {}
		
		/// copy constructor
		ofxTimer(const ofxTimer &from) {
			this->ofxTimer::operator=(from);
		}
		/// copy operator
		ofxTimer& operator=(const ofxTimer &from) {
			alarmMS = from.alarmMS;
			alarmstamp = from.alarmstamp;
			timestamp = from.timestamp;
			return *this;
		}
		
		/// set the timestamp to the current time
		inline void set() {
			timestamp = ofGetElapsedTimeMillis();
		}
		
		/// set the timestamp and alarm
		/// \param	alarmTime	how many ms in the future the alarm should go off
		inline void setAlarm(const unsigned int alarmTime) {
			alarmMS = alarmTime;
			timestamp = ofGetElapsedTimeMillis();
			alarmstamp = timestamp + alarmTime;
		}
		
		/// set the alarm using the existing alarm time
		inline void resetAlarm() {
			setAlarm(alarmMS);
		}
		
		/// has the alarm gone off?
		inline bool alarm() {
			return ofGetElapsedTimeMillis() >= alarmstamp;
		}
		
		/// returns how many ms have expired since the timestamp was last set
		inline unsigned int getDiff() {
			return ofGetElapsedTimeMillis() - timestamp;
		}
		
		/// returns the difference between 0 and 1 (normalized)
		inline float getDiffN() {
			return (float)getDiff()/(float)alarmMS;
		}
		
	protected:
	
		unsigned int alarmMS;    //< length of alarm
		unsigned int timestamp;  //< current timestamp
		unsigned int alarmstamp; //< future timestamp
};
