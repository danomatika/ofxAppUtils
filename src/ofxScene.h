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

#include "ofxApp.h"
#include "ofxTimer.h"

///	\class  Scene
///	\brief  application scene abstract class
class ofxScene :

#ifdef TARGET_OF_IOS
	public ofxiOSApp {
#else
	public ofBaseApp {
#endif
	
	public:
		
		/// the singleSetup bool controls whether the scene's setup function
		/// is called only once or each time a scene change is made
		ofxScene(std::string name, bool singleSetup=true) :
			_name(name), _bSetup(false), _bRunning(true),
			_bEntering(false), _bEnteringFirst(false),
			_bExiting(false), _bExitingFirst(false),
			_bDone(false), _bSingleSetup(singleSetup) {}
		virtual ~ofxScene() {}
		
	/// \section Main
		
		/// implement the regular ofBaseApp clalbacks functions
		/// ie setup(), update(), draw(), etc
		///
		/// there are also the following functions for scene enter and exit
		/// control

		/// called when entering
		virtual void updateEnter() {finishedEntering();};

		/// called when exiting
		virtual void updateExit()	{finishedExiting();};

	/// \section Transitional Control

		/// start entering
		inline void startEntering()     {_bEntering = true; _bEnteringFirst = true; _bDone = false;}

		/// the scene has finished entering (this should be called automatically by the scene itself)
		inline void finishedEntering()  {_bEntering = false; _bEnteringFirst = false;}

		/// is this scene currently entering?
		inline bool isEntering()        {return _bEntering;}

		/// is this scene entering for the first time?
		inline bool isEnteringFirst()   {return _bEnteringFirst;}

		/// start exiting
		inline void startExiting()      {_bExiting = true; _bExitingFirst = true;}

		/// the scene has finished exiting (this should be called automatically by the scene itself)
		inline void finishedExiting()   {_bExiting = false; _bExitingFirst = false;}

		/// is this scene currently exiting?
		inline bool isExiting()         {return _bExiting;}

		/// is this scene exiting for the first time?
		inline bool isExitingFirst()    {return _bExitingFirst;}

		/// this scene is done and wants to start exiting, note: does not start exiting
		inline void done()              {_bDone = true;}

		/// does this scene want to exit?
		inline bool isDone()            {return _bDone;}
		
	/// \section Util
		
		/// run/pause
		inline void run(bool yesno) {_bRunning = yesno;}
		inline void runToggle()     {_bRunning = !_bRunning;}

		/// get/set the scenes name
		inline void setName(std::string name)   {_name = name;}
		inline std::string getName()            {return _name;}

		/// is the scene currently running?
		inline bool isRunning() {return _bRunning;}

		/// is the scene already setup?
		inline bool isSetup()   {return _bSetup;}
		
		/// controls whether the scene's setup function
		/// is called only once or each time a scene change is made
		inline void setSingleSetup(bool single) {_bSingleSetup = single;}
		inline bool usingSingleSetup()          {return _bSingleSetup;}
		
	private:
	
		std::string _name; ///< the name of this scene
		bool _bSetup, _bRunning, _bEntering, _bEnteringFirst,
			 _bExiting, _bExitingFirst, _bDone, _bSingleSetup;

	public:
	
		/// wrapper used to handle ofxScene magic behind the scenes ...
		/// do not use directly!
		class RunnerScene :

		#ifdef TARGET_OF_IOS
			public ofxiOSApp {
		#else
			public ofBaseApp {
		#endif

			public:

				RunnerScene(ofxScene *scene);
				~RunnerScene();

				// need these for proper entering/exit, etc
				void setup();
				void update();
				void draw();
				void exit();
				
				ofxScene *scene;
		};
		
		friend class RunnerScene; //< used to wrap this app
};

///	\class  FadeScene
///	\brief  application scene abstract class with calculated alpha fade
///
/// note: this only automates calculating a normalized alpha value, it's up to
///       you to use this in your drawing code and call ofEnableAlphaBlending()
class ofxFadeScene : public ofxScene {

	public:
	
		ofxFadeScene(const std::string &name) : ofxScene(name) {
			fadeIn = 0;
			fadeOut = 0;
			fadeTimer.set();
			alpha = 1.0;
		}
	
		/// calculates fade in
		void updateEnter() {
			if(isEnteringFirst()) {
				fadeTimer.setAlarm(fadeIn*0.5);
				alpha = 0.0;
			}
			alpha = fadeTimer.getDiffN();
			update();
			if(fadeTimer.alarm()) {
				finishedEntering();
				alpha = 1.0;
			}
		}
	
		/// calculates fade out
		void updateExit() {
			if(isExitingFirst()) {
				fadeTimer.setAlarm(fadeOut*0.5);
				alpha = 1.0;
			}
			alpha = abs(fadeTimer.getDiffN()-1.0);
			update();
			if(fadeTimer.alarm()) {
				finishedExiting();
				alpha = 0.0;
			}
		}
	
		/// set transition fade in and out times (ms)
		void setFade(unsigned int fadeInMS, unsigned int fadeOutMS) {
			fadeIn = fadeInMS;
			fadeOut = fadeOutMS;
		}
		void setFadeIn(unsigned int fadeInMS)  {fadeIn = fadeInMS;}
		void setFadeOut(unsigned int fadeOutMS) {fadeOut = fadeOutMS;}
	
		/// get transition fade in time (ms)
		unsigned int getFadeIn() {return fadeIn;}
	
		/// get transition fade out time (ms)
		unsigned int getFadeOut() {return fadeOut;}

	protected:
	
		unsigned int fadeIn;  //< scene transition fade in (ms), default: 0
		unsigned int fadeOut; //< scene transition fade out (ms), default: 0
	
		ofxTimer fadeTimer; //< for calculating transition fade
		float alpha; //< calculated fade alpha value, normalized 0-1.0
};
