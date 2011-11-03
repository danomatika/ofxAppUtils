#include "ofxApp.h"

#include "ofxSceneManager.h"

/// APP

//--------------------------------------------------------------
ofxApp::ofxApp() : ofBaseApp(), _currentWarpPoint(-1) {

	bDebug = false;
	
	_bEditingWarpPoints = false;
	_bTransformControls = false;
	
	_sceneManager = NULL;
}

//--------------------------------------------------------------

void ofxApp::setMirror(bool mirrorX, bool mirrorY) {
	setMirrorX(mirrorX);
	setMirrorY(mirrorY);
}

void ofxApp::setMirrorX(bool mirrorX) {
	_bMirrorX = mirrorX;
	if(_bTransformControls)
		controlPanel.setValueB("transformMirrorX", _bMirrorX);
}

void ofxApp::setMirrorY(bool mirrorY) {
	_bMirrorY = mirrorY;
	if(_bTransformControls)
		controlPanel.setValueB("transformMirrorY", _bMirrorY);
}

void ofxApp::setOrigin(float x, float y, float z)	{
	_origin.set(x, y, z);
	if(_bTransformControls) {
		controlPanel.setValueF("transformPos", x, 0);
		controlPanel.setValueF("transformPos", y, 1);
		controlPanel.setValueF("transformZ", x);
	}
}

void ofxApp::setWarp(bool warp) {
	_bWarp = warp;
	if(_bTransformControls)
		controlPanel.setValueB("transformEnableQuadWarper", _bWarp);
}

//--------------------------------------------------------------
void ofxApp::addTransformControls(int panelNum, int panelCol) {
	if(_bTransformControls)
		return;
	if(panelNum < 0) {
		controlPanel.addPanel("Transformer", 1, false);
		controlPanel.setWhichPanel(controlPanel.panels.size()-1);
	}
	else {
		controlPanel.setWhichPanel(panelNum);
	}
	controlPanel.setWhichColumn(panelCol);
	controlPanel.addSlider2D("position", "transformPosition", 0, 0,
							 -getRenderWidth(), getRenderWidth(),
							 -getRenderHeight(), getRenderHeight(), false);
	controlPanel.addSlider("z", "transformZ", 0, -1000, 200, false);
	controlPanel.addToggle("mirror x", "transformMirrorX", false);
	controlPanel.addToggle("mirror y", "transformMirrorY", false);
	controlPanel.addToggle("enable quad warper", "transformEnableQuadWarper", true);
	controlPanel.addToggle("edit quad warper", "transformEditQuadWarper", false);
	controlPanel.addToggle("save quad warper", "transformSaveQuadWarper", false);
	_bTransformControls = true;
}

//--------------------------------------------------------------
void ofxApp::setSceneManager(ofxSceneManager* manager) {
	if(manager == NULL) {
		ofLogWarning("ofxApp") << "cannot add NULL scene manager";
		return;
	}
	_sceneManager = manager;
}

//--------------------------------------------------------------
ofxSceneManager* ofxApp::getSceneManager() {
	return _sceneManager;
}
		
//--------------------------------------------------------------
void ofxApp::clearSceneManager() {
	_sceneManager = NULL;
}

/// RUNNER APP

//--------------------------------------------------------------
void ofxRunnerApp::setup() {
	app->controlPanel.setup("App Controls", 1, 0, 275, app->getRenderHeight()-40);
	app->setup();
}

//--------------------------------------------------------------
void ofxRunnerApp::update() {

    app->mouseX = mouseX;
    app->mouseY = mouseY;

    ofxControlPanel& controlPanel = app->controlPanel;

	if(app->_bTransformControls) {
    
		// grab control panel variables
		app->_origin.set(controlPanel.getValueF("transformPosition", 0),	// x
				   controlPanel.getValueF("transformPosition", 1),          // y
				   controlPanel.getValueF("transformZ"));                   // z
		
		// mirror x/y?
		app->_bMirrorX = controlPanel.getValueB("transformMirrorX");
		app->_bMirrorY = controlPanel.getValueB("transformMirrorY");
		
		// enable quad warper?
		app->_bWarp = controlPanel.getValueB("transformEnableQuadWarper");
		
		// edit quad warper?
		if(controlPanel.getValueB("transformEditQuadWarper")) {
			app->setEditWarp(true);
			controlPanel.setValueB("transformEditQuadWarper", false);
		}
		
		// save quad warper?
		if(controlPanel.getValueB("transformSaveQuadWarper")) {
			app->saveWarpSettings();
			controlPanel.setValueB("transformSaveQuadWarper", false);
		}
	}

	controlPanel.update();
	if(app->_sceneManager)
		app->_sceneManager->update();
	app->update();
}

//--------------------------------------------------------------
void ofxRunnerApp::draw() {

	ofPushMatrix();

		if(app->_bScale) {
			app->applyRenderScale();
		}

		if(app->_bTranslate) {
			app->applyOriginTranslate();
		}
		
		if(app->_bWarp) {
			app->applyWarp();
			ofPushMatrix();
		}
		
		if(app->_bMirrorX) {
			app->applyMirrorX();
		}
		
		if(app->_bMirrorY) {
			app->applyMirrorY();
		}
		
		ofPushMatrix();
		if(app->_sceneManager)
			app->_sceneManager->draw();
		app->draw();	// do the user callback
		ofPopMatrix();
		
		if(app->_bWarp) {
			ofPopMatrix();
			
			if(app->_bEditingWarpPoints && app->bDebug) {
				// draw projection warping bounding box
				ofNoFill();
				ofSetRectMode(OF_RECTMODE_CORNER);
				ofSetHexColor(0x00FF00);
				ofRect(0.35, 0.35, app->_renderWidth-1.35, app->_renderHeight-1.35);
				ofSetRectMode(OF_RECTMODE_CORNER);
				ofFill();
			}
		}
	
	ofPopMatrix();
	
	if(app->bDebug) {
        stringstream text;
		if(app->_bEditingWarpPoints) {
		
			ofSetHexColor(0x00FF00);
			text << "Quad Warper Edit Mode" << endl
                 << "Drag from the corners of the screen" << endl
                 << "Click center rectangle to exit";
            ofDrawBitmapString(text.str(), 28, 28);
            text.str("");
				
			// draw center exit box
			ofNoFill();
			ofSetRectMode(OF_RECTMODE_CENTER);
			ofRect(ofGetWidth()/2, ofGetHeight()/2, 100, 100);
			ofSetRectMode(OF_RECTMODE_CORNER);
			ofFill();
		}
		else {
			ofFill();
			ofSetRectMode(OF_RECTMODE_CORNER);
			app->controlPanel.draw();
		}
		ofSetColor(255);
        text << "fps: " << ofGetFrameRate();
		ofDrawBitmapString(text.str(), ofGetWidth()-100, ofGetHeight()-14);
	}
}

//--------------------------------------------------------------
void ofxRunnerApp::exit() {
	app->exit();
	if(app->_sceneManager)
		app->_sceneManager->clear();
}

//--------------------------------------------------------------
void ofxRunnerApp::windowResized(int w, int h) {
    if(app->_sceneManager)
		app->_sceneManager->windowResized(w, h);
    app->windowResized(w, h);
}

//--------------------------------------------------------------
void ofxRunnerApp::keyPressed(int key) {
	if(app->_sceneManager)
		app->_sceneManager->keyPressed(key);
	app->keyPressed(key);
}

//--------------------------------------------------------------
void ofxRunnerApp::keyReleased(int key) {
    if(app->_sceneManager)
		app->_sceneManager->keyReleased(key);
    app->keyReleased(key);
}

//--------------------------------------------------------------
void ofxRunnerApp::mouseMoved(int x, int y) {
	if(app->_sceneManager)
		app->_sceneManager->mouseMoved(x, y);
	app->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofxRunnerApp::mouseDragged(int x, int y, int button) {
	if(app->_sceneManager)
		app->_sceneManager->mouseDragged(x, y, button);
	app->mouseDragged(x, y, button);
	
	if(app->bDebug) {
		if(app->_bWarp && app->_bEditingWarpPoints) {
			if(app->_currentWarpPoint >= 0) {
				app->_quadWarper.setPoint(app->_currentWarpPoint,
					ofVec2f((float)x/ofGetWidth(), (float)y/ofGetHeight()));
			}
		} else {
			app->controlPanel.mouseDragged(x, y, button);
		}
	}
}

//--------------------------------------------------------------
void ofxRunnerApp::mousePressed(int x, int y, int button) {
	if(app->_sceneManager)
		app->_sceneManager->mousePressed(x, y, button);
	app->mousePressed(x, y, button);
	
	if(app->bDebug) {
		if(app->_bWarp && app->_bEditingWarpPoints) {
		
			// check if middle of the screen was pressed to exit edit mode
			if((x > ofGetWidth()/2  - 50 && (x < ofGetWidth()/2  + 50) &&
			   (y > ofGetHeight()/2 - 50 && (y < ofGetHeight()/2 + 50))))
			{
				app->_bEditingWarpPoints = false;
				return;
			}
			
			// check if the screen corners are being clicked
			float smallestDist = 1.0;
			app->_currentWarpPoint = -1;
			for(int i = 0; i < 4; i++) {
				float distx = app->_quadWarper.getPoint(i).x - (float) x/ofGetWidth();
				float disty = app->_quadWarper.getPoint(i).y - (float) y/ofGetHeight();
				float dist  = sqrt(distx * distx + disty * disty);

				if(dist < smallestDist && dist < 0.1) {
					app->_currentWarpPoint = i;
					smallestDist = dist;
				}
			}	
		} else {
			app->controlPanel.mousePressed(x, y, button);
		}
	}
}

//--------------------------------------------------------------
void ofxRunnerApp::mouseReleased() {
    if(app->_sceneManager)
		app->_sceneManager->mouseReleased();
    app->mouseReleased();
}

//--------------------------------------------------------------
void ofxRunnerApp::mouseReleased(int x, int y, int button) {
	if(app->_sceneManager)
		app->_sceneManager->mouseReleased(x, y, button);
	app->mouseReleased(x, y, button);
	
	if(app->bDebug) {
		if(!app->_bEditingWarpPoints) {
			app->controlPanel.mouseReleased();
		}
	}
	app->_currentWarpPoint = -1;
}

//--------------------------------------------------------------
void ofxRunnerApp::dragEvent(ofDragInfo dragInfo) {
    if(app->_sceneManager)
		app->_sceneManager->dragEvent(dragInfo);
    app->dragEvent(dragInfo);
}

//--------------------------------------------------------------
void ofxRunnerApp::gotMessage(ofMessage msg){
    if(app->_sceneManager)
		app->_sceneManager->gotMessage(msg);
    app->gotMessage(msg);
}

//--------------------------------------------------------------
void ofxRunnerApp::audioIn(float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
    if(app->_sceneManager)
		app->_sceneManager->audioIn(input, bufferSize, nChannels, deviceID, tickCount);
    app->audioIn(input, bufferSize, nChannels, deviceID, tickCount);
}

void ofxRunnerApp::audioIn(float * input, int bufferSize, int nChannel ) {
    if(app->_sceneManager)
		app->_sceneManager->audioIn(input, bufferSize, nChannel);
    app->audioIn(input, bufferSize, nChannel);
}
void ofxRunnerApp::audioReceived(float * input, int bufferSize, int nChannels) {
    if(app->_sceneManager)
		app->_sceneManager->audioIn(input, bufferSize, nChannels);
    app->audioIn(input, bufferSize, nChannels);
}
        
//--------------------------------------------------------------
void ofxRunnerApp::audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
    if(app->_sceneManager)
		app->_sceneManager->audioOut(output, bufferSize, nChannels, deviceID, tickCount);
    app->audioOut(output, bufferSize, nChannels, deviceID, tickCount);
}

void ofxRunnerApp::audioOut(float * output, int bufferSize, int nChannels) {
    if(app->_sceneManager)
		app->_sceneManager->audioOut(output, bufferSize, nChannels);
    app->audioOut(output, bufferSize, nChannels);
}

void ofxRunnerApp::audioRequested(float * output, int bufferSize, int nChannels) {
    if(app->_sceneManager)
		app->_sceneManager->audioOut(output, bufferSize, nChannels);
    app->audioOut(output, bufferSize, nChannels);
}
