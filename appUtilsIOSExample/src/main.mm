/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#include "ofMain.h"
#include "ofApp.h"

int main() {

	//  here are the most commonly used iOS window settings.
    //------------------------------------------------------
    ofiOSWindowSettings settings;
    settings.enableRetina = false; // enables retina resolution if the device supports it.
    settings.enableDepth = false; // enables depth buffer for 3d drawing.
    settings.enableAntiAliasing = false; // enables anti-aliasing which smooths out graphics on the screen.
    settings.numOfAntiAliasingSamples = 0; // number of samples used for anti-aliasing.
    settings.enableHardwareOrientation = true; // enables native view orientation.
    settings.enableHardwareOrientationAnimation = true; // enables native orientation changes to be animated.
    //settings.glesVersion = OFXIOS_RENDERER_ES1; // type of renderer to use, ES1, ES2, etc.
    //settings.setupOrientation = OF_ORIENTATION_90_LEFT; // set default orientation for setup

    ofCreateWindow(settings);

	// this replaces ofRunApp and kicks off the running of the app
    // with the app utils
	ofRunAppWithAppUtils(new ofApp);
}
