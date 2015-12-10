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

Frag::Frag(b2World* aWorld, float x, float y, float mx, float my, b2Vec2* vertices)
{
    
    mWorld = aWorld;
    posX = x;
    posY = y;
    movX = mx;
    movY = my;
    
    for (int i = 0; i < 3; i++){
        mVertice[i] = vertices[i];
//        cout << "i : " << i << mVertice[i].x << " / " << mVertice[i].y << endl;
    }
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
//    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
    myBodyDef.position.Set(0, 0);
	mBody = mWorld -> CreateBody(&myBodyDef);
    
	b2PolygonShape triangle;
    
//    mVertice[0].Set(0.0f, 0.0f);
//    mVertice[1].Set(1.0f, 0.0f);
//    mVertice[2].Set(0.0f, 1.0f);
    
	triangle.Set(mVertice, 3);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &triangle;
	myFixtureDef.density = 0.5f;
//    myFixtureDef.restitution = 0.01f;
//    myFixtureDef.friction = 0.7f;
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
    float32 angle = mBody->GetAngle();


//    cout << pos.x << " / " << pos.y << endl;
//    cout << _toPixelX(pos.x)<< " / " << _toPixelY(pos.y) << endl;
    ofPushMatrix();
    ofTranslate(movX, movY);
    ofSetColor(244, 122, 0);
    ofEllipse(0, 0, 20, 20);
    ofPopMatrix();
    
    ofPushStyle();
    ofSetColor(0, 200, 255);
    ofFill();
    ofPushMatrix();
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y)); //Must use for image moving.
    ofRotate(_toDegree(angle));
//    ofTranslate((posX), (posY)); //Must use for image moving.
//    ofEllipse(0, 0, 10, 10);
    ofBeginShape();
    
    for (int i = 0; i < 3; i++) {
        //        ofVertex(_tovPixelX(mPts[i].x), _tovPixelY(mPts[i].y));
        ofVertex(mVertice[i].x * BOX2D_SCALE, mVertice[i].y * BOX2D_SCALE * (-1.f) );
    }
    
    ofEndShape();
    
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