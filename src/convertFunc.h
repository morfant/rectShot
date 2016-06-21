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


enum {
    //1111 1111
    WALL_CATE_BIT = 0x00FF,
    WALL_MASK_BIT = 0x00FF,

    //1000 0000
    CENTERWALL_CATE_BIT = 0x0080,
    CENTERWALL_MASK_BIT = 0x0080,

    //1100 0000
    DARKBOX_CATE_BIT = 0x00C0,
    DARKBOX_MASK_BIT = 0x00C0,

    //1110 0000
    BOX_CATE_BIT = 0x00E0,
    BOX_MASK_BIT = 0x00E0,

    //FRAG FROM BOX
    //0111 0000
    FRAG1_CATE_BIT = 0x0070,
    FRAG1_MASK_BIT = 0x0070,

    //FRAG FROM POLYGON BODY
    //0011 0000
    FRAG2_CATE_BIT = 0x0030,
    FRAG2_MASK_BIT = 0x0030,

    //BALL
    //0111 1000
    BALL_CATE_BIT = 0x0078,
    BALL_MASK_BIT = 0x0078

};


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

#define HOST "localhost"
//#define HOST "192.168.1.103" //supercollider HOST
#define PORT 57120
#define RECV_PORT 20000


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


