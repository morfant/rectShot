//
//  colorTrack.h
//  cvBody_chainShape
//
//  Created by giy on 15/11/2015.
//
//

#ifndef __cvBody_chainShape__colorTrack__
#define __cvBody_chainShape__colorTrack__

// ----Preprocessor command----
#pragma once

#include <iostream>
#include "ofMain.h"
#include "ofxOpenCv.h"

#include "ofQuickTimeGrabber.h"



class ColorTrack{
public:
    // Birth and Death
    ColorTrack(int w = 640, int h = 480);
    ~ColorTrack();
    
    void setup();
    void update();
    void draw(int x, int y);
    
    void setGrabber(ofVideoGrabber* vgrab);

    void exit();
    
    ofVideoGrabber usbcam;
    
    ofxCvColorImage rgb,hsb;
    ofxCvGrayscaleImage hue,sat,bri,filtered;
    ofxCvContourFinder contours;
    
    int w,h;
    int findHue;
    
};


#endif /* defined(__cvBody_chainShape__colorTrack__) */
