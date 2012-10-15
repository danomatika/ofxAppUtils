/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include "ofMain.h"

#include "ofxAppUtils.h"

// a simple gui button
class Button {
	
    public:

		// what kind of button?
		enum Type {
			LEFT,
			RIGHT
		} type;

		Button() {
			type = RIGHT;
			isInside = false;
			offset = 2;
		}

		void setType(Type t) {
			type = t;
		}

		void set(int x, int y, int w, int h) {
			rect.set(x, y, w, h);
			offset = w*0.10;
		}

		bool inside(int x, int y) {
			isInside = rect.inside(x, y);
			return isInside;
		}

        void draw() {
			
			// background
			if(isInside) {
				ofFill();
				ofSetColor(127);
				ofRect(rect);
			}
			
			// outline
			ofNoFill();
			ofSetLineWidth(1.0);
			ofSetColor(255);
			ofRect(rect);
			
			// arrow
			ofFill();
			if(type == LEFT) {
				ofLine(rect.x+offset, rect.y+rect.height/2, rect.x+rect.width-offset, rect.y+rect.height/2);
				ofLine(rect.x+offset, rect.y+rect.height/2, rect.x+rect.width/2, rect.y+offset);
				ofLine(rect.x+offset, rect.y+rect.height/2, rect.x+rect.width/2, rect.y+rect.height-offset);
			}
			if(type == RIGHT) {
				ofLine(rect.x+rect.width-offset, rect.y+rect.height/2, rect.x+offset, rect.y+rect.height/2);
				ofLine(rect.x+rect.width-offset, rect.y+rect.height/2, rect.x+rect.width/2, rect.y+offset);
				ofLine(rect.x+rect.width-offset, rect.y+rect.height/2, rect.x+rect.width/2, rect.y+rect.height-offset);
			}
		}

		ofRectangle rect;
		bool isInside;
		float offset;
};
