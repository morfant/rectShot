//
//  PolygonBody.cpp
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Headers----
#include "PolygonBody.h"


// ----Birth & Death----

PolygonBody::PolygonBody(b2World* aWorld, b2Vec2* vertices, int maxVCount, float x, float y)
{
    
    mWorld = aWorld;
    posX = x;
    posY = y;
    maxVertexCount = maxVCount;
    
    for (int i = 0; i < maxVertexCount; i++) {
        mPts[i].x = _toWorldX(vertices[i].x);
        mPts[i].y = _toWorldY(vertices[i].y);
    }
    
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
//    myBodyDef.position.Set(_toWorldX(posX/2.f), _toWorldY(posY/2.f));
    
    myBodyDef.position.Set(0, 0);
    
	mBody = mWorld -> CreateBody(&myBodyDef);
    
//	b2PolygonShape myPolygonShape;
//    myPolygonShape.Set(mPts, maxVertexCount);

    b2ChainShape chain;
//    chain.CreateChain(mPts, 8);
    chain.CreateLoop(mPts, maxVertexCount);
	   
	b2FixtureDef myFixtureDef;
//	myFixtureDef.shape = &myPolygonShape;
	myFixtureDef.shape = &chain;
//	myFixtureDef.density = 1.f;
//    myFixtureDef.restitution = 0.5f;
    mBody->CreateFixture(&myFixtureDef);

	
}

PolygonBody::~PolygonBody()
{


}


// getter & setter
float
PolygonBody::getArea(b2Vec2* vertices, int maxVCount)
{
        int i,j;
        double area = 0;
        
        for (i = 0; i < maxVCount; i++) {
            j = (i + 1) % maxVCount;
            area += vertices[i].x * vertices[j].y;
            area -= vertices[i].y * vertices[j].x;
        }
        
        area /= 2;
        return(area < 0 ? -area : area);
}

float
PolygonBody::getX()
{
    return posX;
}

float
PolygonBody::getY()
{
    return posY;
}


b2World*
PolygonBody::getWorld()
{
    return mWorld;
}

b2Body*
PolygonBody::getBody()
{
    return mBody;

}

void
PolygonBody::setX(float _posX)
{
    
}

void
PolygonBody::setY(float _posY)
{

}


void
PolygonBody::setVertices(b2Vec2* vertices)
{
    for (int i = 0; i < maxVertexCount; i++) {
        mPts[i].x = _toWorldX(vertices[i].x);
        mPts[i].y = _toWorldY(vertices[i].y);
    }
    
    
	b2PolygonShape myPolygonShape;
    myPolygonShape.Set(mPts, maxVertexCount);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myPolygonShape;
	myFixtureDef.density = 1.f;
    myFixtureDef.restitution = 0.5f;
    mBody->CreateFixture(&myFixtureDef);
    
    
    

}

void
PolygonBody::renderAtBodyPosition()
{
    b2Vec2 pos = mBody->GetPosition();
    
    ofSetColor(0, 200, 255);
//    ofPushMatrix();
//    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
    ofBeginShape();

    for (int i = 0; i < maxVertexCount; i++) {
        ofVertex(_toPixelX(mPts[i].x), _toPixelY(mPts[i].y));
//        ofVertex(mPts[i].x, mPts[i].y);
    }
    
    ofEndShape();
//    ofPopMatrix();
}


// Update & draw
void
PolygonBody::update()
{
    
}


void
PolygonBody::draw()
{
   


}