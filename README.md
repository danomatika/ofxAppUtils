ofxAppUtils
===========
<p align="center">
  <img src="https://raw.github.com/danomatika/ofxAppUtils/master/res/logo.png"/>
</p>

openFrameworks app utilities

Copyright (c) [Dan Wilcox](danomatika.com) 2011-2023

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See Documentation on [Github](https://github.com/danomatika/ofxAppUtils) and the [openFrameworks Forum post](https://forum.openframeworks.cc/t/ofxapputils/5995).

openFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

This project has been supported by the CMU [Frank-Ratchey STUDIO for Creative Inquiry](http://studioforcreativeinquiry.org) and the DU [Emergent Digital Practices Program](https://www.du.edu/ahss/edp/).

Description
-----------

ofxAppUtils is a set of utilities for openFrameworks application development including:

* ofxApp: an ofBaseApp/ofxiPhoneApp extension with optional screen scaling transformer & scene manager
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

### Which version to use?

The master branch of ofxAppUtils will work with the current stable version of openFrameworks and can be considered *relatively* stable.

Previous versions are tagged using [Semantic Versioning](http://semver.org) with the updates to newer versions of openFrameworks noted in the changelog, CHANGES.txt. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

If you want to use ofxAppUtils with a previous version of openFrameworks, checkout the corresponding version tag after cloning:

    git clone git://github.com/danomatika/ofxAppUtils.git
    cd ofxAppUtils
    git checkout 0.5.0

### Dependencies

You may also need the following addon dependencies:

* ofxGui: included with openFrameworks

To enable the ofxGui transformer panel, include `ofxTransformerPanel.h`, create an "ofxTransformerPanel" object, and set the transformer in _ofxTransformerPanel::setup()_.

Running the Example Projects
----------------------------

The example projects are in the `appUtilsExample` & `appUtilsIOSExample` folders.

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the openFrameworks distribution.

To (re)generate project files for an *existing* project:

* Click the "Import" button in the ProjectGenerator
* Navigate to the project's parent folder ie. "ofxAppUtils", select the base folder for the example project ie. "appUtilsExample", and click the Open button
* Click the "Update" button

If everything went Ok, you should now be able to open the generated project and build/run the example.

### macOS

Open the Xcode project, select the appUtilsExample-Debug scheme, and hit "Run".

### Linux

Open the QT Creator project file and/or build the example with the Makefile.

To build and run via Makefile on the terminal:
~~~
make
make run
~~~

### Windows

Open the Visual Studio project, build and run.

Adding ofxAppUtils to an Existing Project
-----------------------------------------

### ProjectGenerator

Select ofxAppUtils and other addons used by your project from the available addons in the ProjectGenerator, select the parent folder of your project, and set the *exact* name of the existing project in the text box. This will overwrite the existing project files with new ones that now include ofxAppUtils.

*Note: you will lose any custom settings you've added manually to your project.*

### Manual Method

_Note: These instructions are for manually adding ofxAppUtils to an existing project. You do not need to follow these steps if you use the ProjecGenerator app, except for adding the OFX_APP_UTILS defines._

If you want to add ofxAppUtils to another project, you need to make sure you include the src folders:
~~~
openFrameworks/addons/ofxAppUtils/src
~~~

### Xcode

* Create a new group "ofxAppUtils"
* Drag these directories from ofxAppUtils into this new group: ofxAppUtils/src
* You also need to add the following addon dependencies (if you're using them) in a similar manner:
	* ofxGui
* Make sure to remove any of the example folders in the Xcode project tree for any of the addons you've added manually

### Linux

Edit addons.make in your project folder and add the following line to the end of the file: 
~~~
ofxAppUtils
~~~

### Visual Studio

* Drag the ofxApputils/src folder onto the project tree
* Right-click on the project in the project tree and select Properties
* Set the Configuration to All Configurations
* Add the defines and search paths:
  - add to the search paths in Configuration Properties->C/C++->General->Additonal Include Directories:
  ~~~
  ..\\..\\..\\addons\ofxAppUtils\src
  ~~~

### Extending Your BaseApp

There are two changes you need to make to extend your app with the ofApputils ofxApp class which handles all the *automagic*:

* Your base app needs to inherit from ofxApp in your ofApp.h
~~~
class ofApp : public ofBaseApp { 
// becomes
class ofApp : public ofxApp { 
~~~
* You need to run your app with the app utils in main.cpp
~~~
ofRunApp(new ofApp());
// becomes
ofRunAppWithAppUtils(new ofApp());
~~~

That's it. Check the example ofApp::setup() for more info on startup settings.

Note: this is completely optional. You can always handle the transforms etc yourself using an ofxTransformer instance, etc.

#### ofxGetAppPtr

Internally, calling `ofRunAppWithAppUtils` actually creates a wrapper ofBaseApp which then runs the given ofxApp. As a result, **calling `ofGetAppPtr()` does not work correctly** as it returns a pointer to the wrapper and not the app itself.

In order to fix this, there is a new function added with ofxAppUtils called `ofxGetAppPtr()` (note the "x") which returns the correct pointer. If you are porting existing code, make sure to change any `ofGetAppPtr()` calls to `ofxGetAppPtr()`.

Developing
----------

You can help develop ofxAppUtils on GitHub: [https://github.com/danomatika/ofxAppUtils](https://github.com/danomatika/ofxAppUtils)

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.
