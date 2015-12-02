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

#include "ofGraphics.h"
#include "ofVectorMath.h"

/// \class ofxBitmapStream
/// \brief a stream interface to ofDrawBitmapStream
///
/// ofxBitmapStream accepts variables via the ostream operator <<, builds a string,
/// and logs it when the stream is finished (via the destructor). All the stream
/// controls work (endl, flush, hex, etc).
///
/// Usage: ofxBitmapString(10, 10) << "a string" << 100 << 20.234f;
///
/// class idea from:
///     http://www.gamedev.net/community/forums/topic.asp?topic_id=525405&whichpage=1&#3406418
/// how to catch std::endl (which is actually a func pointer):
///     http://yvan.seth.id.au/Entries/Technology/Code/std__endl.html
///
class ofxBitmapString {
	public:

		ofxBitmapString(const ofPoint &p) {
			pos = p;
		}
		
		ofxBitmapString(float x, float y, float z=0.0f) {
			pos.set(x, y, z);
		}
		
		/// does the actual printing on when the ostream is done
		~ofxBitmapString() {
			ofDrawBitmapString(message.str(), pos.x, pos.y, pos.z);
		}
		
		/// catch the << ostream with a template class to read any type of data
		template <class T> 
		ofxBitmapString& operator<<(const T &value) {
			message << value;
			return *this;
		}

		/// catch the << ostream function pointers such as std::endl and std::hex
		ofxBitmapString& operator<<(std::ostream& (*func)(std::ostream &)) {
			func(message);
			return *this;
		}
		
	private:
	
		ofPoint pos;                //< temp position
		std::ostringstream message; //< temp buffer
		
		ofxBitmapString(ofxBitmapString const&) {} // not defined, not copyable
		ofxBitmapString& operator=(ofxBitmapString& from) {return *this;} // not defined, not assignable
};
