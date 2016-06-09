//
//  Box.cpp
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Headers----
#include "Box.h"


// ----Birth & Death----

Box::Box(b2World* aWorld, float x, float y)
{
    
    mWorld = aWorld;
    posX = x;
    posY = y;
    
//    posX = ofGetWidth()/2;
//    posY = ofGetHeight()/2;
    
	b2BodyDef myBodyDef;
//	myBodyDef.type = b2_staticBody;
	myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
	mBody = mWorld -> CreateBody(&myBodyDef);
    
//    cout << "addr of aWorld in Box: " << aWorld << endl;

    
	b2PolygonShape myPolygonShape;
	myPolygonShape.SetAsBox(_toWorldScale(size)/2.f, _toWorldScale(size)/2.f);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myPolygonShape;
	myFixtureDef.density = 20.f;
    myFixtureDef.restitution = 0.8f;
//    myFixtureDef.isSensor = true;
    mBody->CreateFixture(&myFixtureDef);

	
}

Box::~Box()
{
    mWorld->DestroyBody(mBody);

}


// getter & setter
float
Box::getX()
{
    return posX;
}

float
Box::getY()
{
    return posY;
}


b2World*
Box::getWorld()
{
    return mWorld;
}

b2Body*
Box::getBody()
{
    return mBody;

}

void
Box::setX(float _posX)
{
    
}

void
Box::setY(float _posY)
{

}


void
Box::renderAtBodyPosition()
{
    b2Vec2 pos = mBody->GetPosition();
    
    ofPushStyle();
    
    if (toBlack){
        ofSetColor(0, 0, 0);
    }else{
        ofSetColor(255, 255, 255);
    }
    ofPushMatrix();
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(0, 0, size, size);
    ofPopMatrix();
    ofPopStyle();
}


// Update & draw
void
Box::update()
{
    
}


void
Box::draw()
{
   


}


