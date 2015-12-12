//
//  OscManager.cpp
//  cvBody_chainShape
//
//  Created by giy on 12/12/2015.
//
//

#include "OscManager.h"

OscManager::OscManager()
{

	sender.setup(HOST, PORT);
	receiver.setup(RECV_PORT);
    
    
    shotBallMade = false;
    isShot = false;
    butPressed = false;
    bodyHit = false;
    
    
    
    
}

OscManager::~OscManager()
{
    
}


void OscManager::oscRecv()
{
    // check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		if(m.getAddress() == "/laserPos"){
            isShot = true;
			shot_X = m.getArgAsInt32(0);
			shot_Y = m.getArgAsInt32(1);
            
            cout << shot_X << shot_Y << endl;
            
		}else if(m.getAddress() == "/button"){
			butMsg = m.getArgAsInt32(0);
            if (butMsg) butPressed = true;
		}
        
        
        else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}else{
					msg_string += "unknown";
				}
			}
		}
	}
    
    
}

void OscManager::oscSendMsg(string addr, float data)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addFloatArg(data);
    sender.sendMessage(m);
}


void OscManager::oscSendMsg2(string addr, ofVec2f data)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addFloatArg(data.x);
    m.addFloatArg(data.y);
    sender.sendMessage(m);
}
