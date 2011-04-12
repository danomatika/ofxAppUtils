#include "ofxAppUtils.h"


/// APP

//--------------------------------------------------------------
ofxAppUtilsApp::ofxAppUtilsApp() : ofBaseApp(), origin(0, 0, 0) {
	
	bDebug = false;
	
	bScale = false;
	bMirrorX = false;
	bMirrorY = false;
	bTranslate = false;
	bHandleAspect = false;
	
	renderWidth = ofGetWidth();
	renderHeight = ofGetHeight();
	renderScaleX = 1;
	renderScaleY = 1;
	renderAspect = 1;
	screenAspect = ofGetWidth()/ofGetHeight();
}

//--------------------------------------------------------------
void ofxAppUtilsApp::setRenderSize(float w, float h, bool handleAspectRatio, bool useFbo) {
	
	bHandleAspect = handleAspectRatio;
	renderAspect = w/h;
	renderWidth = w;
	renderHeight = h;
	renderScaleX = (float) ofGetWidth()/renderWidth;
	renderScaleY = (float) ofGetHeight()/renderHeight;
	
	bUseFbo = useFbo;
	if(useFbo) {
		fbo.setup(w, h);
	}
}

//--------------------------------------------------------------
void ofxAppUtilsApp::setRenderScale(float x, float y) {
	renderScaleX = x;
	renderScaleY = y;	
}

//--------------------------------------------------------------
void ofxAppUtilsApp::applyRenderScale() {
	if(renderScaleX != 1.0 || renderScaleY != 1.0) {
		
		// adjust to screen dimensions?
		if(bHandleAspect) {
			
			if(renderAspect == 1.0) { // square, windowbox
				ofTranslate((ofGetWidth()-(renderScaleY*renderWidth))/4,
							(ofGetHeight()+(renderScaleX*renderHeight))/4);
			}
			else if(renderAspect < 1.0) { // w < h, letterbox
				ofScale(renderScaleX, renderScaleX);
				ofTranslate((0, ofGetHeight()-(renderScaleX*renderHeight))/4);
			}
			else { // w > h, pillarbox
				ofScale(renderScaleY, renderScaleY);
				ofTranslate((ofGetWidth()-(renderScaleY*renderWidth))/2, 0);
			}
		
		} else { // basic stretch
			ofScale(renderScaleX, renderScaleY);
		}
	}
}

//--------------------------------------------------------------
void ofxAppUtilsApp::applyMirrorX() {
	ofRotateY(180);
	ofTranslate(-renderWidth, 0, 0);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::applyMirrorY() {
	ofRotateX(180);
	ofTranslate(0, -renderHeight, 0);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::applyOriginTranslate() {
	ofTranslate(origin.x, origin.y, origin.z);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::update() {
	controlPanel.update();
	updateApp();
}

//--------------------------------------------------------------
void ofxAppUtilsApp::draw() {

	if(bUseFbo) {
		fbo.begin();
	}

	ofPushMatrix();
	
		if(bScale) {
			applyRenderScale();
		}
		
		if(bMirrorX) {
			applyMirrorX();
		}
		
		if(bMirrorY) {
			applyMirrorY();
		}
		
		if(bTranslate) {
			applyOriginTranslate();
		}
			
		drawApp();	// do the user callback
	
	ofPopMatrix();
	
	if(bUseFbo) {
		fbo.end();
		fbo.draw(0, 0);
	}
	
	if(bDebug) {
		controlPanel.draw();
		ofBitmapString(14, 14) << "fps: " << ofGetFrameRate();
	}
}

//--------------------------------------------------------------
void ofxAppUtilsApp::keyPressed(int key) {
	keyPressedApp(key);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::mouseMoved(int x, int y) {
	mouseMovedApp(x, y);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::mouseDragged(int x, int y, int button){
	mouseDraggedApp(x, y, button);
	if(bDebug)
		controlPanel.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::mousePressed(int x, int y, int button){
	mousePressedApp(x, y, button);
	if(bDebug)
		controlPanel.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofxAppUtilsApp::mouseReleased(int x, int y, int button){
	mouseReleasedApp(x, y, button);
	if(bDebug)
		controlPanel.mouseReleased();
}

/// SCENE

//--------------------------------------------------------------
void ofxAppUtilsScene::setup() {
	if(!bSetup){
		setupScene();
		bSetup = true;
	}
}

//--------------------------------------------------------------
void ofxAppUtilsScene::update() {
 	if(!bSetup || !bRunning)
		return;

	if(bEntering) {
		updateSceneEnter();
		bEnteringFirst = false;
	}
	else if(bExiting) {
		updateSceneExit();
		bExitingFirst = false;
	}
	else {
		updateScene();
	}
}
		
//--------------------------------------------------------------
void ofxAppUtilsScene::draw() {
	if(!bSetup)
		return;
	drawScene();			
}

/// SCENE MANAGER

//--------------------------------------------------------------
ofxAppUtilsSceneManager::ofxAppUtilsSceneManager(ofxAppUtilsApp& app) :
	app(app), currentScene(SCENE_NONE), newScene(SCENE_NOCHANGE),
	bChangeNow(false), minChangeTimeMS(100), bSignalledAutoChange(false)
{
	ofLogAddTopic("SceneManager");
	sceneChangeTimer.set();
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::add(ofxAppUtilsScene* scene) {
	if(scene == NULL) {
		ofLogWarning("SceneManager") << "Cannot add NULL scene";
		return;
	}
	//scenes.push_back(scene);
	if(scenes.find(scene->getName()) != scenes.end()) {
		ofLogWarning("SceneManager") << "Scene " << scene->getName()
									 << " already added";
		return;
	}
	
	scenes.insert(scenes.end(), pair<std::string,ofxAppUtilsScene*>(scene->getName(), scene));
}
		
//--------------------------------------------------------------
void ofxAppUtilsSceneManager::remove(ofxAppUtilsScene* scene) {
	if(scene == NULL) {
		ofLogWarning("SceneManager") << "Cannot remove NULL scene";
		return;
	}
	
//	// find scene in vector remove it
//	for(unsigned int i = 0; i < scenes.size(); ++i) {
//		ofxAppUtilsScene* s = scenes[i];
//		if(s == scene) {
//			if(s != NULL) delete s;
//			scenes.erase(scenes.begin()+i);
//			return;
//		}
//	}
	map<std::string,ofxAppUtilsScene*>::iterator iter;
	map<std::string,ofxAppUtilsScene*>::iterator diter;
	for(iter = scenes.begin(); iter != scenes.end(); ++iter) {
		ofxAppUtilsScene* s = (*iter).second;
		if(s == scene) {
			if(s != NULL) delete s;
			diter = iter;
			scenes.erase(diter);
			return;
		}
	}

}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::clear() {
//	for(unsigned int i = 0; i < scenes.size(); ++i) {
//		ofxAppUtilsScene* s = scenes[i];
//		if(s != NULL) delete s;
//	}
	map<std::string,ofxAppUtilsScene*>::iterator iter;
	for(iter = scenes.begin(); iter != scenes.end(); ++iter) {
		ofxAppUtilsScene* s = (*iter).second;
		if(s != NULL) delete s;
	}
	scenes.clear();
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::setup(bool loadAll) {
	if(loadAll) {
//        for(unsigned int i = 0; i < scenes.size(); i++) {
//            scenes[i]->setup();
//        }
		map<std::string,ofxAppUtilsScene*>::iterator iter;
		for(iter = scenes.begin(); iter != scenes.end(); ++iter) {
			ofxAppUtilsScene* s = (*iter).second;
			s->setup();
		}
    } else {	// load the current one only
        if(!scenes.empty() && currentScene >= 0) {
//            scenes[currentScene]->setup();
			currentScenePtr->setup();
        }
    }
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::update() {

	handleSceneChanges();

	// update the current main scene
	if(!scenes.empty() && currentScene >= 0) {
	
//		ofxAppUtilsScene* s = scenes[currentScene];
		ofxAppUtilsScene* s = currentScenePtr;

		// call setup if scene is not setup yet
		if(!s->isSetup()) {
			s->setup();
		}

		s->update();

		// if this scene says it's done, go to the next one
		if(s->isDone() && !bSignalledAutoChange) {
			nextScene();
			bSignalledAutoChange = true;
		}
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::draw() {
	if(!scenes.empty() && currentScene >= 0) {
//		scenes[currentScene]->draw();
		currentScenePtr->draw();
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::keyPressed(int key) {
	if(!scenes.empty() && currentScene >= 0) {
//		scenes[currentScene]->keyPressed(key);
		currentScenePtr->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::mouseMoved(int x, int y ) {
	if(!scenes.empty() && currentScene >= 0) {
//		scenes[currentScene]->mouseMoved(x, y);
		currentScenePtr->mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::mouseDragged(int x, int y, int button) {
	if(!scenes.empty() && currentScene >= 0) {
//		scurrentScenePtr->mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::mousePressed(int x, int y, int button) {
	if(!scenes.empty() && currentScene >= 0) {
//		scenes[currentScene]->mousePressed(x, y, button);
		currentScenePtr->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::mouseReleased(int x, int y, int button) {
	if(!scenes.empty() && currentScene >= 0) {
//		scenes[currentScene]->mouseReleased(x, y, button);
		currentScenePtr->mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::run(bool run) {
	if(!scenes.empty() && currentScene >= 0) {
//        scenes[currentScene]->run(run);
		currentScenePtr->run(run);
		ofLogDebug("SceneManager") << "SCENE " << currentScenePtr->getName()
					  << " RUN: " << currentScenePtr->isRunning();
    }
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::runToggle() {
	run(!isRunning());
}

//--------------------------------------------------------------
bool ofxAppUtilsSceneManager::isRunning() {
	if(!scenes.empty() && currentScene >= 0) {
//        return scenes[currentScene]->isRunning();
		return currentScenePtr->isRunning();
	}
	return false;
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::noScene(bool now) {
	if(sceneChangeTimer.getDiff() < minChangeTimeMS)
		return;
	
	if(!now && currentScene > -1) {
//		scenes[currentScene]->startExiting();
		currentScenePtr->startExiting();
	}
	bChangeNow = now;
	newScene = SCENE_NONE;
	ofLogDebug("SceneManager") << "NO SCENE";
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::nextScene(bool now) {
	if(currentScene+1 >= (int) scenes.size()) {
        gotoScene(0, now);
    } else {
        gotoScene(currentScene+1, now);
    }
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::prevScene(bool now) {
	if(currentScene-1 < 0) {
        gotoScene(scenes.size()-1, now);
    } else {
        gotoScene(currentScene-1, now);
    }
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::gotoScene(unsigned int num, bool now) {
	if(scenes.empty() || num >= scenes.size() ||
	   sceneChangeTimer.getDiff() < minChangeTimeMS)
        return;

	ofxAppUtilsScene* s;
	if(currentScene == (int) num) {
		ofLogWarning("SceneManager") << "ignoring duplicate goto scene change";
		return;
	}

	if(!now) {
	
		// tell current scene to exit
		if(currentScene > SCENE_NONE) {
//			s = scenes[currentScene];
			s = currentScenePtr;
			s->startExiting();
		}

		// tell new scene to enter
//		s = scenes[num];
		s = getSceneAt(num);
		s->startEntering();
	}
	
	newScene = num;
	bChangeNow = now;
	ofLogDebug("SceneManager") << "GOTO SCENE: " << newScene;
}

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::gotoScene(std::string name, bool now) {
	map<std::string,ofxAppUtilsScene*>::iterator iter = scenes.find(name);
	if(iter == scenes.end()) {
		ofLogWarning("SceneManager") << "Could not find \"" << name << "\"";
	}
	currentScenePtr = (*iter).second;
}

/* ***** PRIVATE ***** */

//--------------------------------------------------------------
void ofxAppUtilsSceneManager::handleSceneChanges() {

	// do the actual main scene change
    if(newScene != SCENE_NOCHANGE) {
	
        // ignore duplicates if not done entering
        if(newScene == currentScene) {
//            ofxAppUtilsScene* s = scenes[currentScene];
            if(currentScenePtr->isEntering()) {
                newScene = SCENE_NOCHANGE;
                ofLogWarning("SceneManager") << "ignoring duplicate scene change, "
                          	<< "current scene is not done entering";
				sceneChangeTimer.set();
			}
        }

        // only change to the new scene if the old scene is done exiting
        if(currentScene > SCENE_NONE) {
//            ofxAppUtilsScene* s = scenes[currentScene];
            if(bChangeNow || !currentScenePtr->isExiting()) {
                currentScene = newScene;
				currentScenePtr = getSceneAt(currentScene);
                newScene = SCENE_NOCHANGE; // done
                bSignalledAutoChange = false;
            	sceneChangeTimer.set();
				ofLogDebug("SceneManager") << "changed to " << currentScene
					<< " " << currentScenePtr->getName();
			}
        } else {   // no current scene to wait for
            currentScene = newScene;
			currentScenePtr = getSceneAt(currentScene);
            newScene = SCENE_NOCHANGE; // done
            bSignalledAutoChange = false;
			sceneChangeTimer.set();
			ofLogDebug("SceneManager") << "changed to " << currentScene
					<< " " << currentScenePtr->getName();
        }
    }
}

//--------------------------------------------------------------
ofxAppUtilsScene* ofxAppUtilsSceneManager::getSceneAt(int index) {
	map<std::string,ofxAppUtilsScene*>::iterator iter = scenes.begin();
	std::advance(iter, index);
	return (*iter).second;
}
