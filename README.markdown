ofxAppUtils
===========

OpenFrameworks app utilities

Copyright (c) [Dan Wilcox](danomatika.com) 2011

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See Documentation on [Github](https://github.com/danomatika/ofxAppUtils) and the [OpenFrameworks Forum post](http://forum.openframeworks.cc/index.php/topic,5995.0.html).

OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

Description
-----------

ofxAppUtils is a set of utilities for OpenFrameworks application development including:

* ofxApp: an ofBaseApp extension with built in screen scaling, projection mapping transforms, quad warping, and an optional ofxControlPanel
* ofxScene: a mini ofBaseApp for writing stand alone scenes
* ofxSceneManager: handles a list of scenes using a std::map
* ofxTransformer: open gl transformer for origin translation, screen scaling, mirroring, and quad warping
* ofxQuadWarper: an open gl matrix quad warper (useful for oblique projection mapping)
* ofxTimer: a simple millis-based timer
* ofxParticle: a simple time-based particle base class
* ofxParticleSystem: an auto manager for ofxParticles
* ofxBitmapString: a stream interface for ofDrawBitmapString

All ofBaseApp callbacks are handled down to the scene level.

Installation
------------

To use ofxAppUtils, first you need to download and install Open Frameworks. ofxAppUtils is currently developed against the latest version of Open Frameworks on github (007).

To get a copy of the repository you can download the source from [http://github.com/danomatika/ofxAppUtils/zipball/master](http://github.com/danomatika/ofxAppUtils/zipball/master) or, alternatively, you can use git clone:
<pre>
git clone git://github.com/danomatika/ofxAppUtils.git
</pre>

The addon should sit in `openFrameworks/addons/ofxAppUtils/`.

You will also need the following addon dependencies:

* ofxXmlSettings: included with OpenFrameworks
* ofxControlPanel: https://github.com/kylemcdonald/ofxControlPanel (optional)

To disable the control panel (and ofxControlPanel dependency), define `OFX_APP_UTILS_NO_CONTROL_PANEL` in your CFLAGS. For Xcode, see the Project.xcconfig file.

Running the Example Project
-------------------------------

If you're using OS X, open the Xcode project in ofxAppUtils/example/ and hit "Build and Run".

The addon should work fine in Windows and Linux, although there are no example build files yet.

Adding ofxAppUtils to an Existing Project
---------------------------------------

If you want to add ofxAppUtils to another project, you need to make sure you include the src folders:
<pre>
openFrameworks/addons/ofxAppUtils/src
</pre>

For Xcode:

* create a new group "ofxAppUtils"
* drag these directories from ofxAppUtils into this new group: ofxAppUtils/src
* you also need to add the following addon dependencies in a similar manner:
	* ofxControlPanel
	* ofxXmlSettings

There are two changes you need to make to extend your app with the ofApputils ofxApp class:

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

DEVELOPING
----------

You can help develop ofxAppUtils on GitHub: [https://github.com/danomatika/ofxAppUtils](https://github.com/danomatika/ofxAppUtils)

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.
