//
//  Tm.cpp
//  cvBody_chainShape
//
//  Created by giy on 11/12/2015.
//
//

#include "Tm.h" //Tm means 'Target Machine"


// ----Birth & Death----

Tm::Tm(b2World* world, PolygonBody* pBody, float time)
{
    
    mWorld = world;
    
    startTime = ofGetElapsedTimeMillis();
    
    timeInterval = time;
    targetOrigin = pBody;
    
}

Tm::~Tm()
{

}


// Get & Set
void
Tm::setTimer(float time){
    
    timeInterval = time;
    
}


void
Tm::setPbody(PolygonBody* pBody)
{
    targetOrigin = pBody;
    
}



float
Tm::getTimer()
{
    return timeInterval;
    
}


PolygonBody*
Tm::getTargetBody()
{
    return targetOrigin;
}


// Duplicate Target
PolygonBody*
Tm::dupPbody(PolygonBody* pbody, float x, float y)
{
    // Get index
    int index = pbody->getIndex();
    
    // GET Centroid
    float pbx = pbody->getX();
    float pby = pbody->getY();
    
    dupPosX = pbx;
    dupPosY = pby;
    //    cout << "pbx: " << pbx << " / " << " pby: " << pby << endl;
    
    
    float dx = x - pbx;
    float dy = y - pby;
    //    cout << "dx: " << dx << " / " << " dy: " << dy << endl;
    
    b2Vec2* pVertice;
    pVertice = pbody->getVertices();
    b2Vec2 tVertice[kMAX_VERTICES];
    
    // Translate vertices
    for (int i = 0; i < kMAX_VERTICES; i++) {
        
        tVertice[i].x = (pVertice[i].x + dx);
        tVertice[i].y = (pVertice[i].y + dy);
        //        cout << "tVertice: " << i << " : "<< tVertice[i].x << " / " << tVertice[i].y << endl;
    }

    
    // Duplicate pBody
    PolygonBody * aPbody = new PolygonBody(mWorld, &tVertice[0], kMAX_VERTICES, x, y, index);
    return aPbody;

    
}


// Update & draw
PolygonBody*
Tm::update()
{
    float marginX = 200;
    float marginY = 100;
    float randXPos = ofRandom(marginX, ofGetWidth() - marginX);
    
    curTime = ofGetElapsedTimeMillis();
//    cout << "curTime: " << curTime << endl;
    
    if ((curTime - startTime) > timeInterval){
//        cout << "time ellaplsed!" << endl;
//        dupPbody(targetOrigin, dupPosX, dupPosY);
        
        startTime = curTime;
//        cout << "startTime: " << startTime << endl;
        
        return dupPbody(targetOrigin, randXPos, dupPosY + marginY);
        
        
    }else{
        return NULL;
    }
    
}


void
Tm::draw()
{
    
}






// Util

