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
//#include "ofxOsc.h"
#include "ContactListener.h"




// ---- Macros ----
enum {
    MOVNUM = 5, //Num of movie files + cam
    TH_CAM = 100,
    TH_1 = 100,
    TH_2 = 100,
    TH_3 = 100,
    TH_4 = 100,
    CV_CAM_WIDTH = 720,
    CV_CAM_HEIGHT = 480,
    kBLOBNUM = 2,
    kMIN_BLOBAREA = 100
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

        // OSC
        void oscSendMsg(string addr, float data);
        void oscSendMsg(string addr, ofVec2f data);
    
        // CV
        void sendBlobsOSC();
    
        //Box2d
        void makePolygonBody(int blobNum);
        void drawPolygonBodies();
        void resetPolygonBody();
        void makeBodyAtCvPosition();
    
        float getArea(b2Vec2* vertices, int maxVCount);    
    
        //cam
        ofVideoGrabber 		vidGrabber;
    
        //video
        ofVideoPlayer 		movie[MOVNUM];
        ofVec2f             movRes[MOVNUM];
        bool                camUse;
        int                 curMovie;
        int                 preMovie;
        bool                frameByframe;
        bool                movPlay;
        bool                grayPlay;
        bool                movPlaySmall;
    
        bool                info;
    
    
        //blob
        ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
        ofxCvContourFinder      contourFinder;

		int                     threshold[MOVNUM];
        bool                    inverting[MOVNUM];
        bool                    drawBlob;
    
    
        //Box2d
        World*                  aWorld;
        b2World*                iWorld; //aWorld -> getWorld()
        Ball*                   aBall;
        ofVec2f                 tVec;
        Wall                    *left, *right, *floor, *ceil;
        Box*                    aBox;
        ContactListener*        contactlistener;
    
        int                     pBodyIdx;
    
        bool                    touched;
    
        // OSC
        ofxOscSender            sender;
    
        // container
    ofPoint cvBlobPos;
    ofRectangle cvBlobRect;
    int cvBlobNum;
    int selBlobRect;
        vector<Ball*>           balls;
        vector<Box*>            boxes;
        vector<PolygonBody*>    pBodies;
        vector<ofxCvBlob>       blobsVec;
        vector<ofPoint>         blobsPts;
        vector<ofVec2f>         blobCenterPos;
    
        vector<b2Vec2>          blobsPtsDiv;
        vector<b2Vec2>          rBlobsPtsDiv;

    
    
        float                   divNum;
    
    float                   aforce;


};

