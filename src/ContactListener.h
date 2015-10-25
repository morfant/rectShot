//
//  ContactListener.h
//  cvBody_chainShape
//
//  Created by giy on 23/10/2015.
//
//


#include <iostream>
#include "Box2D.h"
#include "ofxOsc.h"
#include "convertFunc.h"

#define HOST "localhost"
#define PORT 57120


// ----Class definition----
class ContactListener : public b2ContactListener{

protected:
    
    
    
public:
    // Birth and Death
    ContactListener();
    ~ContactListener();
        
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    
    // OSC
    ofxOscSender            sender;
    

    
};