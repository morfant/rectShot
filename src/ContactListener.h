//
//  ContactListener.h
//  cvBody_chainShape
//
//  Created by giy on 23/10/2015.
//
//


#include <iostream>
#include "Box2D.h"


// ----Class definition----
class ContactListener{
protected:
    
    
    
public:
    // Birth and Death
    ContactListener(b2World* world, float x, float y);
    ~ContactListener();
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    
};