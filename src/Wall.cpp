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
    
    
    // Allot user data to each wall.
    if (w > h){
        if (y < (ofGetHeight()/2.f)) wallUserData = TOP;
        else wallUserData = BOTTOM;
        
    }else{
        if (x > (ofGetWidth()/2.f)) wallUserData = RIGHT;
        else wallUserData = LEFT;
    }
    
    
    mBody->SetUserData((void*)wallUserData);
    
    //    cout << "addr of aWorld in Box: " << aWorld << endl;
    
    
	b2PolygonShape myPolygonShape;
	myPolygonShape.SetAsBox(_toWorldScale(w)/2.f, _toWorldScale(h)/2.f);

    // Store vertice
    for (int i = 0; i < 4; i++){
        vertice[i] = myPolygonShape.GetVertex(i);
    }

	
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

b2Vec2
Wall::getVertex(int idx)
{
    return vertice[idx];
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
    /* // Edge body
    ofSetLineWidth(1.f);
    ofLine(mBeginPoint.x, mBeginPoint.y, mEndPoint.x, mEndPoint.y);
     */

    b2Vec2 pos = mBody->GetPosition();
    
//    printf("Wall pos: %f, %f\n", _toPixelX(pos.x), _toPixelY(pos.y));
//    
//    for (int i = 0; i < 4; i++){
//            printf("vtestice %d: %f, %f\n", i, _toPixelX(vertice[i].x), _toPixelY(vertice[i].y));
//    }
    
    float w = abs((vertice[0].x - vertice[1].x) * BOX2D_SCALE);
    float h = abs((vertice[0].y - vertice[3].y) * BOX2D_SCALE);

//    printf("Wall w: %f, h: %f\n", w, h);
    
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CENTER);
//    ofSetColor(100, 200, 200); // Wall color
    ofSetColor(0, 0, 0); // Wall color
    ofFill();
    ofPushMatrix();
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
    ofRect(0, 0, w, h);
    ofPopMatrix();
    ofPopStyle();
    
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