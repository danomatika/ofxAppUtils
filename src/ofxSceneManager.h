/*
 * Copyright (c) 2011-2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include <map>
#include <climits>

#include "ofxApp.h"
#include "ofxScene.h"
#include "ofxTimer.h"

///	\class	SceneManager
///	\brief	a map based scene manager
class ofxSceneManager {
	public:

		ofxSceneManager();
		virtual ~ofxSceneManager() {}
		
	/// \section Main
		
		/// add a scene
		/// returns a pointer to the scene or NULL if the scene could not be added
		ofxScene* add(ofxScene *scene);
		
		// remove a scene
		void remove(ofxScene *scene);
		
		// clear all scenes
		void clear();
		
		/// setup current scenes
		/// set loadAll to true to load all scenes at once
		///
		/// unloaded scenes are automatically loaded on their first update
		void setup(bool loadAll=true);
		
	/// \section Scene Control
		
		/// play/pause the current scene
		void run(bool run);
		void runToggle();
		bool isRunning(); ///< is the current scene running?
		
		/// scene transport
		void noScene(bool now=false);
		void nextScene(bool now=false);
		void prevScene(bool now=false);
		void gotoScene(unsigned int index, bool now=false);
		void gotoScene(std::string name, bool now=false);
		
		/// scene access
		/// returns NULL if scene not found
		ofxScene* getScene(std::string name);
		ofxScene* getSceneAt(unsigned int index);
		
		/// returns empty string "" if index out of range
		std::string getSceneName(unsigned int index);
		
		/// returns -1 if index scene not found
		int getSceneIndex(std::string name);
		
		/// current scene access
		/// returns NULL if there is no current scene
		ofxScene* getCurrentScene();
		
		/// returns "NO_SCENE" if there is no current scene
		std::string getCurrentSceneName(); 
		
		/// returns -1 if there is no current scene
		int getCurrentSceneIndex();
		
		/// returns the current number of scenes
		int getNumScenes() {return _scenes.size();}
		
	/// \section Util
		
		/// get/set the minimum allowed time between scene changes in ms
		/// note: this is ignored it the change is done "now"
		unsigned int getMinChangeTime();
		void setMinChangeTime(unsigned int time);
	
		/// get/set overlapping current & new scene updates on a scene change
		/// note: current scene does not change until end of the transition
		void setOverlap(bool overlap);
		bool getOverlap();
		
	/// \section Current Scene Callbacks
		
		/// these are called in the current scene
		
		/// ofBaseApp callbacks
		void update();
		void draw();
		/// exit() is called automatically on removal/clear
	
		/// this is sent to all currently loaded scenes so resize events
		/// are handled during transtions, etc correctly
		void windowResized(int w, int h);

		void keyPressed(int key);
		void keyReleased(int key);

		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
	
	#ifdef TARGET_OF_IOS
		/// ofxIOSApp callbacks
		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		
		/// this is sent to all currently loaded scenes so orientation changes
		/// are handled during transtions, etc correctly
		void deviceOrientationChanged(int newOrientation);
	#endif
		
		/// ofBaseSoundInput callbacks
		void audioIn(ofSoundBuffer& buffer);
		void audioIn(float *input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
		void audioIn(float *input, int bufferSize, int nChannels );
		void audioReceived(float *input, int bufferSize, int nChannels);
		
		/// ofBaseSoundOutput callbacks
		void audioOut(ofSoundBuffer& buffer);
		void audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
		void audioOut(float *output, int bufferSize, int nChannels);
		void audioRequested(float *output, int bufferSize, int nChannels);
		
	private:
	
		/// handle a pending scene change
		void _handleSceneChanges();
		
		/// do actual scene change
		void changeToNewScene();
		
		/// wrapper around iter + advance
		ofxScene::RunnerScene* _getRunnerSceneAt(int index);
	
		/// valid scene index value enums
		enum {
			SCENE_NOCHANGE = INT_MIN,
			SCENE_NONE = -1,
		};
	
		ofxScene *_currentScenePtr; //< pointer to the current scene
		ofxScene::RunnerScene *_currentRunnerScenePtr; //< pointer to the current runner scene
		ofxScene::RunnerScene *_newRunnerScenePtr; //< pointer to the next runner scene (when overlapping)
		int _currentScene; //< the current scene, < 0 if none
		int _newScene;     //< scene to change to
		bool _bChangeNow;  //< ignore enter and exit when changing scenes?
		bool _bOverlap;    //< make new scenes start entering while current scene is finishing?
		
		std::map<std::string,ofxScene::RunnerScene*> _scenes; //< scenes
	
		bool _bSignalledAutoChange;    //< has an automatic change been called?
		unsigned int _minChangeTimeMS; //< minimum ms to wait before accepting scene change commands

		ofxTimer _sceneChangeTimer;    //< timers to keep track of change times
};
