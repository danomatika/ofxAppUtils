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
#ifdef TARGET_OF_IPHONE
	#include "ofxiPhone.h"
#else
	#include "ofBaseApp.h"
#endif

#ifdef OFX_APP_UTILS_USE_CONTROL_PANEL
	#include "ofxXmlSettings.h"
	#include "ofxControlPanel.h"
#endif

#include "ofxQuadWarper.h"
#include "ofxTransformer.h"
#include "ofxTimer.h"

class ofxSceneManager;

/**
	\class  App
	\brief  application implementation with automatic transforms and projection mapping,
			includes an optional built-in ofxControlPanel
			
	simply inherit from this class and implement the regular app callbacks
	ie setup(), update(), draw(), etc
**/
class ofxApp :

#ifdef TARGET_OF_IPHONE
	public ofxiPhoneApp,
#else
	public ofBaseApp,
#endif
	public ofxTransformer {

	public:
	
		ofxApp();
		virtual ~ofxApp() {}
		
	/// \section Transform Controls
		
		/// screen mirroring
		void setMirror(bool mirrorX, bool mirrorY);
		void setMirrorX(bool mirrorX);
		void setMirrorY(bool mirrorY);
		
		/// set the origin position
		void setOrigin(float x, float y, float z=0);
		
		/// keep the aspect ratio when scaling?
		void setAspect(bool aspect);
		
		/// center within the parent screen area?
		void setCentering(bool center);
		
		/// set/edit the projection warping
		void setWarp(bool warp);
		void setEditWarp(bool edit) {_bEditingWarpPoints = edit;}
		bool getEditWarp()			{return _bEditingWarpPoints;}
		
		/// enable/disable automatically applying the transforms
		///
		/// enable/disable individual transforms using setTransforms or the
		/// specific ofxTransform setter functions (setScale, setOriginTranslate,
		/// etc)
		///
		/// this is done automatically before the scene and app draw functions
		/// are called, see push/popTransforms if you need to drop back to
		/// screen space
		///
		/// warning: the quad warper requires an extra matrix push/pop, do not
		///          change the setWarp within your draw function as an extra
		///          push or pop may ocurr which could muck up your transforms
		///
		void setAutoTransforms(bool apply)	{_bAutoTransforms = apply;}
		bool getAutoTransforms()			{return _bAutoTransforms;}
		
		/// manually push/pop the transforms
		///
		/// use this if you need to jump out of the auto transform and do
		/// something in the default screen space
		///
		/// note: the transforms can only be pushed/popped in a pair, multiple
		///		  subsequent calls to push or pop will be ignored
		///
		/// note: this is done automatically before draw is called, it will do 
		///       nothing if you've disabled all of the individual transform
		///       options (origin translation, scaling, etc)
		///
		/// inherited from ofxTransformer
		///
		/// void pushTransforms();
		/// void popTransforms();
		
#ifdef OFX_APP_UTILS_USE_CONTROL_PANEL

	/// \section Control Panel (optional) 
	///
	/// define OFX_APP_UTILS_USE_CONTROL_PANEL to enable the control panel
	/// (and ofxControlPanel dependency)
	///
		
		/// add transform controls to the ofxControlPanel (optional)
		/// set panelNum to choose which to add the controls to, otherwise a
		/// new panel is added
		///
		/// the controls are (xmlName, type):
		///	- transformPos 				float slider2D
		/// - transformZ 				float slider
		/// - transformMirrorX			bool toggle
		/// - transformMirrorY			bool toggle
		/// - transformEnableQuadWarper	bool toggle
		/// - transformEditQuadWarper	bool toggle (button)
		/// - transformSaveQuadWarper	bool toggle (button)
		///
		void addTransformControls(int panelNum=-1, int panelCol=0);
		
		/// load and save the control panel settings
		void loadControlSettings(const string xmlFile="controlPanelSettings.xml");
		void saveControlSettings(const string xmlFile="controlPanelSettings.xml");
		
		/// draw the control panel automatically in debug mode? (on by default)
		void setDrawControlPanel(bool draw);
		bool getDrawControlPanel();
		
		/// draw the control panel manually
		void drawControlPanel();
#endif

	/// \section Drawing the Framerate (as text, default lower right corner)

		/// draw the framerate automatically in debug mode? (on by default)
		virtual void setDrawFramerate(bool draw)	{_bDrawFramerate = draw;}
		bool getDrawFramerate()						{return _bDrawFramerate;}
		
		/// reference to the framerate text color
		ofColor& getFramerateColorRef()	{return _framerateColor;}
		
		/// draw the framerate text manually
		void drawFramerate(float x, float y);

	/// \section SceneManager
	
		/// set the built in SceneManager (optional)
		///
		/// the scene manager's update, draw, and input callback functions are
		/// called automatically which, in turn, calls those of the current scene
		///
		void setSceneManager(ofxSceneManager* manager);
		ofxSceneManager* getSceneManager();
		void clearSceneManager();
		
		/// enable/disable automatic scene manager update and/or draw functions
		/// (on by default)
		///
		/// disable these in order to set your own update/draw order,
		/// simply call sceneManager.update() and/or sceneManager.draw()
		/// manually
		void setSceneManagerUpdate(bool update)	{_bSceneManagerUpdate = update;}
		bool getSceneManagerUpdate()			{return _bSceneManagerUpdate;} 
		void setSceneManagerDraw(bool draw)		{_bSceneManagerDraw = draw;}
		bool getSceneManagerDraw()				{return _bSceneManagerDraw;} 
	
	/// \section Util
			
		/// is debug mode on? (show control panel and fps, allow editing of warper)
		inline bool isDebug()	{return bDebug;}

	protected:
	
		bool bDebug;	///< are we in debug mode?

#ifdef OFX_APP_UTILS_USE_CONTROL_PANEL
		ofxControlPanel	controlPanel; ///< the settings control panel
#endif

	private:
		
		ofxTransformer _transformer;
		bool _bAutoTransforms;		///< apply the transforms automatically?
		
		/// quad warper
		int _currentWarpPoint;		///< currently selected projection point
		bool _bEditingWarpPoints;	///< are we currently editing the warp points?
		ofMatrix4x4 _warpTransform;	///< warp transform matrix needed for mouse picking
		
		bool _bDrawFramerate;		///< draw the dramerate in debug mode?
		ofColor _framerateColor;	///< framerate text color
		
		ofxSceneManager* _sceneManager;	///< optional built in scene manager
		bool _bSceneManagerUpdate, _bSceneManagerDraw;
		
#ifdef OFX_APP_UTILS_USE_CONTROL_PANEL
		bool _bTransformControls;	///< have the projection controls been added?
		bool _bDrawControlPanel;	///< draw the control panel automatically?
#endif

	public:

		/// wrapper used to handle ofxApp magic behind the scenes ...
		/// do not use directly!
		class RunnerApp :

		#ifdef TARGET_OF_IPHONE
			public ofxiPhoneApp {
		#else
			public ofBaseApp {
		#endif
			
			public:

				RunnerApp(ofxApp* app);
				~RunnerApp();

				// ofBaseApp callbacks
				void setup();
				void update();
				void draw();
				void exit();

				void keyPressed(int key);
				void keyReleased(int key);

				void mouseMoved(int x, int y);
				void mouseDragged(int x, int y, int button);
				void mousePressed(int x, int y, int button);
				void mouseReleased();
				void mouseReleased(int x, int y, int button);
				
				void windowResized(int w, int h);
				void dragEvent(ofDragInfo dragInfo);
				void gotMessage(ofMessage msg);
				
			#ifdef TARGET_OF_IPHONE
				// ofxIphone callbacks
				void touchDown(ofTouchEventArgs & touch);
				void touchMoved(ofTouchEventArgs & touch);
				void touchUp(ofTouchEventArgs & touch);
				void touchDoubleTap(ofTouchEventArgs & touch);
				void touchCancelled(ofTouchEventArgs & touch);

				void lostFocus();
				void gotFocus();
				void gotMemoryWarning();
				void deviceOrientationChanged(int newOrientation);
			#endif
				
				// ofBaseSoundInput callbacks
				void audioIn(float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
				void audioIn(float * input, int bufferSize, int nChannels );
				void audioReceived(float * input, int bufferSize, int nChannels);
				
				// ofBaseSoundOutput callbacks
				void audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
				void audioOut(float * output, int bufferSize, int nChannels);
				void audioRequested(float * output, int bufferSize, int nChannels);
				
				ofxApp* getAppPtr() {return app;}
				
			private:
			
				ofxApp* app;
		};
		
		friend class RunnerApp;  ///< used to wrap this app
};
