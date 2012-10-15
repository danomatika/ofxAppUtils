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
#include "testApp.h"

int main(){
	ofSetupOpenGL(1024, 768, OF_FULLSCREEN);

	// this replaces ofRunApp and kicks off the running of the app
    // with the app utils
	ofRunAppWithAppUtils(new testApp);
}
