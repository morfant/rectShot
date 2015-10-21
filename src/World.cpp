//
//  World.cpp
//  cvBody_001
//
//  Created by giy on 8/31/13.
//
//

#include "World.h"


World::World()
{
    
    gravity = b2Vec2(GRAVITY_X, GRAVITY_Y);
    mWorld = new b2World(gravity);

}

b2World*
World::getWorld()
{
    return mWorld;

}