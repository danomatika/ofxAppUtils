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
#include "ofMain.h"
#include "ofApp.h"

int main() {

	ofSetupOpenGL(800, 600, OF_WINDOW);

	// this replaces ofRunApp() and kicks off the running of the app
	// with the app utils
	ofRunAppWithAppUtils(new ofApp());
}
