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

#include "ofPoint.h"

///	\class  QuadWarper
///	\brief  a gl matrix transform quad warper
class ofxQuadWarper {
	public:
	
		ofxQuadWarper();
	
		/// set width and height of the screen/render area
		void setSize(float width, float height);
	
		/// apply the gl matrix transform, make sure to set the render size first
		void push();
	
		/// pop the gl matrix transform
		void pop();
	
		/// has the gl matrix trasnform been pushed?
		bool isPushed();
	
		/// draw the quad warper corner points in screen space
		void drawPoints();
	
		/// draw the quad warper corner points in a given width & height
		void drawPoints(float width, float height);
		
		/// set/get the warp points
		/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
		void setPoint(unsigned int index, float x, float y);
		void setPoint(unsigned int index, const ofVec2f &point);
		const ofVec2f& getPoint(unsigned int index); //< coords are normalized 0-1 based on render size
		
		/// reset the quad to screen size
		void reset();
		
		/// load/save the quad coords from/to an xml file
		bool loadSettings(const std::string &xmlFile="quadWarper.xml");
		void saveSettings(const std::string &xmlFile="quadWarper.xml");
		
	protected:
	
		/// recompute warp matrix based on render size & points
		void updateMatrix();
	
		float _width;  //< projection width
		float _height; //< projection height
	
		ofVec2f	_warpPoints[4];    //< projection warp points
		double _warpMatrix[3][3];  //< interim projection warping matrix
		GLfloat _glWarpMatrix[16]; //< projection warping matrix
	
		bool _bPushed; //< was the warp pushed?
};
