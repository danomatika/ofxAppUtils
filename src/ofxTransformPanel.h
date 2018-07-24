/*
 * Copyright (c) 2015 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include "ofxGui.h"
#include "ofxTransformer.h"

/// \class TransformPanel
/// \brief ofxGui panel for transform control
///
/// note: requires including ofxGui in your project
class ofxTransformPanel {

	public:
	
		ofxTransformPanel() : transformer(NULL) {}
	
		ofxPanel panel; //< gui panel
	
		ofxPointSlider position; //< 3d transform origin
		ofxToggle translate;     //< translate the origin?
		ofxToggle mirrorX;       //< mirror horizontally?
		ofxToggle mirrorY;       //< mirror vertically?
		ofxToggle aspect;        //< keep aspect ratio?
		ofxToggle centering;     //< center in middle of the screen?
		ofxToggle warp;          //< enable the quad warper?
		ofxButton editWarp;      //< edit quad warper
		ofxButton saveWarp;      //< save quad warper
		ofxButton reset;         //< reset everything
	
		/// set the transformer to control, set optional panel name and settings file name,
		/// loads settings file if found
		void setup(ofxTransformer *transformer, const std::string &name="Transform",
		           const std::string &xmlFile="transformSettings.xml");
	
		/// update the transformer with the current panel settings
		void update();
	
		/// draw the panel
		void draw();
	
		/// load and save the control panel settings
		void loadSettings(const std::string &xmlFile="transformSettings.xml");
		void saveSettings(const std::string &xmlFile="transformSettings.xml");
	
		/// button callbacks
		void editWarpPressed();
		void saveWarpPressed();
		void resetPressed();
	
	protected:
	
		ofxTransformer *transformer; //< current transformer to control
};
