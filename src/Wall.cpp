//
//  Wall.cpp
//  emptyExample
//
//  Created by Gangil Yi on 9/1/13.
//
//

// ----Headers----
#include "Wall.h"


// ----Birth & Death----
Wall::Wall()
{
    cout << "NOT INITIALIZED!!!" << endl;
}


//Wall::Wall(b2World* aWorld, b2Vec2 beginPoint, b2Vec2 endPoint, float length)
Wall::Wall(b2World* aWorld, int x, int y, float w, float h)
{
    mWorld = aWorld;
    
//    cout << "addr of aWorld in Wall: " << aWorld << endl;
    /*
    mBeginPoint = beginPoint;
    mEndPoint = endPoint;
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(0, 0); //?
	mBody = mWorld -> CreateBody(&myBodyDef);
    
    
	b2EdgeShape myEdgeShape;
    b2Vec2 bBeginPoint = b2Vec2(_toWorldX(mBeginPoint.x), _toWorldY(mBeginPoint.y));
    
    b2Vec2 bEndPoint = b2Vec2(_toWorldX(mEndPoint.x), _toWorldY(mEndPoint.y));
    
	myEdgeShape.Set(bBeginPoint, bEndPoint);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myEdgeShape;
    myFixtureDef.density = 1.f;
    mBody->CreateFixture(&myFixtureDef);
*/

    posX = x;
    posY = y;
    
    //    posX = ofGetWidth()/2;
    //    posY = ofGetHeight()/2;
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_staticBody;
    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
	mBody = mWorld -> CreateBody(&myBodyDef);
    
    //    cout << "addr of aWorld in Box: " << aWorld << endl;
    
    
	b2PolygonShape myPolygonShape;
	myPolygonShape.SetAsBox(_toWorldScale(w)/2.f, _toWorldScale(h)/2.f);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myPolygonShape;
	myFixtureDef.density = 1.f;
    myFixtureDef.restitution = 0.5f;
    mBody->CreateFixture(&myFixtureDef);
	
    
}


Wall::~Wall()
{


}


// getter & setter
b2World*
Wall::getWorld()
{
    return mWorld;
}

b2Body*
Wall::getBody()
{
    return mBody;
}

b2Vec2
Wall::getBeginPoint()
{
    return mBeginPoint;
}

b2Vec2
Wall::getEndPoint()
{
    return mEndPoint;
}


void
Wall::setX(float _posX)
{
    
}

void
Wall::setY(float _posY)
{

}


void
Wall::renderAtBodyPosition()
{
    ofSetLineWidth(1.f);
    ofLine(mBeginPoint.x, mBeginPoint.y, mEndPoint.x, mEndPoint.y);
}


// Update & draw
void
Wall::update()
{
    
}


void
Wall::draw()
{
   


}