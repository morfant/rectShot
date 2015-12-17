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
#include <iostream>
#include <fstream>
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "World.h"
#include "Ball.h"
#include "Wall.h"
//#include "Faces.h"
#include "Box.h"
//#include "ofxOsc.h"
#include "Tm.h"




// ---- Macros ----
enum {
    
    ORIGINAL_DUP_IDX = 0,
    COPY_DUP_IDX = 0,
    GUNTIME = 10000,
    STAGE_NUM = 6,
    MOVNUM = 6, //Num of movie files + cam
    TH_CAM = 100, //curMov = 0
    TH_1 = 94, //aya
    TH_2 = 103,
    TH_3 = 100,
    TH_4 = 113,
    TH_5 = 111,

    CV_CAM_WIDTH = 720,
    CV_CAM_HEIGHT = 480,
    kBLOBNUM = 4,
    kMIN_BLOBAREA = 150
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
        void oscRecv();
        void oscSendMsg(string addr, float data);
        void oscSendMsg2(string addr, ofVec2f data);
    
        // Staging
        void nextStage();
        void nextStage(unsigned long long time, bool enable);
        void videoEnd();
        void tmEnable(int tNum);
        int targetTimer;
    
        // CV
        void sendBlobsOSC();
        void makeFaceAt(float x, float y);
        void firstShotCheck(int curStage);
    
        //Box2d
        void makeFaces(int blobNum);
        void drawPolygonBodies();
        void resetFaces();
        void makeBodyAtCvPosition();
        void makeBodyAtCvPosition(b2Vec2* vertices);
        void makeBodyAtCvPosition(vector<b2Vec2> vertices);
    
    void dupPbody(Faces* pb, float x, float y);
    
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
        bool                curMoviePlaying;
        bool                info;
    
        float               movX, movY;
    
    
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
    
        int                     pBodyIdx;
    
        bool                    touched;
        bool    isFirstShot[STAGE_NUM];
        ofPoint bornPoint[STAGE_NUM];
    
    
    //Staging
    
//    ofColor     originColor[STAGE_NUM];
    ofColor     pBodyOutlineColor[STAGE_NUM];    
    
    Tm*         tMan;
    bool        tmOpen;
    int         targetNum;
    int         curStage;
    unsigned long long stageStartTime;
    ofImage     title;
    bool        inPreparing;
    bool        inTitle;
    bool        OriginDestroyed;
    bool        nextStageReady;

    
    //Osc
    // OSC
    ofxOscSender            sender;
    ofxOscReceiver          receiver;

    void oscSendIIFF(string addr, int i, int j, float a, float b);
    void oscSendIFF(string addr, int i, float a, float b);
    void oscSendIF(string addr, int i, float a);
    void oscSendII(string addr, int i, int j);
    void oscSendI(string addr, int i);    
    
    
    
    bool                    bodyHit;
    bool                    shotBallMade;
    bool                    isShot;
    float                     shot_X;
    float                     shot_Y;
    bool                    butPressed;
    int                     butMsg;

    
        // container
    ofPoint cvBlobPos;
    ofRectangle cvBlobRect;
    int cvBlobNum;
    int selBlobRect;
        vector<Ball*>           balls;
        vector<Box*>            boxes;
        vector<Faces*>    pBodies;
        Faces       pBodiesOriginalCopy[STAGE_NUM];
//        vector<Faces>     pBodiesOriginalCopy;
    
        vector<ofxCvBlob>       blobsVec;
        vector<ofPoint>         blobsPts;
        vector<ofVec2f>         blobCenterPos;
    
        vector<b2Vec2>          blobsPtsDiv;
    
        float                   divNum;
        float                   aforce;

    
    // Tracking
    bool        shotPointTest;
    
};

