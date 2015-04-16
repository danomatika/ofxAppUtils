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

#include "ofVectorMath.h"

///	\class  QuadWarper
///	\brief  a gl matrix transform quad warper
class ofxQuadWarper {
	public:
	
		ofxQuadWarper();
		virtual ~ofxQuadWarper() {}
		
		/// apply the gl matrix transform
		/// width and height is the size of the screen/render area
		void apply(float width, float height);
		
		/// set/get the warp points
		/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
		void setPoint(unsigned int index, const ofVec2f &point);
		ofVec2f getPoint(unsigned int index);
		
		/// reset the quad to screen size
		void reset();
		
		/// load/save the quad coords from/to an xml file
		bool loadSettings(const string &xmlFile="quadWarper.xml");
		void saveSettings(const string &xmlFile="quadWarper.xml");
		
	private:
		
		// projection warp points
		ofVec2f	_warpPoints[4];
		
		// projection warping matrices
		double _warpMatrix[3][3];
		GLfloat _glWarpMatrix[16];
};
