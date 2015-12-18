//
//  Faces.h
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Preprocessor command----
#pragma once
//#define POLYGON_BODY



// ----Headers----
#include <iostream>
#include "ofMain.h"
#include "Box2D.h"
#include "Frag.h"
#include "convertFunc.h"





// ----Class definition----
class Faces{
protected:
    
    // Position
    float   posX;
    float   posY;
    
	// Box2D
    b2World*    mWorld;
	b2Body*		mBody;
    b2Body*     mBody2; //Outline tracker.
    
    float outliner_rad;
    float rotSpd;
    float audioLen;
    
    vector<Frag*>   mFrags;


    b2Vec2 mPts[kMAX_VERTICES];
    b2Vec2 mPtsP[kMAX_VERTICES];
    b2Vec2 mVertice[kMAX_VERTICES];
    b2Vec2 mVerticeDiv[kMAX_VERTICES/kSAMPLING_INTV]; //For breaking
    b2Vec2 tVertice[kMAX_VERTICES];
    
    int     fragNum;
    
    
    int     maxVertexCount; // maxVertexCount
    int     index;
    int     dupIndex;
    
    // Color
    ofColor     contactedColor;
    ofColor     normalColor;
    ofColor     fragOutlineColor;
    
    // Select interface
    bool        selected;
    bool        isNewBorn;
    bool        isBreaked;
    bool        isContacted;
    
    
    
    // Userdata
    int pBodyUserData;
    int smallBodyUserData;
    
    // Polyliner
    int curSection;
    int curPointofSection;
    vector<float>           dists;
    vector<ofVec2f>         addDist;
        
    // Aging
    bool                    isAlive;
    bool                    isThereMbodybool;
    bool                    isOriginal;
    float       fragLifeTimeBySec;
    float       fragLifeTime;
    
    bool                    isReal;
    
public:
    // Birth and Death
    Faces();
    Faces(b2World* world, b2Vec2* vertices, int maxVCount, float x, float y, int idx, bool isReal, bool isOrigin, int dupIdx);
    
    ~Faces();
    
    // Getter
    float   getArea(b2Vec2* vertices, int maxVCount);
    float   getX();
    float   getY();
    int     getIndex();
    int     getDupIndex();
    bool    getIsBreaked();
    b2Vec2   getVertex(int i);
    b2Vec2*  getVertices();
    b2Vec2* getBreakArray();
    
    b2World*    getWorld();
    b2Body*     getBody();
//    b2Body*     getSmallBody(); // outline tracking body
    
    bool    getSelectState();
    bool    getIsThereMBody();
    bool    getIsAlive();
    bool    getIsOriginal();
    bool    getIsReal();
    bool    getIsNewBorn();
    ofColor getFragOutlineColor();
    
    vector<Frag*> *  getFrags();
    
    void    clearFrags();
    
    // Setter
    void    setContact(bool cont);
    void    setSelectState(bool isSelect);
    void    setX(float posX);
    void    setY(float posY);
    void    setVertices(b2Vec2* vertices);
    void    setAudioLen(float len);
    void    setContactColor(ofColor color);
    void    setIsNewBorn(bool born);
    void    setIsBreaked(bool isBreak);
    void    setFragOutlineColor(ofColor foc);
    void    delMbody();
    void    pushForce(float x, float y);
        
    // Render
    void    renderAtBodyPosition();
    void    renderFrags();
    void    getSection();
    void    breakBody();
    void    breakFrags();
    

    
    // Update & draw
    void    update();
    void    draw();
    
    float   perp_dot(ofVec2f a, ofVec2f b);
    float   perp_dot(b2Vec2 a, b2Vec2 b);
    
    bool    IsInside(b2Vec2 p);
    
    
    
};