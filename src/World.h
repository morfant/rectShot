//
//  World.h
//  cvBody_001
//
//  Created by giy on 8/31/13.
//
//

#pragma once

#include "Box2D.h"
#include "ContactListener.h"

#define     GRAVITY_X       0.0
#define     GRAVITY_Y       -9.8


class World{

protected:
    b2Vec2          gravity;
    b2World*        mWorld;
    
    ContactListener*    contactlistener;

    
public:
    
    World();
    ~World();
    
    b2World*        getWorld();

};