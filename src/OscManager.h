//
//  OscManager.h
//  cvBody_chainShape
//
//  Created by giy on 12/12/2015.
//
//

#include "ofxOsc.h"


#define HOST "localhost"
#define PORT 57120
#define RECV_PORT 20000


// ----Class definition----
class OscManager{
    
protected:
    
    ofxOscSender            sender;
    ofxOscReceiver          receiver;

    
    bool                    bodyHit;
    bool                    shotBallMade;
    bool                    isShot;
    int                     shot_X;
    int                     shot_Y;
    bool                    butPressed;
    int                     butMsg;

    
    
public:
    // Birth and Death
    OscManager();
    ~OscManager();
    
    
    // OSC
    void oscRecv();
    void oscSendMsg(string addr, float data);
    void oscSendMsg2(string addr, ofVec2f data);
    
    
    
    
};