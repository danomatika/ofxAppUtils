#pragma once

#include <ofMain.h>
#include <ofxControlPanel.h>
#include <ofxLogger.h>

#include "ofxQuadWarper.h"
#include "ofxTransformer.h"
#include "ofxTimer.h"

class ofxSceneManager;

/**
    \class  App
    \brief  application implementation with automatic transforms and projection mapping,
			includes a built in ofxControlPanel
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
		
		/// set/edit the projection warping
		void setWarp(bool warp);
		void setEditWarp(bool edit) {_bEditingWarpPoints = edit;}
		bool getEditWarp()			{return _bEditingWarpPoints;}
		
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
		
		/// set the built in SceneManager (optional)
		void setSceneManager(ofxSceneManager* manager);
		ofxSceneManager* getSceneManager();
		void clearSceneManager();
		
		/// is debug mode on?
		bool isDebug()	{return bDebug;}

		/// functions imeplemented by this class,
		/// use the protected callbacks instead
		void setup();
		void exit();
		void update();
		void draw();
		void keyPressed(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

	protected:
	
		/// callbacks to implement
		virtual void setupApp() = 0;
		virtual void exitApp() {}
		virtual void updateApp() = 0;
		virtual void drawApp() = 0;
		virtual void keyPressedApp(int key) = 0;
		virtual void mouseMovedApp(int x, int y) = 0;
		virtual void mouseDraggedApp(int x, int y, int button) = 0;
		virtual void mousePressedApp(int x, int y, int button) = 0;
		virtual void mouseReleasedApp(int x, int y, int button) = 0;
	
		bool bDebug;	///< are we in debug mode?
		
		ofxControlPanel	controlPanel; ///< the settings control panel
		
	private:
		
		/// setup the control panel
		void _setupControlPanel();
		
		ofxTransformer _transformer;
		
		/// quad warper
		int _currentWarpPoint;		// currently selected projection point
		bool _bEditingWarpPoints;	// are we currently editing the warp points?
		ofMatrix4x4 _warpTransform;	// warp transform matrix needed for mouse picking
		
		bool _bTransformControls; ///< have the projeciton controls been added?
		
		ofxSceneManager* _sceneManager;	///< optional buitl in scene manager
};
