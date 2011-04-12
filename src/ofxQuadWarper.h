#pragma once

#include <ofMain.h>

/**
    \class  QuadWarper
    \brief  a gl matrix transform quad warper
**/
class ofxQuadWarper {

	public:
	
		ofxQuadWarper() {}
		virtual ~ofxQuadWarper() {}
		
		// apply the gl matrix transform
		void apply(float width, float height);
		
		/// set/get the warp points
		/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
		void setPoint(unsigned int index, ofVec2f point);
		ofVec2f getPoint(unsigned int index);
		
		// reset the quad
		void reset();
		
		// load/save the quad coords from/to an xml file
		bool loadSettings(const string xmlFile="quadWarper.xml");
		void saveSettings(const string xmlFile="quadWarper.xml");
		
	private:
		
		// projection warp points
		ofVec2f	_warpPoints[4];
		
		// projection warping matrices
		double _warpMatrix[3][3];
        GLfloat _glWarpMatrix[16];
};
