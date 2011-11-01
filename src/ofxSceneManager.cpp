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
	
	_scenes.insert(_scenes.end(), pair<std::string,ofxScene*>(scene->getName(), scene));
}
		
//--------------------------------------------------------------
void ofxSceneManager::remove(ofxScene* scene) {
	if(scene == NULL) {
		ofLogWarning("ofxSceneManager") << "Cannot remove NULL scene";
		return;
	}
	
	map<std::string,ofxScene*>::iterator iter;
	map<std::string,ofxScene*>::iterator diter;
	for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		ofxScene* s = (*iter).second;
		if(s == scene) {
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
	map<std::string,ofxScene*>::iterator iter;
	for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
		ofxScene* s = (*iter).second;
		if(s != NULL) {
			s->exit();
			delete s;
		}
	}
	_scenes.clear();
}

//--------------------------------------------------------------
void ofxSceneManager::setup(bool loadAll) {
	if(loadAll) {
		map<std::string,ofxScene*>::iterator iter;
		for(iter = _scenes.begin(); iter != _scenes.end(); ++iter) {
			ofxScene* s = (*iter).second;
			s->setup();
		}
    } else {	// load the current one only
        if(!_scenes.empty() && _currentScene >= 0) {
			_currentScenePtr->setup();
        }
    }
}

//--------------------------------------------------------------
void ofxSceneManager::update() {

	handleSceneChanges();

	// update the current main scene
	if(!_scenes.empty() && _currentScene >= 0) {
	
		ofxScene* s = _currentScenePtr;

		// call setup if scene is not setup yet
		if(!s->isSetup()) {
			s->setup();
		}

		s->update();

		// if this scene says it's done, go to the next one
		if(s->isDone() && !_bSignalledAutoChange) {
			nextScene();
			_bSignalledAutoChange = true;
		}
	}
}

//--------------------------------------------------------------
void ofxSceneManager::draw() {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->draw();
	}
}

//--------------------------------------------------------------
void ofxSceneManager::keyPressed(int key) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->keyPressed(key);
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
void ofxSceneManager::mouseReleased(int x, int y, int button) {
	if(!_scenes.empty() && _currentScene >= 0) {
		_currentScenePtr->mouseReleased(x, y, button);
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
		s = getSceneAt(num);
		s->startEntering();
	}
	
	_newScene = num;
	_bChangeNow = now;
	ofLogVerbose("ofxSceneManager") << "GOTO SCENE: " << _newScene;
}

//--------------------------------------------------------------
void ofxSceneManager::gotoScene(std::string name, bool now) {
	map<std::string,ofxScene*>::iterator iter = _scenes.find(name);
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
				_currentScenePtr = getSceneAt(_currentScene);
                _newScene = SCENE_NOCHANGE; // done
                _bSignalledAutoChange = false;
            	_sceneChangeTimer.set();
				ofLogVerbose("ofxSceneManager") << "changed to " << _currentScene
					<< " \"" << _currentScenePtr->getName() << "\"";
			}
        } else {   // no current scene to wait for
            _currentScene = _newScene;
			_currentScenePtr = getSceneAt(_currentScene);
            _newScene = SCENE_NOCHANGE; // done
            _bSignalledAutoChange = false;
			_sceneChangeTimer.set();
			ofLogVerbose("ofxSceneManager") << "changed to " << _currentScene
					<< " \"" << _currentScenePtr->getName() << "\"";
        }
    }
}

//--------------------------------------------------------------
ofxScene* ofxSceneManager::getSceneAt(int index) {
	map<std::string,ofxScene*>::iterator iter = _scenes.begin();
	std::advance(iter, index);
	return (*iter).second;
}
