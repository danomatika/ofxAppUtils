#pragma once

#include <ofMain.h>

#include "ofxApp.h"
#include "ofxTimer.h"

/**
    \class  Scene
    \brief  application scene abstract class
**/
class ofxScene : public ofBaseApp {
	
	public:
		
		ofxScene(ofxApp& app, std::string name) :
			ofBaseApp(), app(app), mouseX(app.mouseX), mouseY(app.mouseY), 
			_name(name), _bSetup(false), _bRunning(true),
            _bEntering(false), _bEnteringFirst(false),
			_bExiting(false), _bExitingFirst(false),
            _bDone(false) {}
		virtual ~ofxScene() {}
		
		// \section Main

		/// functions imeplemented by this class,
		/// use the protected callbacks instead
		void setup();
		void update();
		void draw();
		void exit();
		
		// input callbacks
		virtual void keyPressed(int key) {}
		virtual void mouseMoved(int x, int y) {}
		virtual void mouseDragged(int x, int y, int button) {}
		virtual void mousePressed(int x, int y, int button) {}
		virtual void mouseReleased(int x, int y, int button) {}
		
		// \section Transitional Control

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
        inline void done()      		{_bDone = true;}

        /// does this scene want to exit?
        inline bool isDone()    		{return _bDone;}
		
		/// \section Util
		
		/// run/pause
        inline void run(bool yesno) {_bRunning = yesno;}
        inline void runToggle()     {_bRunning = !_bRunning;}

        /// get/set the scenes name
        inline void setName(std::string& name)   {_name = name;}
        inline std::string getName()            {return _name;}

        /// is the scene currently running?
        inline bool isRunning() {return _bRunning;}

        /// is the scene already setup?
        inline bool isSetup()   {return _bSetup;}

	protected:

		/// \section Callbacks to Implement
		
		/// has an OpenGL context, set GL parameters, load textures etc here
        virtual void setupScene() {};

        /// called when entering
        virtual void updateSceneEnter() {finishedEntering();};

        /// called when running
        virtual void updateScene() = 0;

        /// called when exiting
        virtual void updateSceneExit()	{finishedExiting();};

        /// called when drawing
        virtual void drawScene() = 0;
		
		/// called before being deleted, cleanup resources here
		virtual void cleanupScene() {};
		
		ofxApp& app;	///< the parent application reference
		
		/// for processing heads
		int& mouseX;
		int& mouseY;

	private:
	
		std::string _name;		///< the name of this scene
		bool _bSetup, _bRunning, _bEntering, _bEnteringFirst,
			 _bExiting, _bExitingFirst, _bDone;
};
