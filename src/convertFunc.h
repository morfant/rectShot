//
//  convertFunc.h
//  cvBody_001
//
//  Created by giy on 9/1/13.
//
//

// ----Preprocessor command----
#pragma once

// ---- Macros ----
enum { //box2d Userdata
    BALL = 1,
    SUPER_BALL = 11,
    POLYGON_BODY = 2,
    OT_BODY = 3,
    FRAG = 4,
    TOP = 5,
    BOTTOM = 6,
    LEFT = 7,
    RIGHT = 8
};


enum {
    kCONTACT_LIMIT = 30,
    kMAX_VERTICES = 200,
    kSAMPLING_INTV = 10 //Fewer value makes more frags
};


// ----Common Headers----
#include "ofMain.h"
#include "ofxOsc.h"



// ----Define----
#define _round2(f)      (floor(f * 100.f) / 100.f)
#define _toDegree(a)	(-a / TWO_PI * 360.f) // Box2D use counter-clock wise radian
#define _toRadian(a)	(-a / 360.f * TWO_PI)

#define BOX2D_SCALE		80. // 1 meter of box2D world is 80 pixel on the screen. MUST FLOAT type!
#define _toPixelX(x)	(ofGetWidth() / 2.f + (BOX2D_SCALE * x))
#define _toPixelY(y)	(ofGetHeight() / 2.f - (BOX2D_SCALE * y))

// #define _toWorldX(x)	(_round2(((ofGetWidth() / 2.f) - x) / BOX2D_SCALE) * -1.f)
#define _toWorldX(x)	(_round2((x - (ofGetWidth() / 2.f)) / BOX2D_SCALE))
#define _toWorldY(y)	(_round2(((ofGetHeight() / 2.f) - y) / BOX2D_SCALE))

#define _toWorldScale(v) (_round2(v / BOX2D_SCALE))

#define _tovPixelX(x)	(720.f / 2.f + (BOX2D_SCALE * x))
#define _tovPixelY(y)	(480.f / 2.f - (BOX2D_SCALE * y))
#define _tovWorldX(x)	(_round2(((720.f / 2.f) - x) / BOX2D_SCALE) * -1.f)
#define _tovWorldY(y)	(_round2(((480.f / 2.f) - y) / BOX2D_SCALE))
