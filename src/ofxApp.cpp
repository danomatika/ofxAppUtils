#include "ofxApp.h"

#include "matrix_funcs.h"
#include "ofxSceneManager.h"

/// APP

//--------------------------------------------------------------
ofxApp::ofxApp() :
	ofBaseApp(), _origin(0, 0, 0), _currentWarpPoint(-1) {
	
	bDebug = false;
	
	_bScale = false;
	_bMirrorX = false;
	_bMirrorY = false;
	_bTranslate = false;
	_bHandleAspect = false;
	_bWarp = false;
	
	_bEditingWarpPoints = false;
	
	_renderWidth = ofGetWidth();
	_renderHeight = ofGetHeight();
	_renderScaleX = 1;
	_renderScaleY = 1;
	_renderAspect = 1;
	_screenAspect = ofGetWidth()/ofGetHeight();
	
	_bTransformControls = false;
	
	_sceneManager = NULL;
	
	resetWarp();
}

//--------------------------------------------------------------
void ofxApp::setRenderSize(float w, float h, bool handleAspectRatio,
								   bool translate, bool useWarp) {
	_bHandleAspect = handleAspectRatio;
	_bTranslate = translate;
	_bWarp = useWarp;
	_renderAspect = w/h;
	_renderWidth = w;
	_renderHeight = h;
	_renderScaleX = (float) ofGetWidth()/_renderWidth;
	_renderScaleY = (float) ofGetHeight()/_renderHeight;
}

//--------------------------------------------------------------
void ofxApp::setRenderScale(float x, float y) {
	_renderScaleX = x;
	_renderScaleY = y;	
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
void ofxApp::resetWarp() {
	_warpPoints[0].set(0.0, 0.0);
	_warpPoints[1].set(1.0, 0.0);
	_warpPoints[2].set(1.0, 1.0);
	_warpPoints[3].set(0.0, 1.0);
}

bool ofxApp::loadWarpSettings(const string xmlFile) {
	
	ofxXmlSettings xml;
	if(!xml.loadFile(xmlFile))
		return false;
		
	_warpPoints[0].x	= xml.getValue("quad:upperLeft:x", 0.0);
	_warpPoints[0].y	= xml.getValue("quad:upperLeft:y", 0.0);
	
	_warpPoints[1].x	= xml.getValue("quad:upperRight:x", 1.0);
	_warpPoints[1].y	= xml.getValue("quad:upperRight:y", 0.0);
	
	_warpPoints[2].x	= xml.getValue("quad:lowerRight:x", 1.0);
	_warpPoints[2].y	= xml.getValue("quad:lowerRight:y", 1.0);
	
	_warpPoints[3].x	= xml.getValue("quad:lowerLeft:x", 0.0);
	_warpPoints[3].y	= xml.getValue("quad:lowerLeft:y", 1.0);
	
	return true;
}

void ofxApp::saveWarpSettings(const string xmlFile) {

	ofxXmlSettings xml;
	
	xml.addTag("quad");
	xml.pushTag("quad");
	
	xml.addTag("upperLeft");
	xml.pushTag("upperLeft");
	xml.addValue("x", _warpPoints[0].x);
	xml.addValue("y", _warpPoints[0].y);
	xml.popTag();
	
	xml.addTag("upperRight");
	xml.pushTag("upperRight");
	xml.addValue("x", _warpPoints[1].x);
	xml.addValue("y", _warpPoints[1].y);
	xml.popTag();
	
	xml.addTag("lowerRight");
	xml.pushTag("lowerRight");
	xml.addValue("x", _warpPoints[2].x);
	xml.addValue("y", _warpPoints[2].y);
	xml.popTag();
	
	xml.addTag("lowerLeft");
	xml.pushTag("lowerLeft");
	xml.addValue("x", _warpPoints[3].x);
	xml.addValue("y", _warpPoints[3].y);
	xml.popTag();
	
	xml.saveFile(xmlFile);
}

//--------------------------------------------------------------
void ofxApp::setWarpPoint(unsigned int index, ofVec2f point) {
	if(index > 3)
		return;
	_warpPoints[index] = point;
}

ofVec2f ofxApp::getWarpPoint(unsigned int index) {
	if(index > 3)
		return ofVec2f();
	return _warpPoints[index];
}

//--------------------------------------------------------------
void ofxApp::applyRenderScale() {
	if(_renderScaleX != 1.0 || _renderScaleY != 1.0) {
		
		// adjust to screen dimensions?
		if(_bHandleAspect) {
			
			if(_renderAspect == 1.0) { // square, windowbox
				ofTranslate((ofGetWidth()-(_renderScaleY*_renderWidth))/4,
							(ofGetHeight()+(_renderScaleX*_renderHeight))/4);
			}
			else if(_renderAspect < 1.0) { // w < h, letterbox
				ofScale(_renderScaleX, _renderScaleX);
				ofTranslate((0, ofGetHeight()-(_renderScaleX*_renderHeight))/4);
			}
			else { // w > h, pillarbox
				ofScale(_renderScaleY, _renderScaleY);
				ofTranslate((ofGetWidth()-(_renderScaleY*_renderWidth))/2, 0);
			}
		
		} else { // basic stretch
			ofScale(_renderScaleX, _renderScaleY);
		}
	}
}

//--------------------------------------------------------------
void ofxApp::applyMirrorX() {
	ofRotateY(180);
	ofTranslate(-_renderWidth, 0, 0);
}

//--------------------------------------------------------------
void ofxApp::applyMirrorY() {
	ofRotateX(180);
	ofTranslate(0, -_renderHeight, 0);
}

//--------------------------------------------------------------
void ofxApp::applyOriginTranslate() {
	ofTranslate(_origin);
}

//--------------------------------------------------------------
//
// projection warping from a modified Theo example on the OF forums:
// http://threeblindmiceandamonkey.com/?p=31
//
void ofxApp::applyWarp() {

	// we set it to the default - 0 translation
	// and 1.0 scale for x y z and w
	for(int i = 0; i < 16; i++) {
		if(i % 5 != 0) _glWarpMatrix[i] = 0.0;
		else _glWarpMatrix[i] = 1.0;
	}

	// source and destination points
	double src[4][2];
	double dest[4][2];

	// we set the warp coordinates
	// source coordinates as the dimensions of our window
	src[0][0] = 0;
	src[0][1] = 0;
	src[1][0] = _renderWidth;
	src[1][1] = 0;
	src[2][0] = _renderWidth;
	src[2][1] = _renderHeight;
	src[3][0] = 0;
	src[3][1] = _renderHeight;

	// corners are in 0.0 - 1.0 range
	// so we scale up so that they are at the window's scale
	for(int i = 0; i < 4; i++){
		dest[i][0] = _warpPoints[i].x * (float) _renderWidth;
		dest[i][1] = _warpPoints[i].y * (float) _renderHeight;
	}

	// perform the warp calculation
	mapQuadToQuad(src, dest, _warpMatrix);

	// copy the values
	_glWarpMatrix[0]	= _warpMatrix[0][0];
	_glWarpMatrix[1]	= _warpMatrix[0][1];
	_glWarpMatrix[3]	= _warpMatrix[0][2];

	_glWarpMatrix[4]	= _warpMatrix[1][0];
	_glWarpMatrix[5]	= _warpMatrix[1][1];
	_glWarpMatrix[7]	= _warpMatrix[1][2];

	_glWarpMatrix[12]	= _warpMatrix[2][0];
	_glWarpMatrix[13]	= _warpMatrix[2][1];
	_glWarpMatrix[15]	= _warpMatrix[2][2];

	// finally lets multiply our matrix
	glMultMatrixf(_glWarpMatrix);
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
		ofxLogWarning("ofxApp") << "cannot add NULL scene manager";
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
		if(_bEditingWarpPoints) {
		
			ofSetHexColor(0x00FF00);
			ofxBitmapString(28, 28) << "Quad Warper Edit Mode" << endl
									<< "Drag from the corners of the screen" << endl
								    << "Click center rectangle to exit";
				
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
		ofxBitmapString(ofGetWidth()-100, ofGetHeight()-14) << "fps: " << ofGetFrameRate();
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
				_warpPoints[_currentWarpPoint].x = (float)x/ofGetWidth();
				_warpPoints[_currentWarpPoint].y = (float)y/ofGetHeight();
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
				float distx = _warpPoints[i].x - (float) x/ofGetWidth();
				float disty = _warpPoints[i].y - (float) y/ofGetHeight();
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
