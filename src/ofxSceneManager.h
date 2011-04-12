#pragma once

#include <ofMain.h>

#include "ofxApp.h"
#include "ofxScene.h"
#include "ofxTimer.h"

/**
	\class	SceneManager
	\brief	a map based scene manager
**/
class ofxSceneManager {

    public:

        ofxSceneManager(ofxApp& app);
        virtual ~ofxSceneManager() {}
		
		/// \section Main
		
		/// add a scene
		void add(ofxScene* scene);
		
		// remove a scene
		void remove(ofxScene* scene);
		
		// clear all scenes
		void clear();
		
		/// setup current scenes
		/// set loadAll to true to laod all scenes at once
		void setup(bool loadAll=true);
		
		/// update the current scene
		void update();
		
		/// render the current scene
		void draw();
		
		//// \section Input
		
		/// passes input to current scene
		void keyPressed(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		/// \section Scene Control
		
		/// play/pause the current scene
		void run(bool run);
        void runToggle();
		bool isRunning();	///< is the current scene running?
		
		/// scene transport
		void noScene(bool now=false);
        void nextScene(bool now=false);
        void prevScene(bool now=false);
        void gotoScene(unsigned int num, bool now=false);
		void gotoScene(std::string name, bool now=false);
		
	protected:
		
		ofxApp& app;	///< the parent application reference
		
	private:
	
		/// handle a pedning scene change
		void handleSceneChanges();
		
		/// wrapper around iter + advance
		ofxScene* getSceneAt(int index);
	
		/// valid scene index value enums
        enum
        {
            SCENE_NOCHANGE = INT_MIN,
            SCENE_NONE = -1,
        };
	
		ofxScene*	_currentScenePtr;	///< pointer to the current scene
		int _currentScene;	///< the current scene, < 0 if none
		int _newScene;		///< scene to change to
		bool _bChangeNow;	///< ignore enter and exit when changing scenes?
		
		std::map<std::string, ofxScene*> _scenes;	///< scenes
	
		bool _bSignalledAutoChange;		///< has an automatic change been called?
		unsigned int _minChangeTimeMS;	///< minimum ms to wait before accepting scene change commands

        ofxTimer _sceneChangeTimer;	///< timers to keep track of change times
};
