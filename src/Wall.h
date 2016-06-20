//
//  Wall.h
//  emptyExample
//
//  Created by Gangil Yi on 9/1/13.
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
class Wall{
protected:
    
    // Position
    float   posX;
    float   posY;
    
    // length
    float   lenght;
    
    // Point
    b2Vec2 mBeginPoint;
    b2Vec2 mEndPoint;
    
    b2Vec2 vertice[4];
    
	// Body & world
	b2Body*		mBody;
    b2World*    mWorld;
    
    // Userdata
    int wallUserData;
    
    
public:
    // Birth and Death
    Wall();
//    Wall(b2World* aWorld, b2Vec2 beginPoint, b2Vec2 endPoint, float length);
    Wall(b2World* aWorld, int x, int y, float w, float h);
    Wall(b2World* aWorld, int x, int y, float w, float h, int cBit, int mBit);
    
    ~Wall();
    
    // Getter
    b2World*    getWorld();
    b2Body*     getBody();
    
    b2Vec2   getBeginPoint();
    b2Vec2   getEndPoint();
    
    b2Vec2  getVertex(int idx);
    
    // Setter
    void    setX(float posX);
    void    setY(float posY);
    
    // Render
    void    renderAtBodyPosition();
    
    // Update & draw
    void    update();
    void    draw();
    
    
    
};