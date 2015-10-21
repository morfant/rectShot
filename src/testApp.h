//
//  testApp.h
//  cvBody_001
//
//  Created by giy.
//
//

// ---- Preprocessor command ----
#pragma once

// ---- Headers ----
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "World.h"
#include "Ball.h"
#include "Wall.h"
#include "PolygonBody.h"
#include "Box.h"

// ---- Macros ----
enum {
    OPENCV_WIDTH = 720,
    OPENCV_HEIGHT = 480,
    kBLOBNUM = 1,
    kMAX_VERTICES = 320,
    kMIN_BLOBAREA = 10000
};



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

        //cv
    int selBlob(ofRectangle blobRect, float x, float y);
    
        //Box2d
        void makePolygonBody(int blobNum);
        void drawPolygonBodies();
        void resetPolygonBody();
        void makeBodyAtCvPosition();
        float getArea(b2Vec2* vertices, int maxVCount);
    
    
        //cam
        ofVideoGrabber 		vidGrabber;
    
        //video
        ofVideoPlayer 		movie;
        bool                frameByframe;
    
    
        //blob
        ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
        ofxCvContourFinder      contourFinder;

		int                     threshold;
		bool                    bLearnBakground;
    
    
        // World
        World*                  aWorld;
        b2World*                iWorld; //aWorld -> getWorld()
        
        // Ball
        Ball*                   aBall;
        
        ofVec2f                 tVec;
        
        // Wall
        Wall                    *left, *right, *floor, *ceil;
        
        // container
    ofPoint cvBlobPos;
    ofRectangle cvBlobRect;
    int cvBlobNum;
    int selBlobRect;
        vector<Ball*>           balls;
        vector<PolygonBody*>    pBodies;
        vector<ofxCvBlob>       blobsVec;
        vector<ofPoint>         blobsPts;
        vector<ofVec2f>         blobCenterPos;
    
        vector<b2Vec2>          blobsPtsDiv;
        vector<b2Vec2>          rBlobsPtsDiv;
        
        float                   divNum;


};

