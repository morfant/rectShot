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
#include "convertFunc.h"


// ----Class definition----
class Box{
protected:
    
    // Position
    float   posX;
    float   posY;
    
    // Size
    float   size = 200.f;
    
	// Box2D
	b2Body*		mBody;
    b2World*    mWorld;
    
    
public:
    // Birth and Death
    Box(b2World* world, float x, float y);
    ~Box();
    
    // Getter
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
    
    
    
    
    
};