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
#include "Box.h"
#include "OSCFunc.h"
#include "Faces.h"




// ---- Macros ----
enum {
    FACENUM = 7,
    ORIGINAL_DUP_IDX = 0,
    COPY_DUP_IDX = 0,
    GUNTIME = 10000,
    STAGE_NUM = 6,
    MOVNUM = 12, //Num of movie files + cam
    TH_CAM = 0, //curMov = 0
    TH_1 = 94, //aya
    TH_2 = 103, //han
    TH_3 = 100, //sewol
    TH_4 = 113, //park
    TH_5 = 111, //meÍ
    TH_6 = 111, //meÍ
    TH_7 = 111, //meÍ
    TH_8 = 111, //meÍ
    TH_9 = 111, //meÍ
    TH_10 = 111, //meÍ
    TH_11 = 111, //meÍ

    kMIN_BLOBAREA = 200,

    kBLUE_BOX_IDX = 10,
    kPRE_ENDING_BOX_LIMIT = 53, //kENDING_BOX_LIMIT - FACENUM
    kENDING_BOX_LIMIT = 60
};



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		

        //AUDIO
        void audioIn(float * input, int bufferSize, int nChannels);
    
        vector <float> leftSig;
        vector <float> rightSig;
        
        int     bufferCounter;
        float   smoothedVol;
        float   scaledVol;
        
        ofSoundStream soundStream;
    
    
        //OSCFunc
        OSCFunc*             oscFunc;

    
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
        void makeFaceAt(float posX, float posY);
        void makeBodyAt(float posX, float posY); 
        
        //Box2d
        void drawPolygonBodies();
        void resetFaces();

        void makeFaceVertice(int blobNum);
        void saveFaceVerticeToFile(int blobNum, string fileName);
        void makeBodyFromFile(ofFile file, float drawPosX, float drawPosY);
    
        //UTIL
        float getArea(b2Vec2* vertices, int maxVCount);
        float getFragsArea();
        float ofToFloat(string);
    
    
        // OSC
        void oscRecv();

    
    
        // Staging
        void videoEnd();
    
        // UI
        bool slideLeft = false;
        bool slideRight = false;
        bool slideWindowInTime(ofPoint bPos, ofPoint ePos, float nframe);
        void slideScreen(bool toLeft);


        void killAll();
        bool        killSwitch = false;
        bool        makeBox = true;
    
    
    
    
        /*--------------------VARIABLE--------------------*/


        //WINDOW
        int                 lastTime = 0;
        int                 curTime = 0;
        int                 boxMakingTime;
        float               ampMul = 2.0f;
        ofPoint             windowPosition;


        bool                isLeft = true;
        bool                isInEnding = false;
        bool                isInPreEnding = false; 

        //FILE
        ofFile              fileToRead;
        
        //MOVIE, VIDEO
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
        int    curFace = 1;
    
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
        Wall                    *left, *right, *floor, *ceil, *center, *hardCenter;
        Box*                    aBox;
        Box*                    bBox;
        Ball*                   aBall;
        int                     pBodyIdx;
        int                     boxIdx = 0;
        int                     boxNum = 0;
        bool                    touched;
        float                   sumOfArea;
        int                     thickness;
        int                     targetIdx;
    


        //Frag
        int                     fragLife = 5;
        ofColor                 fragColor;
    
        //TARGET MAKER
        // ofColor     pBodyOutlineColor[STAGE_NUM];
        // int         targetNum[STAGE_NUM];
        // unsigned long long fragLifeTime[STAGE_NUM];
        // Tm*         tMan;
        // bool        tmOpen;
        // bool        randFace;
        // int         curStage;
        // unsigned long long stageStartTime;
        // ofImage     title;
        // bool        inTitle;
        // bool        inLastScene;
        // bool        blackout;
        // bool        OriginDestroyed;
        // bool        nextStageReady;

       

        bool                    bodyHit;
        bool                    shotBallMade;
        bool                    isShot;
        float                   shot_X;
        float                   shot_Y;
        bool                    butPressed;
        // int                     butMsg;
    
        //STAGING
        bool        isFirstShot[STAGE_NUM];
        ofPoint     bornPoint[STAGE_NUM];
        bool        shotPointTest;
    
    
        void                    sendBlobsOSC();
        bool                    blobsSynMade;
        int                     numBoxes = 0;

    
        // container
        // Faces       pBodiesOriginalCopy[STAGE_NUM];
        vector<Ball*>           balls;
        vector<Box*>            boxes;
        vector<Box*>            blackBoxes;
        vector<Box*>            darkBoxes;
        vector<Box*>            hittableBoxes;
        vector<Faces*>          pBodies;
        vector<ofxCvBlob>       blobsVec;
        vector<ofPoint>         blobsPts;
        vector<ofVec2f>         blobCenterPos;
        vector<b2Vec2>          blobsPtsDiv;
    
        float                   divNum;
        float                   aforce;

    
    
    
};

