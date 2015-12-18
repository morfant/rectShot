//
//  testApp.h
//  cvBody_001
//
//  Created by giy.
//
//

// ---- Preprocessor command ----
#pragma once

#define REALTIME 1

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
    TH_2 = 103, //han
    TH_3 = 100, //sewol
    TH_4 = 113, //park
    TH_5 = 111, //me

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
    
        // CV
        void sendBlobsOSC();
        void makeFaceAt(float x, float y);
    
        //Box2d
        void makeFaces(int blobNum);
        void drawPolygonBodies();
        void resetFaces();
        void makeBodyAtCvPosition();
        void makeBodyAtCvPosition(b2Vec2* vertices);
        void makeBodyAtCvPosition(vector<b2Vec2> vertices);
    
        //UTIL
        float getArea(b2Vec2* vertices, int maxVCount);    
    
    
        // OSC
        void oscRecv();
        void oscSendIIFF(string addr, int i, int j, float a, float b);
        void oscSendIFF(string addr, int i, float a, float b);
        void oscSendIF(string addr, int i, float a);
        void oscSendII(string addr, int i, int j);
        void oscSendI(string addr, int i);
    
    
        // Staging
        void nextStage();
        void nextStage(unsigned long long time, bool enable);
        void videoEnd();
        void tmEnable(int tNum);
        void firstShotCheck(int curStage);
        bool isOriginalCopyed();
    
        // UI
        void touchingCheck();
    
    
    
    
        /*--------------------VARIABLE--------------------*/
        //MOVIE, VIDEO
        ofVideoGrabber 		vidGrabber;
        ofVideoPlayer 		movie[MOVNUM];
        ofVec2f             movRes[MOVNUM];
        int                 curMovie;
        float               movDrawPosX, movDrawPosY;
        bool                movShow, grayShow, blobShow;
        bool                curMoviePlaying;
        bool                info;

    
        //CV
        ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage, grayBg, grayDiff;
        ofxCvContourFinder      contourFinder;
		int                     threshold[MOVNUM];
        bool                    inverting[MOVNUM];
        ofPoint                 cvBlobPos;
        ofRectangle             cvBlobRect;
        int                     cvBlobNum;
        int                     selBlobRect;
    
    
    
        //Box2d
        World*                  aWorld;
        b2World*                iWorld; //aWorld -> getWorld()
        Wall                    *left, *right, *floor, *ceil;
        Box*                    aBox;
        Ball*                   aBall;
        int                     pBodyIdx;
        bool                    touched;
    
    
        //TARGET MAKER
        ofColor     pBodyOutlineColor[STAGE_NUM];
        Tm*         tMan;
        bool        tmOpen;
        int         targetNum;
        int         curStage;
        unsigned long long stageStartTime;
        ofImage     title;
        bool        inTitle;
        bool        blackout;
        bool        OriginDestroyed;
        bool        nextStageReady;
        bool                    bodyHit;
        bool                    shotBallMade;
        bool                    isShot;
        float                   shot_X;
        float                   shot_Y;
        bool                    butPressed;
        int                     butMsg;
    
        //STAGING
        bool        isFirstShot[STAGE_NUM];
        ofPoint     bornPoint[STAGE_NUM];
        bool        shotPointTest;
    
    
    
        //OSC
        ofxOscSender            sender;
        ofxOscReceiver          receiver;

    
        // container
        Faces       pBodiesOriginalCopy[STAGE_NUM];
        vector<Ball*>           balls;
        vector<Box*>            boxes;
        vector<Faces*>          pBodies;
        vector<ofxCvBlob>       blobsVec;
        vector<ofPoint>         blobsPts;
        vector<ofVec2f>         blobCenterPos;
        vector<b2Vec2>          blobsPtsDiv;
    
        float                   divNum;
        float                   aforce;

    
    
    
};

