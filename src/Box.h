//
//  Box.h
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Preprocessor command----
#pragma once


// ----Headers----
#include <iostream>
#include "ofMain.h"
#include "Box2D.h"
#include "Frag.h"
#include "convertFunc.h"

#define N_VERTICE 4 //for Rectangle.
#define N_FRAGS 24 //num of frags

// ----Class definition----
class Box{
protected:

    // OSC
    // ofxOscSender            sender;
 
    // Position
    float   posX;
    float   posY;
    
    // Size
    float   size = 128.f; // 1024 / 8
//    float   size = 180.f;
    
	// Box2D
	b2Body*		mBody;
    b2World*    mWorld;
    
    const int     maxVertexCount = N_VERTICE; //just for rectangle.
    
    b2Vec2 mVertice[N_FRAGS];
    b2Vec2 mVerticeDiv[N_FRAGS];
    b2Vec2 tVertice[N_VERTICE];

    
    
    ofColor fragOutlineColor;
    ofColor fragFillColor;
    vector<Frag*>   mFrags;
    bool    isFragsRemain = true;
    bool                    isThereMbodybool;
    bool                    isOriginal;
    float       fragLifeTimeBySec;
    float       fragLifeTime;
    bool        isNextTarget = false;
    
    bool    toBlack = false;
    bool    toBlue = false;
    bool    canBeHit = false;
    bool    canBeHitUpdated = false;
    int     index;
    ofColor         normalColor;
    
    
public:

    ofTrueTypeFont verdana14;
    
    // VARIABLE
    // etc
    int     age = 0;
    
    
    // FUNCTION
    // Birth and Death
    Box(b2World* world, float x, float y,
    int cbit, int mbit);
    Box(b2World* world, float x, float y,
        int cbit, int mbit, int idx);
    ~Box();
    
    // Getter
    int     getIndex();
    bool    getCanBeHit();
    bool    getCanBeHitUpdated();
    bool    getIsThereMBody();
    bool    getFragsRemain();
    
    float   getX();
    float   getY();
    
    
    b2World*    getWorld();
    b2Body*     getBody();
    
    // Setter

    void    setNextTarget(bool target);
    void    setToBlack(bool makeBlack);
    void    setToBlue(bool makeBlue);
    void    setX(float posX);
    void    setY(float posY);
        
    // Render
    void    renderAtBodyPosition();
    
    // Update & draw
    void    update();
    void    draw();
    
    // Etc
    void    delMbody();
    float   getArea(b2Vec2*, int);
    float   perp_dot(ofVec2f a, ofVec2f b);
    float   perp_dot(b2Vec2 a, b2Vec2 b);
    
    void    breakBody();
    void    breakBody(float hitPointX, float hitPointY);
    void    makeMvertice();
    bool    IsInside(b2Vec2 p);
    void    pushForce(float x, float y);
    int     countTouch();


    // Frag
    void    renderFrags();
    void    clearFrags();
    

    //OSC
    void    oscSendI(string addr, int i);
    void    oscSendII(string addr, int i, int j);
    
    
    
};