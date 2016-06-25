#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
    
//	ofSetupOpenGL(800, 600, OF_WINDOW);			// <--------
//	ofSetupOpenGL(800 * 2, 600, OF_WINDOW);			// <--------
  ofSetupOpenGL(1024*2, 768, OF_WINDOW);			// <-------- setup the GL context
//    ofSetupOpenGL(1440, 768, OF_FULLSCREEN);			// <-------- setup the GL context
//	ofSetupOpenGL(2048, 768, OF_WINDOW);			// <--------
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
