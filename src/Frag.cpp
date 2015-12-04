//
//  Frag.cpp
//  cvBody_chainShape
//
//  Created by giy on 04/12/2015.
//
//
// ----Headers----
#include "Frag.h"


// ----Birth & Death----

Frag::Frag(b2World* aWorld, float x, float y, b2Vec2* vertices)
{
    
    mWorld = aWorld;
    posX = x;
    posY = y;
    
    for (int i = 0; i < 3; i++){
        mVertice[i] = vertices[i];
//        cout << "i : " << i << mVertice[i].x << " / " << mVertice[i].y << endl;
    }
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
	mBody = mWorld -> CreateBody(&myBodyDef);
    
	b2PolygonShape triangle;
    
//    mVertice[0].Set(0.0f, 0.0f);
//    mVertice[1].Set(1.0f, 0.0f);
//    mVertice[2].Set(0.0f, 1.0f);
    
	triangle.Set(mVertice, 3);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &triangle;
	myFixtureDef.density = 20.f;
    myFixtureDef.restitution = 0.5f;
    myFixtureDef.isSensor = false;
    mBody->CreateFixture(&myFixtureDef);
    
	
}

Frag::~Frag()
{
    mWorld->DestroyBody(mBody);
    
}


// getter & setter
float
Frag::getX()
{
    return posX;
}

float
Frag::getY()
{
    return posY;
}


b2World*
Frag::getWorld()
{
    return mWorld;
}

b2Body*
Frag::getBody()
{
    return mBody;
    
}

void
Frag::setX(float _posX)
{
    
}

void
Frag::setY(float _posY)
{
    
}


void
Frag::render()
{
    b2Vec2 pos = mBody->GetPosition();
    
    ofPushStyle();
    ofSetColor(0, 200, 255);
    ofFill();
    ofPushMatrix();
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
    ofTriangle(mVertice[0].x, mVertice[0].y, mVertice[1].x, mVertice[1].y, mVertice[2].x, mVertice[2].y);
    ofPopMatrix();
    ofPopStyle();
}


// Update & draw
void
Frag::update()
{
    
}


void
Frag::draw()
{
    
    
    
}