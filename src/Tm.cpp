//
//  Tm.cpp
//  cvBody_chainShape
//
//  Created by giy on 11/12/2015.
//
//

#include "Tm.h" //Tm means 'Target Machine"


// ----Birth & Death----

Tm::Tm(b2World* world, Faces* pBody, float time)
{
    enable = false;
    dupNum = 1;
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
Tm::setPbody(Faces* pBody)
{
    targetOrigin = pBody;
    
}

void
Tm::setDupNum(int num)
{
    dupNum = num;
}

void
Tm::start()
{
    enable = true;
}



float
Tm::getTimer()
{
    return timeInterval;
    
}


Faces*
Tm::getTargetBody()
{
    return targetOrigin;
}




// Duplicate Target
Faces*
Tm::dupPbody(Faces* pbody, float x, float y)
{
    
    // Get index
    int index = pbody->getIndex();
    cout << "in Tm dupPbody index: " << index << endl;
    
    
    // GET Centroid
    float pbx = pbody->getX();
    float pby = pbody->getY();
//    cout << "x: " << x << " / " << " y: " << y << endl;
    
    
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
    Faces * aPbody = new Faces(mWorld, &tVertice[0], kMAX_VERTICES, x, y, index, true, false, dupNum);
    aPbody->setFragOutlineColor(pbody->getFragOutlineColor());

    
    return aPbody;

    
}


// Update & draw
Faces*
Tm::update()
{
        
        float marginX = 150;
        float marginY = 200;
        float randXPos = ofRandom(marginX, ofGetWidth() - marginX);
    
        dupPosX = randXPos;
        dupPosY = marginY;
    
    
        curTime = ofGetElapsedTimeMillis();
    //    cout << "curTime: " << curTime << endl;
        
        if ((curTime - startTime) > timeInterval && dupNum != 0){
    //        cout << "time ellaplsed!" << endl;
    //        dupPbody(targetOrigin, dupPosX, dupPosY);
            
            startTime = curTime;
    //        cout << "startTime: " << startTime << endl;
            
            dupNum--;
            
            return dupPbody(targetOrigin, dupPosX, dupPosY);
        }else{
//            cout << "nn" << endl;
            return NULL; //Nothing is happened.
        }

}


bool
Tm::isEnd()
{
//    cout << dupNum << endl;
    if(dupNum == 0){
        return true;
    }else{
        return false;
    }
}



void
Tm::draw()
{
    
}






// Util

