//
//  testApp.h
//  Created by giy.
//
//

// ---- Preprocessor command ----
#pragma once

#define REALTIME 0

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
    TH_CAM = 0, //curMov = 0
    TH_1 = 94, //aya
    TH_2 = 103, //han
    TH_3 = 100, //sewol
    TH_4 = 113, //park
    TH_5 = 111, //meÍ

    CV_CAM_WIDTH = 720,
    CV_CAM_HEIGHT = 480,
    kMIN_BLOBAREA = 150
};



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		

        //AUDIO
        ofSoundPlayer   soundPlayer;
        float 				fftSmoothed = 0;
        int                 nBandsToGet;
    

    
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
    
        void makeBodyAtCvPosition(ofFile file, float drawPosX, float drawPosY);
    
        void makeBodyAtCvPosition();
        void makeBodyAtCvPosition(b2Vec2* vertices);
        void makeBodyAtCvPosition(vector<b2Vec2> vertices);
    
        //UTIL
        float getArea(b2Vec2* vertices, int maxVCount);
        float getFragsArea();
        float ofToFloat(string);
    
    
        // OSC
        void oscRecv();
        void oscSendIIFF(string addr, int i, int j, float a, float b);
        void oscSendIFF(string addr, int i, float a, float b);
        void oscSendIF(string addr, int i, float a);
        void oscSendII(string addr, int i, int j);
        void oscSendF(string addr, float i);
        void oscSendI(string addr, int i);
    
    
        // Staging
        void nextStage();
        void nextStage(unsigned long long time, bool enable);
        void videoEnd();
        void tmEnable(int tNum, unsigned long long lifetime);
        void firstShotCheck(int curStage);
    
        // UI
        void touchingCheck();
    
    
    
    
        /*--------------------VARIABLE--------------------*/

        //FILE
        ofFile              fileToRead;
        
        //MOVIE, VIDEO
        ofVideoGrabber 		vidGrabber;
        ofVideoPlayer 		movie[MOVNUM];
        ofVec2f             movRes[MOVNUM];
        float               movAmp[MOVNUM];
        int                 curMovie;
        float               movDrawPosX, movDrawPosY;
        bool                movShow, grayShow, blobShow;
        bool                curMoviePlaying;
        bool                info;

    
        //CV
        int    kBLOBNUM = 4;
    
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
        Wall                    *left, *right, *floor, *ceil, *center;
        Box*                    aBox;
        Box*                    bBox;
        Ball*                   aBall;
        int                     pBodyIdx;
        bool                    touched;
        float                   sumOfArea;
    
    
        //TARGET MAKER
        ofColor     pBodyOutlineColor[STAGE_NUM];
        int         targetNum[STAGE_NUM];
        unsigned long long fragLifeTime[STAGE_NUM];
        Tm*         tMan;
        bool        tmOpen;
        bool        randFace;
        int         curStage;
        unsigned long long stageStartTime;
        ofImage     title;
        bool        inTitle;
        bool        inLastScene;
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
        bool                    blobsSynMade;

    
        // container
        Faces       pBodiesOriginalCopy[STAGE_NUM];
        vector<Ball*>           balls;
        vector<Box*>            boxes;
        vector<Box*>            blackBoxes;
        vector<Box*>            darkBoxes;
        vector<Faces*>          pBodies;
        vector<ofxCvBlob>       blobsVec;
        vector<ofPoint>         blobsPts;
        vector<ofVec2f>         blobCenterPos;
        vector<b2Vec2>          blobsPtsDiv;
    
        float                   divNum;
        float                   aforce;

    
    
    
};

