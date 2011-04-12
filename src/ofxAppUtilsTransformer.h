#pragma once

#include <ofMain.h>
#include <ofxControlPanel.h>
#include <ofxLogger.h>

#include "ofxAppUtilsTimer.h"

/**
    \class  App
    \brief  application implementation with automatic transforms and projection mapping
			includes a built in ofxControlPanel
**/
class ofxAppUtilsApp : public ofBaseApp {

	public:
	
		ofxAppUtilsApp();
		virtual ~ofxAppUtilsApp() {}
		
		/// set the desired render size compared to the window size,
		/// set handleAspectRatio to true if you want automatic letter/pillar boxing
		/// set translate if you want to move the origin
		/// set useWarp if you want ot enable to quad warper
		void setRenderSize(float w, float h, bool handleAspectRatio=true,
						   bool translate=true, bool useWarp=false);
		float getRenderWidth()	{return _renderWidth;}
		float getRenderHeight()	{return _renderHeight;}
		
		/// set the render scale directly
		void setRenderScale(float x, float y);
		float getRenderScaleX()	{return _renderScaleX;}
		float getRenderScaleY()	{return _renderScaleY;}
		
		/// turn on/off automatic screen scaling
		void setScale(bool scale)	{_bScale = scale;}
		bool getScale() 			{return _bScale;}
		
		/// screen mirroring
		void setMirror(bool mirrorX, bool mirrorY)	{_bMirrorX = mirrorX; _bMirrorY = mirrorY;}
		void setMirrorX(bool mirrorX)				{_bMirrorX = mirrorX;}
		void setMirrorY(bool mirrorY)				{_bMirrorY = mirrorY;}
		bool getMirrorX()		{return _bMirrorX;}
		bool getMirrorY()		{return _bMirrorY;}
		
		/// origin translation
		void setOriginTranslate(bool translate)		{_bTranslate = translate;}
		bool getOriginTranslate()					{return _bTranslate;}
		
		/// set the origin position
		void setOrigin(float x, float y, float z=0)	{_origin.set(x, y, z);}
		void setOrigin(ofVec2f point)				{_origin.set(point.x, point.y, 0);}
		void setOrigin(ofVec3f point)				{_origin = point;}
		ofVec3f getOrigin()		{return _origin;}
		float getOriginX()		{return _origin.x;}
		float getOriginY()		{return _origin.y;}
		float getOriginZ()		{return _origin.z;}
		
		/// set/edit the projection warping
		void setWarp(bool warp)		{_bWarp = warp;}
		bool getWarp()				{return _bWarp;}
		void resetWarp();
		void setEditWarp(bool edit) {_bEditingWarpPoints = edit;}
		bool getEditWarp()			{return _bEditingWarpPoints;}
		bool loadWarpSettings(const string xmlFile="quadWarper.xml");
		void saveWarpSettings(const string xmlFile="quadWarper.xml");
		
		/// manually set/get the warp points
		/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
		void setWarpPoint(unsigned int index, ofVec2f point);
		ofVec2f getWarpPoint(unsigned int index);
		
		/// apply the transforms manually,
		/// these do not do a matrix push or pop
		void applyRenderScale();
		void applyMirrorX();
		void applyMirrorY();
		void applyOriginTranslate();
		void applyWarp();
		
		/// is debug mode on?
		bool isDebug()	{return bDebug;}

		/// functions imeplemented by this class,
		/// use the protected callbacks instead
		void update();
		void draw();
		void keyPressed(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

	protected:
	
		/// callbacks to implement
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
		
		bool _bScale, _bMirrorX, _bMirrorY, _bTranslate, _bHandleAspect, _bWarp;
		float _renderWidth, _renderHeight;
		float _renderScaleX, _renderScaleY;
		float _renderAspect, _screenAspect;
		ofVec3f _origin;
		
		// projection warp points
		ofVec2f	_warpPoints[4];
		int _currentWarpPoint;	// currently selected projection point
		
		// projection warping matrices
		double _warpMatrix[3][3];
        GLfloat _glWarpMatrix[16];
		
		// are we currently editing the warp points?
		bool _bEditingWarpPoints;
		ofMatrix4x4 _warpTransform;	// warp transform matrix needed for mouse picking
};
