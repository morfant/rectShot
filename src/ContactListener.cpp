//
//  ContactListener.cpp
//  cvBody_chainShape
//
//  Created by giy on 23/10/2015.
//
//

#include "ContactListener.h"




ContactListener::ContactListener()
{
}

ContactListener::~ContactListener()
{
}


void
ContactListener::BeginContact(b2Contact* contact)
{
    /* handle begin event */
//    printf("contact begin!\n");
    
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    if ((int)userDataA == 1) {
        printf("A, Die kugel kontact begonnen.\n");
    }
    
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
    if ((int)userDataB == 1) {
        printf("B, Die kugel kontact begonnen.\n");
    }

}

void
ContactListener::EndContact(b2Contact* contact)
{
    /* handle end event */
//    printf("contact end!\n");
    
    
}

void
ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    /* handle pre-solve event */
    
}

void
ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    /* handle post-solve event */
    
}
