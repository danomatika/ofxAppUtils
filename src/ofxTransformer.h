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

#include "ofMatrix4x4.h"
#include "ofxQuadWarper.h"

///	\class  Transformer
///	\brief  a render transformer
class ofxTransformer {
	public:
	
		/// creates render area of size 1x1, make sure to call setRenderSize()
		ofxTransformer();
	
		/// clear all current transform settings and set render size to screen size
		void clearTransforms();
	
	/// \section Render Size
	
		/// set the desired render size compared to the given screen (or window) size,
		/// set handleAspectRatio to true if you want automatic letter/pillar boxing
		void setRenderSize(float w, float h, float screenWidth, float screenHeight);
		void setRenderSize(float w, float h);	/// use the full screen size
		float getRenderWidth();
		float getRenderHeight();
		float getScreenWidth();
		float getScreenHeight();
	
		/// resize the render scaling to a new parent screen size
		void setNewScreenSize(float screenWidth, float screenHeight);
	
	/// \section Apply Transforms
	
		/// apply the transforms types manually,
		/// these do not do a matrix push or pop
		void applyRenderScale();
		void applyMirrorX();
		void applyMirrorY();
		void applyTranslate();
		void pushWarp(); //< push the quad warper
		void popWarp();  //< pop the quad warper
	
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
		/// set forceWarp to true to ignore the setWarp setting (needed internally)
		///
		void push(bool forceWarp=false);
		void pop();
		bool isPushed(); //< have the transforms been pushed?
	
	/// \section Settings
	
		/// enable/disable the transforms
		///
		/// set translate if you want to be able to move the origin
		/// set scale if you want to turn on automatic scaling to the screen size
		/// set warp if you want to enable to quad warper
		/// set aspect to true if you want automatic letter/pillar boxing
		///
		/// note: all of these options can also be enabled individually
		///
		void setTransforms(bool translate, bool scale, bool warp=false,
		                   bool aspect=false, bool center=false);
		
		/// set the render scale directly
		void setRenderScale(float x, float y);
		float getRenderScaleX();
		float getRenderScaleY();
		
		/// turn on/off automatic screen scaling
		void setScale(bool scale);
		bool getScale();
		
		/// keep the aspect ratio when scaling?
		void setAspect(bool aspect);
		bool getAspect();
		
		/// center within the parent screen area?
		/// only performed if getApsect() = true & the quad warper is disabled
		///
		/// note: does an origin translation when on
		///
		virtual void setCentering(bool center);
		bool getCentering();
		
		/// render mirroring
		virtual void setMirror(bool mirrorX, bool mirrorY);
		virtual void setMirrorX(bool mirrorX);
		virtual void setMirrorY(bool mirrorY);
		bool getMirrorX();
		bool getMirrorY();
		
		/// origin translation
		void setTranslate(bool translate);
		bool getTranslate();
		
		/// set the origin position
		void setPosition(float x, float y, float z=0);
		void setPosition(const ofPoint &point);
		const ofPoint& getPosition();
	
	/// \section Quad Warper
	
		/// set/edit the quad projection warping
		void setWarp(bool warp);
		bool getWarp();
		void resetWarp();
		bool loadWarpSettings(const std::string &xmlFile="quadWarper.xml");
		void saveWarpSettings(const std::string &xmlFile="quadWarper.xml");
	
		/// set/edit the projection warping
		void setEditWarp(bool edit);
		bool getEditWarp();
	
		/// manually set/get the warp points
		/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
		void setWarpPoint(unsigned int index, const ofVec2f &point);
		const ofVec2f& getWarpPoint(unsigned int index);
	
		/// draw projection warping bounding box
		void drawWarpBounds();
	
		/// draw the quad warper editor if in edit mode
		void drawWarpEditor();
	
		/// quad warper editor input callbacks
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
	
	protected:
	
		bool _bScale, _bMirrorX, _bMirrorY, _bTranslate, _bAspect, _bCenter, _bWarp;
		float _screenWidth, _screenHeight;  //< parent render size (screen or window)
		float _renderWidth, _renderHeight;  //< render size
		float _renderScaleX, _renderScaleY; //< render scale
		float _renderAspect, _screenAspect; //< computed aspect ratios (w/h)
		ofPoint _position; //< origin position
	
		ofxQuadWarper _quadWarper;
		bool _bEditingWarpPoints;   //< are we currently editing the warp points?
		int _currentWarpPoint;      //< currently selected projection point
		
		bool _bTransformsPushed; //< have the transforms been pushed?
};
