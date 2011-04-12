#pragma once

#include <ofMain.h>

#include "ofxQuadWarper.h"

/**
    \class  Transformer
    \brief  a render transformer
**/
class ofxTransformer{

	public:
	
		ofxTransformer();
		
		/// set the desired render size compared to the window size,
		/// set handleAspectRatio to true if you want automatic letter/pillar boxing
		/// set translate if you want to move the origin
		/// set useWarp if you want to enable to quad warper
		void setRenderSize(float w, float h, bool handleAspectRatio=true,
						   bool translate=true, bool useWarp=false);
		float getRenderWidth()	{return _renderWidth;}
		float getRenderHeight()	{return _renderHeight;}
		
		/// set the render scale directly
		void setRenderScale(float x, float y);
		float getRenderScaleX()	{return _renderScaleX;}
		float getRenderScaleY()	{return _renderScaleY;}
		
		/// turn on/off automatic screen scaling
		virtual void setScale(bool scale)	{_bScale = scale;}
		bool getScale() 					{return _bScale;}
		
		/// screen mirroring
		virtual void setMirror(bool mirrorX, bool mirrorY);
		virtual void setMirrorX(bool mirrorX);
		virtual void setMirrorY(bool mirrorY);
		bool getMirrorX()		{return _bMirrorX;}
		bool getMirrorY()		{return _bMirrorY;}
		
		/// origin translation
		virtual void setOriginTranslate(bool translate)	{_bTranslate = translate;}
		bool getOriginTranslate()						{return _bTranslate;}
		
		/// set the origin position
		virtual void setOrigin(float x, float y, float z=0);
		virtual void setOrigin(ofVec2f point)	{setOrigin(point.x, point.y, 0);}
		virtual void setOrigin(ofVec3f point)	{setOrigin(point.x, point.y, point.z);}
		ofVec3f getOrigin()		{return _origin;}
		float getOriginX()		{return _origin.x;}
		float getOriginY()		{return _origin.y;}
		float getOriginZ()		{return _origin.z;}
		
		/// set/edit the quad projection warping
		virtual void setWarp(bool warp);
		bool getWarp()				{return _bWarp;}
		void resetWarp();
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
				
	protected:
		
		bool _bScale, _bMirrorX, _bMirrorY, _bTranslate, _bHandleAspect, _bWarp;
		float _renderWidth, _renderHeight;
		float _renderScaleX, _renderScaleY;
		float _renderAspect, _screenAspect;
		ofVec3f _origin;
		
		/// quad warper
		ofxQuadWarper _quadWarper;
};
