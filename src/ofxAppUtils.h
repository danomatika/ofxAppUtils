/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

#include "ofAppRunner.h"

#include "ofxApp.h"
#include "ofxScene.h"
#include "ofxSceneManager.h"
#include "ofxTimer.h"
#include "ofxMovingAverage.h"
#include "ofxParticleManager.h"
#include "ofxBitmapString.h"

/// replace ofRunApp with this in main.cpp, make sure to call ofxGetAppPtr()
/// instead of ofGetAppPtr() if you need access to the global ofxApp pointer
inline void ofRunAppWithAppUtils(ofxApp *app) {

	// wrap up user app with runner
	#ifdef TARGET_OF_IOS
		ofRunApp((ofxiOSApp *) new ofxApp::RunnerApp(app));
	#else
		ofRunApp((ofBaseApp *) new ofxApp::RunnerApp(app));
	#endif
}

/// get the global ofxApp ptr, important! use this INSTEAD of ofGetAppPtr()
/// if you started your app with ofRunAppWithAppUtils()
inline ofxApp* ofxGetAppPtr() {
	return ((ofxApp::RunnerApp *) ofGetAppPtr())->getAppPtr();
}
