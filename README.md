ofxAppUtils
===========
<p align="center">
<img src="https://raw.github.com/danomatika/ofxAppUtils/master/res/logo.png"/>
</p>

OpenFrameworks app utilities

Copyright (c) [Dan Wilcox](danomatika.com) 2011-2013

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See Documentation on [Github](https://github.com/danomatika/ofxAppUtils) and the [OpenFrameworks Forum post](http://forum.openframeworks.cc/index.php/topic,5995.0.html).

OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

Description
-----------

ofxAppUtils is a set of utilities for OpenFrameworks application development including:

* ofxApp: an ofBaseApp/ofxiPhoneApp extension with built in screen scaling, projection mapping transforms, quad warping, and an optional ofxControlPanel
* ofxScene: a mini ofBaseApp/ofxiPhoneApp for writing stand alone scenes
* ofxSceneManager: handles a list of scenes using a std::map
* ofxTransformer: open gl transformer for origin translation, screen scaling, mirroring, and quad warping
* ofxQuadWarper: an open gl matrix quad warper (useful for oblique projection mapping)
* ofxTimer: a simple millis-based timer
* ofxMovingAverage: a simple linear moving average
* ofxParticle: a simple time-based particle base class
* ofxParticleSystem: an auto manager for ofxParticles
* ofxBitmapString: a stream interface for ofDrawBitmapString

All ofBaseApp & ofxiOSApp callbacks are handled down to the scene level.

Installation
------------

To use ofxAppUtils, first you need to download and install Open Frameworks. ofxAppUtils is currently developed against the latest stable version of Open Frameworks on github.

To get a copy of the repository you can download the source from [http://github.com/danomatika/ofxAppUtils/zipball/master](http://github.com/danomatika/ofxAppUtils/zipball/master) or, alternatively, you can use git clone:
<pre>
git clone git://github.com/danomatika/ofxAppUtils.git
</pre>

The addon should sit in `openFrameworks/addons/ofxAppUtils/`.

### Dependencies

You will may also need the following addon dependencies:

* ofxControlPanel: https://github.com/kylemcdonald/ofxControlPanel (optional, also requires ofxXmlSettings)

To enable the control panel (and ofxControlPanel dependency), define `OFX_APP_UTILS_USE_CONTROL_PANEL` in your C++ FLAGS.

Running the Example Project
-------------------------------

The example projects are in the `appUtilsExample` & `appUtilsIOSExample` folders.

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the OpenFrameworks distribution.

Point the ProjectGenerator to `addons/ofxAppUtils`, change the project name to the **exact** name of the example you want to generate (ie `appUtilsExample`), and make sure to choose `ofxAppUtils` (& `ofxControlPanel` for appUtilsExample) from the addons. Hitting "Generate Project" will populate that example with the project files you will need to build it.

### OSX

Open the Xcode project, select the appUtilsExample-Debug scheme, and hit "Run".

### Linux

Open the Code::Blocks .cbp and hit F9 to build. Optionally, you can build the example with the Makefile.

To build and run it on the terminal:
<pre>
make
make run
</pre>

### Windows

An example Visual Studio solution as well as a Codeblocks workspace are included.


Adding ofxAppUtils to an Existing Project
---------------------------------------

### ProjectGenerator

Select ofxAppUtils and other addons used by your project from the available addons in the ProjectGenerator, select the parent folder of your project, and set the *exact* name of the existing project in the text box. This will overwrite the existing project files with new ones that now include ofxAppUtils.

*Note: you will lose any custom settings you've added manually to your project.*

### Manual Method

_Note: These instructions are for manually adding ofxAppUtils to an existing project. You do not need to follow these steps if you use the ProjecGenerator app, except for adding the OFX_APP_UTILS defines._

If you want to add ofxAppUtils to another project, you need to make sure you include the src folders:
<pre>
openFrameworks/addons/ofxAppUtils/src
</pre>

### Xcode

* create a new group "ofxAppUtils"
* drag these directories from ofxAppUtils into this new group: ofxAppUtils/src
* you also need to add the following addon dependencies (if you're using them) in a similar manner:
	* ofxControlPanel
* make sure to remove any of the example folders in the Xcode project tree for any of the addons you've added manually
* add defines to the project C++FLAGS if you want to disable the control panel and/or xml saving: 
	* Select the Project -> Build Settings -> Other C++ Flags
	* add the following to enable the control panel:
	<pre>
	-DOFX_APP_UTILS_USE_CONTROL_PANEL
	</pre>
	
On iOS you will probably don't want to enable the control panel via the `OFX_APP_UTILS_USE_CONTROL_PANEL` define as it doesn't really work with touch screens.

### For Linux (Makefiles & Codeblocks):

* edit addons.make in your project folder and add the following line to the end of the file: 
	<pre>ofxAppUtils</pre>
* edit config.make in your project folder and update the USER_CFLAGS line:
  *  if you want to enable the control panel:
	<pre>
	USER_CFLAGS = -DOFX_APP_UTILS_USE_CONTROL_PANEL
	</pre>
	
### For Codeblocks (Win):

* add the ofxAppUtils sources to the project:
	* right-click on your project in the project tree
	* select "Add Files Recursively ..."
	* navigate and choose the ofxAppUtils/src folder
* add defines, search paths, and libraries to link:
	* right-click on your project in the project tree
	* select "Build options..."
	* make sure the project name is selected in the tree (not release or debug)
	* select the "Compiler settings" tab
	  * add the following to the "#defines" tab if you want to enable the control panel:
	<pre>
	OFX_APP_UTILS_USE_CONTROL_PANEL
	</pre>
	* select the "Search directories" tab, click add the search paths:
	<pre>
	..\\..\\..\addons\ofxAppUtils\src
	</pre>
	
### For Visual Studio:

* drag the ofxApputils/src folder onto the project tree
* right click on the project in the project tree and select Properties
* set the Configuration to All Configurations
* add the defines and searhc paths:
  * add the following to Configuration Properties->C/C++->Preprocessor->Preprocessor Definitions:
    * enable the control panel:
	<pre>
	OFX_APP_UTILS_USE_CONTROL_PANEL
	</pre>
  * add to the search paths in Configuration Properties->C/C++->General->Additonal Include DIrectories:
  <pre>
  ..\\..\\..\\addons\ofxAppUtils\src
  </pre>

### Extending Your BaseApp

There are two changes you need to make to extend your app with the ofApputils ofxApp class which handles all the *automagic*:

* your base app needs to inherit from ofxApp in your testApp.h
<pre>
class testApp : public ofBaseApp { 
// becomes
class testApp : public ofxApp { 
</pre>
* you need to run your app with the app utils in main.cpp
<pre>
ofRunApp(new testApp());
// becomes
ofRunAppWithAppUtils(new testApp());
</pre>

That's it. Check the example testApp::setup() for more info on startup settings.

Note: this is completely optional. You can always handle the transforms etc yourself using an ofxTransformer instance, etc.

#### ofxGetAppPtr

Internally, calling `ofRunAppWithAppUtils` actually creates a wrapper ofBaseApp which then runs the given ofxApp. As a result, **calling `ofGetAppPtr()` does not work correctly** as it returns a pointer to the wrapper and not the app itself.

In order to fix this, there is a new function added with ofxAppUtils called `ofxGetAppPtr()` (note the "x") which returns the correct pointer. If you are porting existing code, make sure to change any `ofGetAppPtr()` calls to `ofxGetAppPtr()`.

DEVELOPING
----------

You can help develop ofxAppUtils on GitHub: [https://github.com/danomatika/ofxAppUtils](https://github.com/danomatika/ofxAppUtils)

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.

TODO
----

* removeal ofxControlPanel dependency, replace with ofxGui
