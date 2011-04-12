

ofxAppUtils
==================

ofxAppUtils is a set utilites for applicaiton development including:

* ofxApp: an ofBaseApp extension with built in screen scaling, projection mapping transforms, quad warping, and an ofxControlPanel
* ofxScene: a mini ofBaseApp for writing stand alone scenes
* ofxSceneManager: handles a list of scenes using a std::map
* ofxTimer: a simple millis timer

OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

Installation
------------

To use ofxAppUtils, first you need to download and install Open Frameworks. ofxAppUtils is currently developed against the latest version of Open Frameworks on github (007).

To get a copy of the repository you can download the source from [http://github.com/danomatika/ofxAppUtils/zipball/master](http://github.com/danomatika/ofxAppUtils/zipball/master) or, alternatively, you can use git clone:
<pre>
git clone git://github.com/danomatika/ofxAppUtilsgit
</pre>

The addon should sit in openFrameworks/addons/ofxAppUtils/.

You will also need the following addon dependencies:

* ofxControlPanel: https://github.com/ofTheo/ofxControlPanel
* ofxLogger: https://github.com/danomatika/ofxLogger

Running the Example Project
-------------------------------

If you're using OS X, open the XCode project in ofxAppUtils/ofxAppUtilsExample/ and hit "Build and Run".

Adding ofxLogger to an Existing Project
---------------------------------------

If you want to add ofxAppUtils to another project, you need to make sure you include the src folders:
<pre>
openFrameworks/addons/ofxAppUtils/src
</pre>

For XCode:

* create a new group "ofxAppUtils"
* drag these directories from ofxAppUtils into this new group: ofxAppUtils/src
* you also need to add the following addon dependencies in a similar manner:
** ofxControlPanel
** ofxLogger

