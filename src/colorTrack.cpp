//
//  colorTrack.cpp
//  cvBody_chainShape
//
//  Created by giy on 15/11/2015.
//
//

#include "colorTrack.h"


ColorTrack::ColorTrack(int w, int h)
{
    
//    ofSetLogLevel(OF_LOG_VERBOSE);
//    usbcam.listDevices();
    usbcam.setDeviceID(0);
    usbcam.initGrabber(w, h);

//    usbcam.setGrabber(ofPtr<ofxPS3EyeGrabber>(new ofxPS3EyeGrabber()));
//    usbcam.initGrabber(640, 480);
    
    //reserve memory for cv images
    rgb.allocate(w, h);
    hsb.allocate(w, h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    filtered.allocate(w, h);
}


ColorTrack::~ColorTrack(){
    
    
    
}

void ColorTrack::setGrabber(ofVideoGrabber* vd)
{
    usbcam = *vd;
}



void ColorTrack::update(){
    
    usbcam.update();
//    usbcam.update();
    
/*
    if (usbcam.isFrameNew()) {
        
        //copy webcam pixels to rgb image
        rgb.setFromPixels(usbcam.getPixels(), w, h);
        
        //mirror horizontal
        rgb.mirror(false, true);
        
        //duplicate rgb
        hsb = rgb;
        
        //convert to hsb
        hsb.convertRgbToHsv();
        
        //store the three channels as grayscale images
        hsb.convertToGrayscalePlanarImages(hue, sat, bri);
        
        //filter image based on the hue value were looking for
        for (int i=0; i<w*h; i++) {
            filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-5,findHue+5) ? 255 : 0;
        }
        filtered.flagImageChanged();
        
        //run the contour finder on the filtered image to find blobs with a certain hue
        contours.findContours(filtered, 50, w*h/2, 1, false);
    }
 */
}


void ColorTrack::draw(int x, int y)
{
    
    //draw all cv images
    usbcam.draw(x, y);
//    rgb.draw(x, y);
//    hsb.draw(640,0);
//    hue.draw(0,240);
//    sat.draw(320,240);
//    bri.draw(640,240);
//    filtered.draw(0,480);
//    contours.draw(0,480);
    
//    filtered.draw(x, y);
    
//    ofSetColor(255, 0, 0);
//    ofFill();
    
    //draw red circles for found blobs
//    for (int i=0; i<contours.nBlobs; i++) {
//        ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
//    }
    
    
}