#include "ofxTransformPanel.h"

void ofxTransformPanel::setup(ofxTransformer *transformer, const string &name, const string &xmlFile) {
	
	this->transformer = transformer;
	
	// setup callbacks
	editWarp.addListener(this, &ofxTransformPanel::editWarpPressed);
	saveWarp.addListener(this, &ofxTransformPanel::saveWarpPressed);
	
	// setup values from transformer
	position.setup("position",
	   transformer->getOrigin(),                                                        // val
	   ofPoint(-transformer->getRenderWidth(), -transformer->getRenderHeight(), -200),  // min
	   ofPoint(transformer->getRenderWidth(), transformer->getRenderHeight(), 1000));   // max
	translate.setup("translate", transformer->getOriginTranslate());
	mirrorX.setup("mirrorX", transformer->getMirrorX());
	mirrorY.setup("mirrorY", transformer->getMirrorY());
	centering.setup("centering", transformer->getCentering());
	aspect.setup("aspect", transformer->getAspect());
	warp.setup("warp", transformer->getWarp());
	editWarp.setup("edit warp");
	saveWarp.setup("save warp");
	
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

	// load settings if they exist
	if(ofFile::doesFileExist(ofToDataPath(xmlFile))) {
		loadSettings(xmlFile);
	}
	transformer->loadWarpSettings();
}

void ofxTransformPanel::update() {
	transformer->setOrigin(position->x, position->y, position->z);
	transformer->setOriginTranslate(translate);
	transformer->setMirrorX(mirrorX);
	transformer->setMirrorY(mirrorY);
	transformer->setCentering(centering);
	transformer->setAspect(aspect);
	transformer->setWarp(warp);
}

void ofxTransformPanel::draw() {
	//if(transformer->isDebug() && !transformer->getEditWarp()) {
		panel.draw();
	//}
}

void ofxTransformPanel::loadSettings(const string &xmlFile) {
	panel.loadFromFile(ofToDataPath(xmlFile));
}

void ofxTransformPanel::saveSettings(const string &xmlFile) {
	panel.saveToFile(ofToDataPath(xmlFile));
}

void ofxTransformPanel::editWarpPressed() {
	//transformer->setEditWarp(true);
	ofLog() << "edit pressed";
}

void ofxTransformPanel::saveWarpPressed() {
	transformer->saveWarpSettings();
	ofLog() << "save pressed";
}
