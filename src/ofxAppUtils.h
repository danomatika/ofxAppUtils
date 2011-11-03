#pragma once

#include <ofxXmlSettings.h>
#include <ofxControlPanel.h>

#include "ofxApp.h"
#include "ofxScene.h"
#include "ofxSceneManager.h"
#include "ofxTimer.h"

/// replace ofRunApp with this in main.cpp ...
inline void ofRunAppWithAppUtils(ofxApp* app) {

    // wrap up user app with runner
    ofRunApp((ofBaseApp*) new ofxRunnerApp(app));
}
