#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){

//	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(1920, 1080, OF_WINDOW);			// <-------- setup the GL
	ofSetupOpenGL(1440, 900, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(720, 480, OF_WINDOW);			// <-------- setup the GL context
    
//	ofSetupOpenGL(1200, 900, OF_WINDOW);			// <--------
//	ofSetupOpenGL(1160, 880, OF_WINDOW);			// <--------
//	ofSetupOpenGL(800, 600, OF_WINDOW);			// <--------

    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
