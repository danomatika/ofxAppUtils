#pragma once

#include <ofMain.h>

#ifndef OFX_APP_UTILS_NO_CONTROL_PANEL
	#include "ofxXmlSettings.h"
	#include "ofxControlPanel.h"
#endif

#include "ofxQuadWarper.h"
#include "ofxTransformer.h"
#include "ofxTimer.h"

class ofxSceneManager;
class ofxRunnerApp;

/**
    \class  App
    \brief  application implementation with automatic transforms and projection mapping,
			includes a built in ofxControlPanel
            
    simply inherit from this class and implement the regular ofBaseApp callbacks
    ie setup(), update(), draw(), etc
**/
class ofxApp : public ofBaseApp, public ofxTransformer {

	public:
	
		ofxApp();
		virtual ~ofxApp() {}
        
		/// screen mirroring
		void setMirror(bool mirrorX, bool mirrorY);
		void setMirrorX(bool mirrorX);
		void setMirrorY(bool mirrorY);
		
		/// set the origin position
		void setOrigin(float x, float y, float z=0);
		
		/// keep the aspect ration when scaling?
		void setAspect(bool aspect);
		
		/// center within the parent screen area?
		void setCentering(bool center);
		
		/// set/edit the projection warping
		void setWarp(bool warp);
		void setEditWarp(bool edit) {_bEditingWarpPoints = edit;}
		bool getEditWarp()			{return _bEditingWarpPoints;}
		
#ifndef OFX_APP_UTILS_NO_CONTROL_PANEL
		///
		/// define OFX_APP_UTILS_NO_CONTROL_PANEL to disable the control panel
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
		
		/// draw the control panel automatically in debug mode? (on by default)
		void setDrawControlPanel(bool draw);
		bool getDrawControlPanel();
		
		/// draw the control panel manually
		void drawControlPanel();
#endif

		/// draw the framerate automatically in debug mode? (on by default)
		virtual void setDrawFramerate(bool draw)	{_bDrawFramerate = draw;}
		bool getDrawFramerate()						{return _bDrawFramerate;}
		
		/// reference to the framerate text color
		ofColor& getFramerateColorRef()	{return _framerateColor;}
		
		/// draw the framerate text manually
		void drawFramerate(float x, float y);

		/// set the built in SceneManager (optional)
		void setSceneManager(ofxSceneManager* manager);
		ofxSceneManager* getSceneManager();
		void clearSceneManager();
		
		/// is debug mode on? (show control panel and fps)
		inline bool isDebug()	{return bDebug;}
        
        friend class ofxRunnerApp;  ///< used to wrap this app

	protected:
	
		bool bDebug;	///< are we in debug mode?

#ifndef OFX_APP_UTILS_NO_CONTROL_PANEL
		ofxControlPanel	controlPanel; ///< the settings control panel
#endif

	private:
		
		ofxTransformer _transformer;
		
		/// quad warper
		int _currentWarpPoint;		///< currently selected projection point
		bool _bEditingWarpPoints;	///< are we currently editing the warp points?
		ofMatrix4x4 _warpTransform;	///< warp transform matrix needed for mouse picking
		
		bool _bDrawFramerate;		///< draw the dramerate in debug mode?
		ofColor _framerateColor;	///< framerate text color
		
		ofxSceneManager* _sceneManager;	///< optional built in scene manager
		
#ifndef OFX_APP_UTILS_NO_CONTROL_PANEL
		bool _bTransformControls;	///< have the projection controls been added?
		bool _bDrawControlPanel;	///< draw the control panel automatically?
#endif
};

/// wrapper used to handle ofxApp magic behind the scenes ...
/// do not use directly!
class ofxRunnerApp : public ofBaseApp {

    public:

        ofxRunnerApp(ofxApp* app) : ofBaseApp() {
            this->app = app;
        }
        ~ofxRunnerApp() {
            delete app;
        }

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
		void mouseReleased();
		void mouseReleased(int x, int y, int button);
		
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);	
        
        // ofBaseSoundInput callbacks
        void audioIn(float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
		void audioIn(float * input, int bufferSize, int nChannels );
		void audioReceived(float * input, int bufferSize, int nChannels);
        
        // ofBaseSoundOutput callbacks
        void audioOut(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount);
		void audioOut(float * output, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);
        
        ofxApp* app;
};
