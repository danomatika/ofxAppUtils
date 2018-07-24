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
#include "ofxTransformPanel.h"

//--------------------------------------------------------------
void ofxTransformPanel::setup(ofxTransformer *transformer, const std::string &name, const std::string &xmlFile) {
	
	this->transformer = transformer;
	
	// setup callbacks
	editWarp.addListener(this, &ofxTransformPanel::editWarpPressed);
	saveWarp.addListener(this, &ofxTransformPanel::saveWarpPressed);
	reset.addListener(this, &ofxTransformPanel::resetPressed);
	
	// setup values from transformer
	position.setup("position",
	   transformer->getPosition(),                                                      // val
	   ofPoint(-transformer->getRenderWidth(), -transformer->getRenderHeight(), -200),  // min
	   ofPoint(transformer->getRenderWidth(), transformer->getRenderHeight(), 1000));   // max
	translate.setup("translate", transformer->getTranslate());
	mirrorX.setup("mirrorX", transformer->getMirrorX());
	mirrorY.setup("mirrorY", transformer->getMirrorY());
	centering.setup("centering", transformer->getCentering());
	aspect.setup("aspect", transformer->getAspect());
	warp.setup("warp", transformer->getWarp());
	editWarp.setup("edit warp");
	saveWarp.setup("save warp");
	reset.setup("reset everything");
	
	// add widgets to panel
	panel.setup(name, ofToDataPath(xmlFile));
	panel.add(&position);
	panel.add(&translate);
	panel.add(&mirrorX);
	panel.add(&mirrorY);
	panel.add(&centering);
	panel.add(&aspect);
	panel.add(&warp);
	panel.add(&editWarp);
	panel.add(&saveWarp);
	panel.add(&reset);

	// load settings if they exist
	if(ofFile::doesFileExist(ofToDataPath(xmlFile))) {
		loadSettings(xmlFile);
	}
	transformer->loadWarpSettings();
	update();
}

//--------------------------------------------------------------
void ofxTransformPanel::update() {
	if(!transformer) {
		return;
	}
	transformer->setPosition(position->x, position->y, position->z);
	transformer->setTranslate(translate);
	transformer->setMirrorX(mirrorX);
	transformer->setMirrorY(mirrorY);
	transformer->setCentering(centering);
	transformer->setAspect(aspect);
	transformer->setWarp(warp);
}

//--------------------------------------------------------------
void ofxTransformPanel::draw() {
	if(!transformer || transformer->getEditWarp()) {
		return;
	}
	panel.draw();
}

//--------------------------------------------------------------
void ofxTransformPanel::loadSettings(const std::string &xmlFile) {
	panel.loadFromFile(ofToDataPath(xmlFile));
}

//--------------------------------------------------------------
void ofxTransformPanel::saveSettings(const std::string &xmlFile) {
	panel.saveToFile(ofToDataPath(xmlFile));
}

//--------------------------------------------------------------
void ofxTransformPanel::editWarpPressed() {
	if(!transformer) {
		return;
	}
	transformer->setEditWarp(true);
}

//--------------------------------------------------------------
void ofxTransformPanel::saveWarpPressed() {
	if(!transformer) {
		return;
	}
	transformer->saveWarpSettings();
}

//--------------------------------------------------------------
void ofxTransformPanel::resetPressed() {
	position = ofPoint(0, 0, 0);
	translate = false;
	mirrorX = false;
	mirrorY = false;
	centering = false;
	aspect = false;
	warp = false;
}
