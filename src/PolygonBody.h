//
//  PolygonBody.h
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
#include "convertFunc.h"


// ----Class definition----
class PolygonBody{
protected:
    
    // Position
    float   posX;
    float   posY;
    
	// Box2D
    b2World*    mWorld;
	b2Body*		mBody;

    b2Vec2 mPts[320];
    
    int     maxVertexCount; // maxVertexCount
    
    // Color
    ofColor     defaultColor;
    ofColor     selectedColor;
    
    // Select interface
    bool        selected;
    
    // Userdata
    int pBodyUserData;
    
    
public:
    // Birth and Death
    PolygonBody(b2World* world, b2Vec2* vertices, int maxVCount, float x, float y);
    
    ~PolygonBody();
    
    // Getter
    float   getArea(b2Vec2* vertices, int maxVCount);
    float   getX();
    float   getY();
    b2Vec2   getVertex(int i);
    
    b2World*    getWorld();
    b2Body*     getBody();
    
    bool    getSelectState();
    
    // Setter
    void    setSelectState(bool isSelect);
    void    setX(float posX);
    void    setY(float posY);
    void    setVertices(b2Vec2* vertices);
        
    // Render
    void    renderAtBodyPosition();
    
    // Update & draw
    void    update();
    void    draw();
    
    
    
    
    
};