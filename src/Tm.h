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
#include "PolygonBody.h"
#include "convertFunc.h"


// ----Class definition----
class Tm{
protected:

    // Box2d
    b2World*    mWorld;
    
    float   dupPosX;
    float   dupPosY;

    
    unsigned long long  startTime;
    unsigned long long  curTime;
    float   timeInterval;
    PolygonBody *   targetOrigin;
    

    
    
public:
    // Birth and Death
    Tm(b2World* world, PolygonBody* pBody, float timeInterval);
    ~Tm();
    

    // Get & Set
    void    setTimer(float time);
    void    setPbody(PolygonBody* pBody);
    
    float   getTimer();
    PolygonBody*    getTargetBody();
    
    // Duplicate Target
    PolygonBody* dupPbody(PolygonBody* pbody, float x, float y);
        

    
    
    // Update & draw
    PolygonBody*    update();
    void    draw();
    
    
    // Util


    // container
    vector<PolygonBody*>    pBodies;
    
    
    
};