#include "testApp.h"

#include "scenes/scenes.h"

//--------------------------------------------------------------
void testApp::setup(){

	// setup of
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	// setup app
	setRenderSize(800, 600, true, true);
	setOriginTranslate(true);
	
	// setup control panel
	controlPanel.setup("App Controls", 1, 0, 275, getRenderHeight()-40);
	controlPanel.addPanel("Projection Mapping", 1, false);
	
	//--------- PANEL 1
	controlPanel.setWhichPanel(1);
	controlPanel.setWhichColumn(0);
	controlPanel.addSlider2D("position", "position", 0, 0,
							 -getRenderWidth(), getRenderWidth(),
							 -getRenderHeight(), getRenderHeight(), false);
	controlPanel.addSlider("z", "z", 0, -1000, 200, false);
	controlPanel.addToggle("mirror x", "mirrorX", false);
	controlPanel.addToggle("mirror y", "mirrorY", false);
	controlPanel.addToggle("enable quad warper", "enableQuadWarper", true);
	controlPanel.addToggle("edit quad warper", "editQuadWarper", false);
	controlPanel.addToggle("save quad warper", "saveQuadWarper", false);
	
	// load scenes
	sceneManager.add(new ParticleScene(*this));
	sceneManager.add(new LineScene(*this));
	sceneManager.setup(true);
	ofxLog::setTopicLogLevel("SceneManager", OF_LOG_VERBOSE);
	
	// start with a specific scene
	sceneManager.gotoScene("Lines", true);
}

//--------------------------------------------------------------
void testApp::updateApp(){
	
	// update scene
	sceneManager.update();
	
	// grab control panel variables
	setOrigin(controlPanel.getValueF("position", 0),	// x
			  controlPanel.getValueF("position", 1),	// y
			  controlPanel.getValueF("z"));				// z
	
	// mirror x/y?
	setMirrorX(controlPanel.getValueB("mirrorX"));
	setMirrorY(controlPanel.getValueB("mirrorY"));
	
	// enable quad warper?
	setWarp(controlPanel.getValueB("enableQuadWarper"));
	
	// edit quad warper?
	if(controlPanel.getValueB("editQuadWarper")) {
		setEditWarp(true);
		controlPanel.setValueB("editQuadWarper", false);
	}
	if(controlPanel.getValueB("saveQuadWarper")) {
		saveWarpSettings();
		controlPanel.setValueB("saveQuadWarper", false);
	}

}

//--------------------------------------------------------------
void testApp::drawApp(){

	ofBackground(0, 0, 0);

	// show the render area edges
	if(bDebug) {
		ofNoFill();
		ofRect(1, 1, getRenderWidth()-2, getRenderHeight()-2);
		ofFill();
	}

	// draw scene
	ofEnableAlphaBlending();
	sceneManager.draw();
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::exit() {
	sceneManager.clear();
}

//--------------------------------------------------------------
void testApp::keyPressedApp(int key) {
	sceneManager.keyPressed(key);
	
	switch (key) {
	
		case 'd':
			bDebug = !bDebug;
			break;
			
		case 'm':
			setMirrorX(!getMirrorX());
			controlPanel.setValueB("mirrorX", getMirrorX());
			break;
			
		case 'n':
			setMirrorY(!getMirrorY());
			controlPanel.setValueB("mirrorY", getMirrorY());
			break;
			
		case 'q':
			setWarp(!getWarp());
			controlPanel.setValueB("enableQuadWarper", getWarp());
			break;
			
		case 'f':
			ofToggleFullscreen();
			break;
	
		case OF_KEY_LEFT:
			sceneManager.prevScene();
			break;
			
		case OF_KEY_RIGHT:
			sceneManager.nextScene();
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMovedApp(int x, int y) {
	sceneManager.mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDraggedApp(int x, int y, int button){
	sceneManager.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressedApp(int x, int y, int button){
	sceneManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleasedApp(int x, int y, int button){
	sceneManager.mouseReleased(x, y, button);
}