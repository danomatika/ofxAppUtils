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

#include "ofConstants.h"
#include "ofColor.h"
#ifdef TARGET_OF_IOS
	#include "ofxiOS.h"
#else
	#include "ofBaseApp.h"
#endif

#include "ofxQuadWarper.h"
#include "ofxTransformer.h"
#include "ofxTimer.h"

class ofxSceneManager;


///	\class  App
///	\brief  application implementation with automatic transforms and projection mapping,
///			includes an optional built-in ofxControlPanel
///
///	simply inherit from this class and implement the regular app callbacks
///	ie setup(), update(), draw(), etc
class ofxApp :

#ifdef TARGET_OF_IOS
	public ofxiOSApp {
#else
	public ofBaseApp {
#endif

	public:
	
		ofxApp();
		virtual ~ofxApp() {}
		
	/// \section Transformer
		
		/// set the transformer, this object is never deleted
		void setTransformer(ofxTransformer *transformer);
		
		/// get the current transformer or NULL if not set
		ofxTransformer* getTransformer();
		
		/// clear the current transformer
		void clearTransformer();
		
		/// get app render width based on current transformer,
		/// returns ofGetWidth() if transformer is not set
		float getRenderWidth();
		
		/// get app render height based on current transformer,
		/// returns ofGetHeight() if transformer is not set
		float getRenderHeight();
		
		/// enable/disable automatically applying the transforms
		///
		/// enable/disable individual transforms using transformer setTransforms
		/// of the specific ofxTransform setter functions (setScale, setTranslate,
		/// etc)
		///
		/// this is done automatically before the scene and app draw functions
		/// are called, see transformer push/pop functions if you need to drop
		/// back to screen space
		///
		/// to manually push/pop the transforms, use ofxTransformer push()/pop():
		///
		/// use these if you need to jump out of the auto transform and do
		/// something in the default screen space
		///
		/// note: the transforms can only be pushed/popped in a pair, multiple
		///		  subsequent calls to push or pop will be ignored
		///
		/// note: this is done automatically before draw is called, it will do 
		///       nothing if you've disabled all of the individual transform
		///       options (origin translation, scaling, etc)
		///
		void setAutoTransforms(bool apply) {_bAutoTransforms = apply;}
		bool getAutoTransforms()           {return _bAutoTransforms;}

	/// \section SceneManager
	
		/// set the built in SceneManager (optional)
		///
		/// the scene manager's update, draw, and input callback functions are
		/// called automatically which, in turn, calls those of the current scene
		///
		void setSceneManager(ofxSceneManager *manager);
		ofxSceneManager* getSceneManager();
		void clearSceneManager();
		
		/// enable/disable automatic scene manager update and/or draw functions
		/// (on by default)
		///
		/// disable these in order to set your own update/draw order,
		/// simply call sceneManager.update() and/or sceneManager.draw()
		/// manually
		void setSceneManagerUpdate(bool update) {_bSceneManagerUpdate = update;}
		bool getSceneManagerUpdate()            {return _bSceneManagerUpdate;} 
		void setSceneManagerDraw(bool draw)     {_bSceneManagerDraw = draw;}
		bool getSceneManagerDraw()              {return _bSceneManagerDraw;}

	/// \section Drawing the Framerate (as text, default lower right corner)

		/// draw the framerate automatically in debug mode? (on by default)
		virtual void setDrawFramerate(bool draw) {_bDrawFramerate = draw;}
		bool getDrawFramerate()                  {return _bDrawFramerate;}
		
		/// reference to the framerate text color
		ofColor& getFramerateColor() {return _framerateColor;}
		
		/// draw the framerate text manually
		void drawFramerate(float x, float y);
	
	/// \section Util
			
		/// is debug mode on? (show fps, allow editing of warper, draw custom debug info),
		/// you can also use bDebug directly in your inherited ofxApp
		inline bool isDebug() {return bDebug;}
		inline void setDebug(bool debug) {bDebug = debug;}

	protected:
	
		bool bDebug; //< are we in debug mode? default: false

	private:
		
		ofxTransformer *_transformer; //< optional built in transformer
		bool _bAutoTransforms; //< apply the transforms automatically? default: true
		
		bool _bDrawFramerate; //< draw the dramerate in debug mode? default: true
		ofColor _framerateColor; //< framerate text color
		
		ofxSceneManager *_sceneManager; //< optional built in scene manager
		bool _bSceneManagerUpdate; //< call scene manager update automatically?
		bool _bSceneManagerDraw; //< call scene manager draw automatically?

	public:

		/// wrapper used to handle ofxApp magic behind the scenes ...
		/// do not use directly!
		class RunnerApp :

		#ifdef TARGET_OF_IOS
			public ofxiOSApp {
		#else
			public ofBaseApp {
		#endif
			
			public:

				RunnerApp(ofxApp *app);
				~RunnerApp();

				// ofBaseApp callbacks
				void setup();
				void update();
				void draw();
				void exit();
				
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
				// ofxIOS callbacks
				void lostFocus();
				void gotFocus();
				void gotMemoryWarning();
				void deviceOrientationChanged(int newOrientation);
			#endif
				
				// ofBaseSoundInput callbacks
				void audioIn(ofSoundBuffer& buffer);
				void audioIn(float *input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
				void audioIn(float *input, int bufferSize, int nChannels );
				void audioReceived(float *input, int bufferSize, int nChannels);
				
				// ofBaseSoundOutput callbacks
				void audioOut(ofSoundBuffer& buffer);
				void audioOut(float *output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
				void audioOut(float *output, int bufferSize, int nChannels);
				void audioRequested(float *output, int bufferSize, int nChannels);
				
				ofxApp* getAppPtr() {return app;}
				
			private:
			
				ofxApp *app;
		};
		
		friend class RunnerApp; ///< used to wrap this app
};
