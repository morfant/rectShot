//
//  OSCFunc.cpp
//  rectShot
//
//  Created by giy on 6/27/16.
//
//

#include "OSCFunc.h"


OSCFunc::OSCFunc()
{
    //OSC
    sender.setup(HOST, PORT);
    receiver.setup(RECV_PORT);

}

ofxOscReceiver*
OSCFunc::getReceiver()
{
    return &receiver;
}


void
OSCFunc::oscSendIFFF(string addr, int i, float a, float b, float c)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    m.addFloatArg(b);
    m.addFloatArg(c);
    sender.sendMessage(m);
}


void
OSCFunc::oscSendIIFF(string addr, int i, int j, float a, float b)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addIntArg(j);
    m.addFloatArg(a);
    m.addFloatArg(b);
    sender.sendMessage(m);
}


void
OSCFunc::oscSendIFF(string addr, int i, float a, float b)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    m.addFloatArg(b);
    sender.sendMessage(m);
}


void
OSCFunc::oscSendIF(string addr, int i, float a)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    sender.sendMessage(m);
}


void
OSCFunc::oscSendII(string addr, int i, int j)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addIntArg(j);
    sender.sendMessage(m);
}


void
OSCFunc::oscSendF(string addr, float i)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addFloatArg(i);
    sender.sendMessage(m);
}


void
OSCFunc::oscSendI(string addr, int i)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    sender.sendMessage(m);
}


