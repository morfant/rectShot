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
#define N_FRAGS 40

// ----Class definition----
class Box{
protected:
    
    // Position
    float   posX;
    float   posY;
    
    // Size
//    float   size = 100.f;
    float   size = 180.f;
    
	// Box2D
	b2Body*		mBody;
    b2World*    mWorld;
    
    const int     maxVertexCount = N_VERTICE; //just for rectangle.
    
    b2Vec2 mVertice[N_FRAGS];
    b2Vec2 mVerticeDiv[N_FRAGS];
    b2Vec2 tVertice[N_VERTICE];
    
    
    ofColor fragOutlineColor;
    vector<Frag*>   mFrags;
    bool                    isAlive;
    bool                    isThereMbodybool;
    bool                    isOriginal;
    float       fragLifeTimeBySec;
    float       fragLifeTime;
    
    
    
public:
    
    // VARIABLE
    // etc
    bool    toBlack = false;
    int     age = 0;
    
    
    // FUNCTION
    // Birth and Death
    Box(b2World* world, float x, float y);
    ~Box();
    
    // Getter
    bool    getIsThereMBody();
    
    float   getX();
    float   getY();
    
    
    b2World*    getWorld();
    b2Body*     getBody();
    
    // Setter
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
    void    makeMvertice();
    bool    IsInside(b2Vec2 p);
    void    pushForce(float x, float y);
    
    // Frag
    void    renderFrags();
    void    clearFrags();
    

    
    
    
};