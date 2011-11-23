/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#include "ofxSceneManager.h"

/// SCENE MANAGER

//--------------------------------------------------------------
ofxSceneManager::ofxSceneManager(ofxApp& app) :
	app(app), _currentScene(SCENE_NONE), _newScene(SCENE_NOCHANGE),
	_bChangeNow(false), _minChangeTimeMS(100), _bSignalledAutoChange(false)
{
	_sceneChangeTimer.set();
}

//--------------------------------------------------------------
void ofxSceneManager::add(ofxScene* scene) {
	if(scene == NULL) {
		ofLogWarning("ofxSceneManager") << "Cannot add NULL scene";
		return;
	}

	if(_scenes.find(scene->getName()) != _scenes.end()) {
		ofLogWarning("ofxSceneManager") << "Scene " << scene->getName()
									 << " already added, only unique names allowed";
		return;
	}
	
	_scenes.insert(_scenes.end(), pair<std::string,ofxRunnerScene*>(scene->getName(), new ofxRunnerScene(scene)));
}
		
//--------------------------------------------------------------
void ofxSceneManager::remove(ofxScene* scene) {
	if(scene == NULL) {
		ofLogWarning("ofxSceneManager") << "Cannot remove NULL scene";
		return;
	}
	
	map<std::string,ofxRunnerScene*>::iterator iter;
	map<std::string,ofxRunnerScene*>::iterator diter;
	for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		ofxRunnerScene* s = (*iter).second;
		if(s->scene == scene) {
			if(s != NULL) {
				s->exit();
				delete s;
			}
			diter = iter;
			_scenes.erase(diter);
			return;
		}
	}

}

//--------------------------------------------------------------
void ofxSceneManager::clear() {
	map<std::string,ofxRunnerScene*>::iterator iter;
	for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		ofxRunnerScene* s = (*iter).second;
		if(s != NULL) {
			s->exit();
			delete s;
		}
	}
	_scenes.clear();
}

// need to call ofxRunnerScene::setup()
//--------------------------------------------------------------
void ofxSceneManager::setup(bool loadAll) {
	if(loadAll) {
		map<std::string,ofxRunnerScene*>::iterator iter;
		for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
			ofxRunnerScene* s = (*iter).second;
			s->setup();
		}
    } else {	// load the current one only
        if(!_scenes.empty() && _currentScene >= 0) {
			_currentRunnerScenePtr->setup();
        }
    }
}

//--------------------------------------------------------------
void ofxSceneManager::run(bool run) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->run(run);
		ofLogVerbose("ofxSceneManager") << "SCENE " << _currentScenePtr->getName()
					  << " RUN: " << _currentScenePtr->isRunning();
    }
}

//--------------------------------------------------------------
void ofxSceneManager::runToggle() {
	run(!isRunning());
}

//--------------------------------------------------------------
bool ofxSceneManager::isRunning() {
	if(!_scenes.empty() && _currentScene >= 0) {
		return _currentScenePtr->isRunning();
	}
	return false;
}

//--------------------------------------------------------------
void ofxSceneManager::noScene(bool now) {
	if(_sceneChangeTimer.getDiff() < _minChangeTimeMS)
		return;
	
	if(!now && _currentScene > -1) {
		_currentScenePtr->startExiting();
	}
	_bChangeNow = now;
	_newScene = SCENE_NONE;
	ofLogVerbose("ofxSceneManager") << "NO SCENE";
}

//--------------------------------------------------------------
void ofxSceneManager::nextScene(bool now) {
	if(_currentScene+1 >= (int) _scenes.size()) {
        gotoScene(0, now);
    } else {
        gotoScene(_currentScene+1, now);
    }
}

//--------------------------------------------------------------
void ofxSceneManager::prevScene(bool now) {
	if(_currentScene-1 < 0) {
        gotoScene(_scenes.size()-1, now);
    } else {
        gotoScene(_currentScene-1, now);
    }
}

//--------------------------------------------------------------
void ofxSceneManager::gotoScene(unsigned int num, bool now) {
	if(_scenes.empty() || num >= _scenes.size() ||
	   _sceneChangeTimer.getDiff() < _minChangeTimeMS)
        return;

	ofxScene* s;
	if(_currentScene == (int) num) {
		ofLogWarning("ofxSceneManager") << "ignoring duplicate goto scene change";
		return;
	}

	if(!now) {
	
		// tell current scene to exit
		if(_currentScene > SCENE_NONE) {
			s = _currentScenePtr;
			s->startExiting();
		}

		// tell new scene to enter
		s = getSceneAt(num)->scene;
		s->startEntering();
	}
	
	_newScene = num;
	_bChangeNow = now;
	ofLogVerbose("ofxSceneManager") << "GOTO SCENE: " << _newScene;
}

//--------------------------------------------------------------
void ofxSceneManager::gotoScene(std::string name, bool now) {
	map<std::string,ofxRunnerScene*>::iterator iter = _scenes.find(name);
	if(iter == _scenes.end()) {
		ofLogWarning("ofxSceneManager") << "Could not find \"" << name << "\"";
	}
	gotoScene(std::distance(_scenes.begin(), iter), now);
}

/* ***** PRIVATE ***** */

//--------------------------------------------------------------
void ofxSceneManager::handleSceneChanges() {

	// do the actual main scene change
    if(_newScene != SCENE_NOCHANGE) {
	
        // ignore duplicates if not done entering
        if(_newScene == _currentScene) {
            if(_currentScenePtr->isEntering()) {
                _newScene = SCENE_NOCHANGE;
                ofLogWarning("ofxSceneManager") << "ignoring duplicate scene change, "
                          	<< "current scene is not done entering";
				_sceneChangeTimer.set();
			}
        }

        // only change to the new scene if the old scene is done exiting
        if(_currentScene > SCENE_NONE) {
            if(_bChangeNow || !_currentScenePtr->isExiting()) {
                _currentScene = _newScene;
                _currentRunnerScenePtr = getSceneAt(_currentScene);
				_currentScenePtr = _currentRunnerScenePtr->scene;
                _newScene = SCENE_NOCHANGE; // done
                _bSignalledAutoChange = false;
            	_sceneChangeTimer.set();
				ofLogVerbose("ofxSceneManager") << "changed to " << _currentScene
					<< " \"" << _currentScenePtr->getName() << "\"";
			}
        } else {   // no current scene to wait for
            _currentScene = _newScene;
			_currentRunnerScenePtr = getSceneAt(_currentScene);
				_currentScenePtr = _currentRunnerScenePtr->scene;
            _newScene = SCENE_NOCHANGE; // done
            _bSignalledAutoChange = false;
			_sceneChangeTimer.set();
			ofLogVerbose("ofxSceneManager") << "changed to " << _currentScene
					<< " \"" << _currentScenePtr->getName() << "\"";
        }
    }
}

//--------------------------------------------------------------
ofxRunnerScene* ofxSceneManager::getSceneAt(int index) {
	map<std::string,ofxRunnerScene*>::iterator iter = _scenes.begin();
	std::advance(iter, index);
	return (*iter).second;
}

// need to call ofxRunnerScene::update()
//--------------------------------------------------------------
void ofxSceneManager::update() {

	handleSceneChanges();

	// update the current main scene
	if(!_scenes.empty() && _currentScene >= 0) {
	
		ofxScene* s = _currentScenePtr;

		// call setup if scene is not setup yet
		if(!s->isSetup()) {
			_currentRunnerScenePtr->setup();
		}

		_currentRunnerScenePtr->update();

		// if this scene says it's done, go to the next one
		if(s->isDone() && !_bSignalledAutoChange) {
			nextScene();
			_bSignalledAutoChange = true;
		}
	}
}

// need to call ofxRunnerScene::draw()
//--------------------------------------------------------------
void ofxSceneManager::draw() {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentRunnerScenePtr->draw();
	}
}

//--------------------------------------------------------------
void ofxSceneManager::windowResized(int w, int h) {
    map<std::string,ofxRunnerScene*>::iterator iter;
    for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
        ofxRunnerScene* s = (*iter).second;
        s->windowResized(w, h);;
    }
}

//--------------------------------------------------------------
void ofxSceneManager::keyPressed(int key) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::keyReleased(int key) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->keyReleased(key);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::mouseMoved(int x, int y ) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->mouseMoved(x, y);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::mouseDragged(int x, int y, int button) {
	if(!_scenes.empty() && _currentScene >= 0) {
	}
}

//--------------------------------------------------------------
void ofxSceneManager::mousePressed(int x, int y, int button) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::mouseReleased() {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->mouseReleased();
	}
}

//--------------------------------------------------------------
void ofxSceneManager::mouseReleased(int x, int y, int button) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::dragEvent(ofDragInfo dragInfo) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->dragEvent(dragInfo);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::gotMessage(ofMessage msg){
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->gotMessage(msg);
	}
}

//--------------------------------------------------------------
void ofxSceneManager::audioIn(float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->audioIn(input, bufferSize, nChannels, deviceID, tickCount);
	}
}

void ofxSceneManager::audioIn(float * input, int bufferSize, int nChannel ) {
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->audioIn(input, bufferSize, nChannel);
	}
}
void ofxSceneManager::audioReceived(float * input, int bufferSize, int nChannels) {
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->audioIn(input, bufferSize, nChannels);
	}
}
        
//--------------------------------------------------------------
void ofxSceneManager::audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount) {
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->audioOut(output, bufferSize, nChannels, deviceID, tickCount);
	}
}

void ofxSceneManager::audioOut(float * output, int bufferSize, int nChannels) {
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->audioOut(output, bufferSize, nChannels);
	}
}

void ofxSceneManager::audioRequested(float * output, int bufferSize, int nChannels) {
    if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->audioOut(output, bufferSize, nChannels);
	}
}
