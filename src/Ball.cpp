//
//  Ball.cpp
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Headers----
#include "Ball.h"


// ----Birth & Death----

Ball::Ball(b2World* aWorld, float x, float y)
{
    
    // Set Userdata
    ballUserdata = 1;
    
    mWorld = aWorld;
    posX = x;
    posY = y;
    
    radius = 40.f;
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
	mBody = mWorld -> CreateBody(&myBodyDef);
    
    mBody->SetUserData((void*)ballUserdata);
//    mBody->SetUserData(this);
    
    
	b2CircleShape myCircleShape;
	myCircleShape.m_p.Set(0, 0);
	myCircleShape.m_radius = _toWorldScale(radius/2.f);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myCircleShape;	
	myFixtureDef.density = 1.f;
    myFixtureDef.restitution = 0.99f;
    myFixtureDef.friction = 0.7f;
    mBody->CreateFixture(&myFixtureDef);

	
}

Ball::~Ball()
{


}

// getter & setter
float
Ball::getX()
{
    return posX;
}

float
Ball::getY()
{
    return posY;
}


b2World*
Ball::getWorld()
{
    return mWorld;
}

b2Body*
Ball::getBody()
{
    return mBody;

}

void
Ball::setX(float _posX)
{
    
}

void
Ball::setY(float _posY)
{

}


void
Ball::renderAtBodyPosition()
{
    b2Vec2 pos = mBody->GetPosition();
    
    ofPushStyle();
    ofSetColor(250, 250, 0); // Set ball color
    ofFill();
    ofPushMatrix();
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
    ofEllipse(0, 0, radius, radius);
    ofPopMatrix();
    ofPopStyle();
}


// Update & draw
void
Ball::update()
{
    
}


void
Ball::draw()
{
   


}