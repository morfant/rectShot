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

PolygonBody::PolygonBody(b2World* aWorld, b2Vec2* vertices, int maxVCount, float xx, float yy)
{

    // Set Userdata
    pBodyUserData = 2;
    
    curSection = 0;
    curPointofSection = 0;

    // Store for later use
    for (int i = 0; i < kMAX_VERTICES; i++) {
        mVertice[i] = vertices[i];
//        printf("vertice cp check from: %f, %f\n", vertices[i].x, vertices[i].y);
//        
//        printf("vertice cp check to: %f, %f\n", mVertice[i].x, mVertice[i].y);
    }
    getSection();
    
    
    mWorld = aWorld;
    posX = xx;
    posY = yy;
    maxVertexCount = maxVCount;
    
    // vec direction check - CCw
//    for (int i = 0; i < 4; i++){
//        printf("vec[%d] x: %f, y: %f\n", i, vertices[i].x, vertices[i].y);
//    }
    
    for (int i = 0; i < maxVertexCount; i++) {
        mPts[i].x = _toWorldX(vertices[i].x);
        mPts[i].y = _toWorldY(vertices[i].y);
    }
    
    
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0, 0);
//    myBodyDef.linearVelocity.Set(4.0f, 0);
	mBody = mWorld -> CreateBody(&myBodyDef);
    
    
#ifdef POLYGON_BODY
    // Polygon body
	b2PolygonShape myPolygonShape;
    myPolygonShape.Set(mPts, maxVertexCount);
#else
    // Chain loop body
    b2ChainShape chain;
    chain.CreateLoop(mPts, maxVertexCount);
#endif
    
    
    
	b2FixtureDef myFixtureDef;
    
#ifdef POLYGON_BODY
	myFixtureDef.shape = &myPolygonShape;
#else
	myFixtureDef.shape = &chain;
#endif
    
	myFixtureDef.density = 1.f;
    myFixtureDef.restitution = 0.8f;
    mBody->CreateFixture(&myFixtureDef);
    mBody->SetUserData((void*)pBodyUserData);
//    mBody->SetLinearVelocity(b2Vec2(1.f, 0));
    
    
    // Set default status
    selected = false;
    defaultColor = ofColor(0, 200, 25);
    selectedColor = ofColor(200, 10, 20);

	
}

PolygonBody::~PolygonBody()
{
    mWorld->DestroyBody(mBody);
    mBody = NULL;
}


void
PolygonBody::setSelectState(bool isSelected)
{
    selected = isSelected;
}


bool
PolygonBody::getSelectState()
{
    return selected;
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
    return _toPixelX(mBody->GetPosition().x);
}

float
PolygonBody::getY()
{
    return _toPixelY(mBody->GetPosition().y);
}


b2Vec2
PolygonBody::getVertex(int idx)
{
    return mPts[idx];
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
    float32 angle = mBody->GetAngle();
//    printf("pbody angle: %f\n", angle);
    
//    printf("pbody pos: %f, %f\n", pos.x, pos.y);
//    printf("pbody pos TO PIXEL: %f, %f\n", _tovPixelX(pos.x), _tovPixelY(pos.y));
    
    ofPushStyle();
//    ofSetColor(0, 200, 25); //Set Polygon body color

    if (selected) {
        ofSetColor(selectedColor);
    }else{
        ofSetColor(defaultColor);
    }

    ofPushMatrix();
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y)); //Must use for image moving.
    ofRotate(_toDegree(angle));
    ofBeginShape();

    for (int i = 0; i < maxVertexCount; i++) {
//        ofVertex(_tovPixelX(mPts[i].x), _tovPixelY(mPts[i].y));
        ofVertex(mPts[i].x * BOX2D_SCALE, mPts[i].y * BOX2D_SCALE * (-1.f) );
    }
    
    ofEndShape();
    ofPopMatrix();
    ofPopStyle();
}


void PolygonBody::getSection()
{
    // Get each distance from current point to next point.
    for (int i = 0; i < kMAX_VERTICES - 1; i++){
        
        float dist = ofDist(mVertice[i].x, mVertice[i].y, mVertice[i+1].x, mVertice[i+1].y);
        
        dists.push_back(dist);
        
        float incX = abs(mVertice[i+1].x - mVertice[i].x) / dists[i];
        float incY = abs(mVertice[i+1].y - mVertice[i].y) / dists[i];
        
        addDist.push_back(ofVec2f(incX, incY));
        
    }
}


// Update & draw
void
PolygonBody::update()
{
    
    ofVec2f curPoint = ofVec2f(mVertice[curSection].x, mVertice[curSection].y);
    ofVec2f nextPoint = ofVec2f(
            mVertice[curSection].x + (addDist[curSection].x * (curPointofSection + 1)),
            mVertice[curSection].y + (addDist[curSection].y * (curPointofSection + 1)));
    
    
    ofPushStyle();
    ofSetColor(10, 200, 100);
    ofFill();
    
    ofPushMatrix();
    ofTranslate(curPoint.x, curPoint.y);
    ofLine(0, 0, nextPoint.x, nextPoint.y);
    ofPoint(0, 0, 0);
    
    ofPopMatrix();
    ofPopStyle();
    
    float distBetweenPoint = ofDist(curPoint.x, curPoint.y, nextPoint.x, nextPoint.y);
    
    if (distBetweenPoint >= dists[curSection] ) {
        curSection++;
        if (curSection > kMAX_VERTICES) {
            curSection = 0;
        }
        
        curPointofSection = 0;
    }else{
        curPointofSection++;
    }

    
}


void
PolygonBody::draw()
{
   


}