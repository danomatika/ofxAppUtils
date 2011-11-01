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

//--------------------------------------------------------------
void ofxApp::setup() {
	_setupControlPanel();
	setupApp();
}

//--------------------------------------------------------------
void ofxApp::exit() {
	exitApp();
	if(_sceneManager)
		_sceneManager->clear();
}

//--------------------------------------------------------------
void ofxApp::update() {

	if(_bTransformControls) {
		// grab control panel variables
		_origin.set(controlPanel.getValueF("transformPosition", 0),	// x
				   controlPanel.getValueF("transformPosition", 1),	// y
				   controlPanel.getValueF("transformZ"));			// z
		
		// mirror x/y?
		_bMirrorX = controlPanel.getValueB("transformMirrorX");
		_bMirrorY = controlPanel.getValueB("transformMirrorY");
		
		// enable quad warper?
		_bWarp = controlPanel.getValueB("transformEnableQuadWarper");
		
		// edit quad warper?
		if(controlPanel.getValueB("transformEditQuadWarper")) {
			setEditWarp(true);
			controlPanel.setValueB("transformEditQuadWarper", false);
		}
		
		// save quad warper?
		if(controlPanel.getValueB("transformSaveQuadWarper")) {
			saveWarpSettings();
			controlPanel.setValueB("transformSaveQuadWarper", false);
		}
	}

	controlPanel.update();
	if(_sceneManager)
		_sceneManager->update();
	updateApp();
}

//--------------------------------------------------------------
void ofxApp::draw() {

	ofPushMatrix();

		if(_bScale) {
			applyRenderScale();
		}

		if(_bTranslate) {
			applyOriginTranslate();
		}
		
		if(_bWarp) {
			applyWarp();
			ofPushMatrix();
		}
		
		if(_bMirrorX) {
			applyMirrorX();
		}
		
		if(_bMirrorY) {
			applyMirrorY();
		}
		
		ofPushMatrix();
		if(_sceneManager)
			_sceneManager->draw();
		drawApp();	// do the user callback
		ofPopMatrix();
		
		if(_bWarp) {
			ofPopMatrix();
			
			if(_bEditingWarpPoints && bDebug) {
				// draw projection warping bounding box
				ofNoFill();
				ofSetRectMode(OF_RECTMODE_CORNER);
				ofSetHexColor(0x00FF00);
				ofRect(0.35, 0.35, _renderWidth-1.35, _renderHeight-1.35);
				ofSetRectMode(OF_RECTMODE_CORNER);
				ofFill();
			}
		}
	
	ofPopMatrix();
	
	if(bDebug) {
        stringstream text;
		if(_bEditingWarpPoints) {
		
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
			controlPanel.draw();
		}
		ofSetColor(255);
        text << "fps: " << ofGetFrameRate();
		ofDrawBitmapString(text.str(), ofGetWidth()-100, ofGetHeight()-14);
	}
}

//--------------------------------------------------------------
void ofxApp::keyPressed(int key) {
	if(_sceneManager)
		_sceneManager->keyPressed(key);
	keyPressedApp(key);
}

//--------------------------------------------------------------
void ofxApp::mouseMoved(int x, int y) {
	if(_sceneManager)
		_sceneManager->mouseMoved(x, y);
	mouseMovedApp(x, y);
}

//--------------------------------------------------------------
void ofxApp::mouseDragged(int x, int y, int button) {
	if(_sceneManager)
		_sceneManager->mouseDragged(x, y, button);
	mouseDraggedApp(x, y, button);
	
	if(bDebug) {
		if(_bWarp && _bEditingWarpPoints) {
			if(_currentWarpPoint >= 0) {
				_quadWarper.setPoint(_currentWarpPoint,
					ofVec2f((float)x/ofGetWidth(), (float)y/ofGetHeight()));
			}
		} else {
			controlPanel.mouseDragged(x, y, button);
		}
	}
}

//--------------------------------------------------------------
void ofxApp::mousePressed(int x, int y, int button) {
	if(_sceneManager)
		_sceneManager->mousePressed(x, y, button);
	mousePressedApp(x, y, button);
	
	if(bDebug) {
		if(_bWarp && _bEditingWarpPoints) {
		
			// check if middle of the screen was pressed to exit edit mode
			if((x > ofGetWidth()/2  - 50 && (x < ofGetWidth()/2  + 50) &&
			   (y > ofGetHeight()/2 - 50 && (y < ofGetHeight()/2 + 50))))
			{
				_bEditingWarpPoints = false;
				return;
			}
			
			// check if the screen corners are being clicked
			float smallestDist = 1.0;
			_currentWarpPoint = -1;
			for(int i = 0; i < 4; i++) {
				float distx = _quadWarper.getPoint(i).x - (float) x/ofGetWidth();
				float disty = _quadWarper.getPoint(i).y - (float) y/ofGetHeight();
				float dist  = sqrt(distx * distx + disty * disty);

				if(dist < smallestDist && dist < 0.1) {
					_currentWarpPoint = i;
					smallestDist = dist;
				}
			}	
		} else {
			controlPanel.mousePressed(x, y, button);
		}
	}
}

//--------------------------------------------------------------
void ofxApp::mouseReleased(int x, int y, int button) {
	if(_sceneManager)
		_sceneManager->mouseReleased(x, y, button);
	mouseReleasedApp(x, y, button);
	
	if(bDebug) {
		if(!_bEditingWarpPoints) {
			controlPanel.mouseReleased();
		}
	}
	_currentWarpPoint = -1;
}

//--------------------------------------------------------------
void ofxApp::_setupControlPanel() {
	controlPanel.setup("App Controls", 1, 0, 275, getRenderHeight()-40);
}
