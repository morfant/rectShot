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
#include "Frag.h"
#include "convertFunc.h"

#define HOST "localhost"
#define PORT 57120



// ----Class definition----
class PolygonBody{
protected:
    
    // Position
    float   posX;
    float   posY;
    
	// Box2D
    b2World*    mWorld;
	b2Body*		mBody;
    b2Body*     mBody2;
    vector<Frag*>   mFrags;


    b2Vec2 mPts[kMAX_VERTICES];
    b2Vec2 mPtsP[kMAX_VERTICES];
    b2Vec2 mVertice[kMAX_VERTICES];
    b2Vec2 mVerticeDiv[kMAX_VERTICES/kSAMPLING_INTV];
    
    int     maxVertexCount; // maxVertexCount
    int     index;
    
    // Color
    ofColor     defaultColor;
    ofColor     selectedColor;
    
    // Select interface
    bool        selected;
    
    // Userdata
    int pBodyUserData;
    
    // Polyliner
    int curSection;
    int curPointofSection;
    vector<float>           dists;
    vector<ofVec2f>         addDist;
    
    // OSC
    ofxOscSender            sender;
    
    
    
    
public:
    // Birth and Death
    PolygonBody(b2World* world, b2Vec2* vertices, int maxVCount, float x, float y, int idx);
    
    ~PolygonBody();
    
    // Getter
    float   getArea(b2Vec2* vertices, int maxVCount);
    float   getX();
    float   getY();
    b2Vec2   getVertex(int i);
    b2Vec2* getBreakArray();
    
    b2World*    getWorld();
    b2Body*     getBody();
    
    bool    getSelectState();
    
    void    clearFrags();
    
    // Setter
    void    setSelectState(bool isSelect);
    void    setX(float posX);
    void    setY(float posY);
    void    setVertices(b2Vec2* vertices);
        
    // Render
    void    renderAtBodyPosition();
    void    renderFrags();
    void    getSection();
    void    breakBody(float x, float y);
    
    // Update & draw
    void    update();
    void    draw();
    
    float   perp_dot(ofVec2f a, ofVec2f b);
    float   perp_dot(b2Vec2 a, b2Vec2 b);
    
    

    
    
    
    
    
    
    
};