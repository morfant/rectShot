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

PolygonBody::PolygonBody(b2World* aWorld, b2Vec2* vertices, int maxVCount, float xx, float yy, int idx)
{

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    
    index = idx;
    
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
//    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
//    myBodyDef.linearVelocity.Set(4.0f, 0);
	mBody = mWorld -> CreateBody(&myBodyDef);
//    mBody2 = mWorld -> CreateBody(&myBodyDef);
    
    
#ifdef POLYGON_BODY
    // Polygon body
	b2PolygonShape myPolygonShape;
    for (int i = 0; i < kMAX_VERTICES - 1; i++) {
        mPtsP[i].x = mPts[i].x * 1.0;
        mPtsP[i].y = mPts[i].y * 1.0;
    }
    
    myPolygonShape.Set(mPtsP, maxVertexCount);
#else
    // Chain loop body
    b2ChainShape chain;
    chain.CreateLoop(mPts, maxVertexCount);
#endif
    
    
    
	b2FixtureDef myFixtureDef;
//	b2FixtureDef myFixtureDef2;
    
#ifdef POLYGON_BODY
//	myFixtureDef2.shape = &myPolygonShape;
	myFixtureDef.shape = &myPolygonShape;
#else
	myFixtureDef.shape = &chain;

#endif
    
	myFixtureDef.density = 1.f;
    myFixtureDef.restitution = 0.8f;
    mBody->CreateFixture(&myFixtureDef);
    mBody->SetUserData((void*)pBodyUserData);
//    mBody->SetLinearVelocity(b2Vec2(1.f, 0));

    
//	myFixtureDef2.density = 1.f;
//    myFixtureDef2.restitution = 0.8f;
//    mBody2->CreateFixture(&myFixtureDef2);
//    mBody2->SetUserData((void*)pBodyUserData);
    
    
    // Set default status
    selected = false;
    defaultColor = ofColor(0, 200, 25);
    selectedColor = ofColor(200, 10, 20);
    
    
    // sub body
//	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0, 0);
	mBody2 = mWorld -> CreateBody(&myBodyDef);
    
	b2CircleShape myCircleShape;
	myCircleShape.m_p.Set(0, 0);
	myCircleShape.m_radius = _toWorldScale(100.f/2.f);
	
//	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myCircleShape;
	myFixtureDef.density = 100.f;
    myFixtureDef.restitution = 1.01f;
    myFixtureDef.friction = 0.7f;
    mBody2->CreateFixture(&myFixtureDef);
    

	
}

PolygonBody::~PolygonBody()
{
    ofxOscMessage m;
    m.setAddress("/fromOF_pBody");
    m.addIntArg(index);    
    m.addFloatArg(-1);
    m.addFloatArg(-1);
    
    sender.sendMessage(m);
    
    mWorld->DestroyBody(mBody);
    mWorld->DestroyBody(mBody2);
    mBody = NULL;
    mBody2 = NULL;
}

float
PolygonBody::perp_dot(ofVec2f a, ofVec2f b)
{
    return (-1.f) * a.y * b.x + a.x * b.y;
}

float
PolygonBody::perp_dot(b2Vec2 a, b2Vec2 b)
{
    return (-1.f) * a.y * b.x + a.x * b.y;
}


void
PolygonBody::breakBody(float x, float y)
{
    


    float movX = (mBody->GetWorldCenter().x * BOX2D_SCALE);
    float movY = (mBody->GetWorldCenter().y * BOX2D_SCALE * (-1.f));

    float cx = posX + movX;
    float cy = posY + movY;
//    float movX = _toPixelX(mBody->GetWorldCenter().x) + posX;
//    float movY = _toPixelY(mBody->GetWorldCenter().y) + posY;
    
    
//    float movX = posX;
//    float movY = posY;
    
    
    // get intervald virtices
    // divNum = sampling interval
    int sampledSize = kMAX_VERTICES/kSAMPLING_INTV;
    
    // Add first point of blob polygon shape.
    b2Vec2 first = b2Vec2(0, 0);
    first.x = mVertice[0].x + movX;
    first.y = mVertice[0].y + movY;
    mVerticeDiv[0] = first;
    
    // Add middle points of blob polygon shape.
    for (int i = 1; i < (sampledSize - 1); i++) {
        b2Vec2 temp = b2Vec2(0, 0);
        temp.x = mVertice[kSAMPLING_INTV * i].x + movX;
        temp.y = mVertice[kSAMPLING_INTV * i].y + movY;
        mVerticeDiv[i] = temp;
    }
    
    // Add end point of blob polygon shape
    b2Vec2 last = b2Vec2(0, 0);
    last.x = mVertice[kMAX_VERTICES - 1].x + movX;
    last.y = mVertice[kMAX_VERTICES - 1].y + movY;
    mVerticeDiv[sampledSize - 1] = last;
    
    for (int i = 0; i < sampledSize; i++){
        cout << i << ": " <<  mVerticeDiv[i].x << " / " << mVerticeDiv[i].y << endl;
    }
    


    for (int i = 0; i < kSAMPLING_INTV - 1; i++){
        b2Vec2 vertices[3];
//        b2Vec2 a = b2Vec2(_toWorldX(movX), _toWorldY(movY));
        b2Vec2 a = b2Vec2(_toWorldX(cx), _toWorldY(cy));
        b2Vec2 b = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
        b2Vec2 c = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));

//        b2Vec2 a = b2Vec2(1, 1);
//        b2Vec2 b = b2Vec2(2, 3);
//        b2Vec2 c = b2Vec2(0, 0);
        

        ofVec2f vh(-1, 0);
        ofVec2f vv(0, -1);
        ofVec2f vAB(b.x - a.x, b.y - a.y);
        ofVec2f vBC(c.x - b.x, c.y - b.y);
        ofVec2f vAC(c.x - a.x, c.y - a.y);
        
        vh.normalize();
        vv.normalize();
        vAB.normalize();
        vBC.normalize();
        vAC.normalize();

//        float d = perp_dot(b - a, c - b);
        float d = perp_dot(vAB, vBC);
        if(d < 0){
            cout << "RIGHT\n";
        }else{
            cout << "LEFT\n";
        }
        
        float angleHAB = acos(vh.dot(vAB));
        float angleHAC = acos(vh.dot(vAC));
        float angleVAB = acos(vv.dot(vAB));
        float angleVAC = acos(vv.dot(vAC));
        float angleABBC = acos(vAB.dot(vBC));
        
        
        cout << "angleHAB : " << (-1.f) * _toDegree(angleHAB) << endl;
        cout << "angleHAC : " << (-1.f) * _toDegree(angleHAC) << endl;

        cout << "angleVAB : " << (-1.f) * _toDegree(angleVAB) << endl;
        cout << "angleVAC : " << (-1.f) * _toDegree(angleVAC) << endl;
        
        cout << "angleAB-BC : " << (-1.f) * _toDegree(angleABBC) << endl;
        
        
        for (int j = 0; j < 3; j++){
//            vertices[0] = b2Vec2(_toWorldX(movX), _toWorldY(movY));
            vertices[0] = b2Vec2(_toWorldX(cx), _toWorldY(cy));
            vertices[1] = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
            vertices[2] = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));
        }

        // To keep CCW direction.
        if (d < 0){
            cout << "2 and 1 changed\n" << endl;
            vertices[1] = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));
            vertices[2] = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
        }
        
        
        cout << "triangle " << i << " : " << "\n" <<
        vertices[0].x << " / " << vertices[0].y << "\n" <<
        vertices[1].x << " / " << vertices[1].y << "\n" <<
        vertices[2].x << " / " << vertices[2].y << "\n" <<
        endl;
        
        
        mWorld->DestroyBody(mBody);

        Frag * aFrag = new Frag(mWorld, cx, cy, movX, movY, vertices);
        mFrags.push_back(aFrag);
    
    }
    
    
}

b2Vec2*
PolygonBody::getBreakArray()
{
    return mVerticeDiv;
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

void
PolygonBody::clearFrags()
{
    for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
        delete (*iter);
    }
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
//    return _toPixelX(mBody->GetPosition().x);
//    return (mBody->GetPosition().x);
    return posX;
}

float
PolygonBody::getY()
{
//    return _toPixelY(mBody->GetPosition().y);
//    return (mBody->GetPosition().y);
    return posY;
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
PolygonBody::renderFrags()
{
    for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
        (*iter)->render();
    }
    
}


void
PolygonBody::renderAtBodyPosition()
{


//    b2Vec2 pos2 = mBody2->GetPosition();
//    float32 pangle = mBody2->GetAngle();
//    cout << "x: " << (pos2.x * BOX2D_SCALE) << " / y: " << (pos2.y * BOX2D_SCALE * -1) << " / angle: " << pangle << endl;

//    mBody->SetTransform(pos2, pangle);
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
    ofSetColor(199, 199, 199);
    ofEllipse(0, 0, 50, 50);
    ofRotate(_toDegree(angle));
    ofBeginShape();

    for (int i = 0; i < maxVertexCount; i++) {
//        ofVertex(_tovPixelX(mPts[i].x), _tovPixelY(mPts[i].y));
        ofVertex(mPts[i].x * BOX2D_SCALE, mPts[i].y * BOX2D_SCALE * (-1.f) );
    }
    
    ofEndShape();
    ofPopMatrix();
    ofPopStyle();
    
    
    // Draw fragments
    renderFrags();
}


void PolygonBody::getSection()
{
    // Get each distance from current point to next point.
    for (int i = 0; i < (kMAX_VERTICES - 1); i++){
        
        float dist = ofDist(mVertice[i].x, mVertice[i].y, mVertice[i+1].x, mVertice[i+1].y);
        
        dists.push_back(dist);
        //cout << "dists: " << i << " -  " << dists[i] << endl;
        
        
        float incX = (mVertice[i + 1].x - mVertice[i].x) / dists[i];
        float incY = (mVertice[i + 1].y - mVertice[i].y) / dists[i];
        
        addDist.push_back(ofVec2f(incX, incY));
        //cout << "addDist: " << i << " x : " << addDist[i].x << " y : " << addDist[i].y << endl;
        
    }
}


// Update & draw
void
PolygonBody::update()
{
    
    b2Vec2 bodyPos = mBody->GetPosition();
//    cout << "bodypos: x: " << bodyPos.x << " y: " << bodyPos.y << endl;
//    cout << "bodypos: x: " << _toPixelX(bodyPos.x) << " y: " << _toPixelY(bodyPos.y) << endl;
    ofVec2f curPoint = ofVec2f(mVertice[curSection].x, mVertice[curSection].y);
    ofVec2f nextPoint = ofVec2f(
            mVertice[curSection].x + (addDist[curSection].x * (curPointofSection + 1)),
            mVertice[curSection].y + (addDist[curSection].y * (curPointofSection + 1)));
    
    float distBetweenPoint = ofDist(curPoint.x, curPoint.y, nextPoint.x, nextPoint.y);

    if (distBetweenPoint < dists[curSection]){
        
        ofxOscMessage m;
        m.setAddress("/fromOF_pBody");
        m.addIntArg(index);
        m.addFloatArg(bodyPos.x * BOX2D_SCALE + nextPoint.x);
        m.addFloatArg(bodyPos.y * BOX2D_SCALE + nextPoint.y);
        sender.sendMessage(m);
        
        
        ofPushStyle();
        ofSetColor(10, 200, 100);
        ofFill();
        
        ofPushMatrix();
        ofTranslate(bodyPos.x * BOX2D_SCALE, bodyPos.y * BOX2D_SCALE * (-1));
//        ofTranslate(curPoint.x, curPoint.y);
//        ofLine(0, 0, nextPoint.x - curPoint.x, nextPoint.y - curPoint.y);
//        ofLine(curPoint.x, curPoint.y, nextPoint.x, nextPoint.y);
//        ofCircle(curPoint.x, curPoint.y, 10.f);
        ofSetColor(200, 200, 150);
        ofCircle(nextPoint.x, nextPoint.y, 3.f);
        ofLine(curPoint.x, curPoint.y, nextPoint.x, nextPoint.y);
        //    ofPoint(0, 0, 0);
        
        ofPopMatrix();
        ofPopStyle();
        
        curPointofSection++;
//        cout << "curPointofSection: " << curPointofSection << endl;

    }else{
        curSection++;
//        cout << "curSection: " << curSection << endl;
        
        if (curSection == (kMAX_VERTICES - 1) ) {
            curSection = 0;
        }
        
        curPointofSection = 0;
    }
    
    mBody2->SetTransform(b2Vec2(nextPoint.x + (bodyPos.x * BOX2D_SCALE), nextPoint.y + (bodyPos.y * BOX2D_SCALE * (-1))), 0);
    
    b2Vec2 spos = mBody2->GetPosition();
//        cout << "spos: x: " << spos.x << " y: " << spos.y << endl;
   
    
}


void
PolygonBody::draw()
{
   


}