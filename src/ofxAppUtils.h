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

#include "ofxApp.h"
#include "ofxScene.h"
#include "ofxSceneManager.h"
#include "ofxTimer.h"
#include "ofxParticleManager.h"
#include "ofxBitmapString.h"

/// replace ofRunApp with this in main.cpp ...
inline void ofRunAppWithAppUtils(ofxApp* app) {

    // wrap up user app with runner
	#ifdef TARGET_OF_IPHONE
		ofRunApp((ofxiPhoneApp*) new ofxApp::RunnerApp(app));
	#else
		ofRunApp((ofBaseApp*) new ofxApp::RunnerApp(app));
	#endif
}

//
inline ofxApp* ofxGetAppPtr() {
	return ((ofxApp::RunnerApp*) ofGetAppPtr())->getAppPtr();
}
