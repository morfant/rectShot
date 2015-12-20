//
//  ContactListener.h
//  cvBody_chainShape
//
//  Created by giy on 23/10/2015.
//
//


#include <iostream>
#include "Box2D.h"
#include "convertFunc.h"

#define HOST "localhost"
//#define HOST "192.168.1.103"
#define PORT 57120
#define RECV_PORT 20000


// ----Class definition----
class ContactListener : public b2ContactListener{

protected:

    // OSC
    ofxOscSender            sender;
    float                   forceMul;
    // Pushing force of virtual bullet

    
//    bool    pBODYcontactBOTTOM;
//    int     curContactPbodyIdx;
    int     fragIdxInContact;
    
public:
    // Birth and Death
    ContactListener();
    ~ContactListener();
        
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    
    // Get
//    bool    getPBODYcontactBOTTOM();
//    int     getCurContactPbodyIdx();
    int     getfragIdxInContact();
    
    
    // osc
    void    oscSendII(string addr, int i, int j);
    
    void    oscSendIFF(string addr, int i, float a, float b);
    void    oscSendIF(string addr, int i, float a);
    void    oscSendIIFF(string addr, int i, int j, float a, float b);
    
    
    

    
};