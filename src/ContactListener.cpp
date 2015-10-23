//
//  ContactListener.cpp
//  cvBody_chainShape
//
//  Created by giy on 23/10/2015.
//
//

#include "ContactListener.h"

void
ContactListener::BeginContact(b2Contact* contact)
{
    /* handle begin event */
}

void
ContactListener::EndContact(b2Contact* contact)
{ /* handle end event */
}

void
ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{ /* handle pre-solve event */
}

void
ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{ /* handle post-solve event */
}



ContactListener::ContactListener(b2World* aWorld, float x, float y)
{
    

    
	
}

ContactListener::~ContactListener()
{
    
    
}