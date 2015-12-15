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

#define HOST "localhost"
#define PORT 57120
#define RECV_PORT 20000


// ----Class definition----
class Frag{
protected:
    
    int     pBodyIndex;
    int     index;
    void *     origin;
    
    // Position
    float   posX;
    float   posY;
    
    // Size
    float   size = 10.f;
    
    // Userdata
    int     fragUserData;
    
	// Frag2D
    
    b2World*    mWorld;
    b2Body*     mBody;
    b2Vec2 mVertice[3];
    
    // Color
    bool        outlineDraw;
    ofColor     outlineColor;
    
    
    
    // OSC
//    ofxOscSender            sender;

    
    // aging
    unsigned long long  lifeLong;
    unsigned long long  age;
    bool                isAlive;
    bool                isNewBorn;


    
    
public:
    // Birth and Death
    Frag(b2World* world, float mx, float my, b2Vec2* vertices, int pBodyIdx, int idx, ofColor outlineCol);
    ~Frag();
    
    bool isContactSelf();
    
    // Getter
    float   getX();
    float   getY();
    unsigned long long    getLifeLong();
    unsigned long long    getAge();
    bool    getIsNewBorn();
    int     getIndex();
    
    
    b2World*    getWorld();
    b2Body*     getBody(int idx);
    
    // Setter
    void    setX(float posX);
    void    setY(float posY);
    void    setLifeLong(unsigned long long life);
    void    setAge(unsigned long long age);
    void    setIsNewBorn(bool born);
    void    makeb2Tri(b2Vec2 * vertices);
    float   perp_dot(ofVec2f a, ofVec2f b);

    void    breakSelf();
    
    // Render
    void    render();
    
    void    beOld();
    
    // Update & draw
    bool    update();
    void    draw();
    
    // osc
    void    oscSendIFF(string addr, int i, float a, float b);
    void    oscSendIF(string addr, int i, float a);
    void    oscSendIIFF(string addr, int i, int j, float a, float b);
    
    
    

    
    
    
};