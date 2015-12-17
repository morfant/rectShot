//
//  Tm.h
//  cvBody_chainShape
//
//  Created by giy on 11/12/2015.
//
//
// ----Preprocessor command----
#pragma once


// ----Headers----
#include <iostream>
#include "ofMain.h"
#include "Box2D.h"
#include "Faces.h"
#include "convertFunc.h"


// ----Class definition----
class Tm{
protected:

    // Box2d
    b2World*    mWorld;
    
    float   dupPosX;
    float   dupPosY;
    
    int     dupNum;

    
    unsigned long long  startTime;
    unsigned long long  curTime;
    float   timeInterval;
    Faces *   targetOrigin;
    
    bool    enable;
    

    
    
public:
    // Birth and Death
    Tm(b2World* world, Faces* pBody, float timeInterval);
    ~Tm();
    

    // Get & Set
    void    setTimer(float time);
    void    setPbody(Faces* pBody);
    void    setDupNum(int num);
    void    start();
    
    float   getTimer();
    Faces*    getTargetBody();
    
    // Duplicate Target
    Faces* dupPbody(Faces* pbody, float x, float y);
        

    // Update & draw
    Faces*    update();
    bool            isEnd();
    void    draw();
    
    
    // Util


    // container
    vector<Faces*>    pBodies;
    
    
    
};