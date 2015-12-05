//
//  Frag.h
//  cvBody_chainShape
//
//  Created by giy on 04/12/2015.
//
//

// ----Preprocessor command----
#pragma once


// ----Headers----
#include <iostream>
#include "ofMain.h"
#include "Box2D.h"
#include "convertFunc.h"


// ----Class definition----
class Frag{
protected:
    
    // Position
    float   posX;
    float   posY;
    float   movX;
    float   movY;
    
    // Size
    float   size = 10.f;
    
	// Frag2D
	b2Body*		mBody;
    b2World*    mWorld;
    b2Vec2 mVertice[3];

    
public:
    // Birth and Death
    Frag(b2World* world, float x, float y, float mx, float my, b2Vec2* vertices);
    ~Frag();
    
    // Getter
    float   getX();
    float   getY();
    
    
    b2World*    getWorld();
    b2Body*     getBody();
    
    // Setter
    void    setX(float posX);
    void    setY(float posY);
    
    // Render
    void    render();
    
    // Update & draw
    void    update();
    void    draw();
    
    
    
    
    
};