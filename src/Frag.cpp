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

Frag::Frag(b2World* aWorld, float mx, float my, b2Vec2* vertices, int pbIdx, int idx, ofColor outlineCol)
{

    // osc
//	sender.setup(HOST, PORT);

    // Set Userdata
    pBodyIndex = pbIdx;
    index = idx;
    fragUserData = (pBodyIndex * 10000) + (index * 100) + FRAG;
    
    lifeLong = 0;
    age = 0;
    isAlive = true;
    
    mWorld = aWorld;
    posX = mx;
    posY = my;
    
    outlineColor = outlineCol;
    

    
    
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
    mBody->CreateFixture(&myFixtureDef);
    mBody->SetUserData((void*)fragUserData);
    
//    oscSendIIFF("/fgBorn", pBodyIndex, index, posX, posY);

	
    isNewBorn = true;
}

Frag::~Frag()
{
    mWorld->DestroyBody(mBody);
    
}


// getter & setter
float
Frag::getX()
{

}

float
Frag::getY()
{

}

unsigned long long
Frag::getLifeLong()
{
    return lifeLong;
}


unsigned long long
Frag::getAge()
{
    return age;
}


bool
Frag::getIsNewBorn()
{
    return isNewBorn;
}


int
Frag::getIndex()
{
    return index;
}



b2World*
Frag::getWorld()
{
    return mWorld;
}

b2Body*
Frag::getBody(int index)
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
Frag::setLifeLong(unsigned long long life)
{
    lifeLong = life;
}


void
Frag::setAge(unsigned long long _age)
{
    age = _age;
}


void
Frag::setIsNewBorn(bool born)
{
    isNewBorn = born;
}

//void
//Frag::makeb2Tri(b2Vec2* vertices)
//{
//    b2Body* mBody;
//    b2BodyDef myBodyDef;
//	myBodyDef.type = b2_dynamicBody;
//    //    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
//    myBodyDef.position.Set(0, 0);
//	mBody = mWorld -> CreateBody(&myBodyDef);
//    
//	b2PolygonShape triangle;
//    
//    //    mVertice[0].Set(0.0f, 0.0f);
//    //    mVertice[1].Set(1.0f, 0.0f);
//    //    mVertice[2].Set(0.0f, 1.0f);
//    
//	triangle.Set(vertices, 3);
//	
//	b2FixtureDef myFixtureDef;
//	myFixtureDef.shape = &triangle;
//	myFixtureDef.density = 0.5f;
//    //    myFixtureDef.restitution = 0.01f;
//    //    myFixtureDef.friction = 0.7f;
//    myFixtureDef.isSensor = false;
//    mBody->CreateFixture(&myFixtureDef);
//    
//    mPartialofFrags.push_back(mBody);
//}


//float
//Frag::perp_dot(ofVec2f a, ofVec2f b)
//{
//    return (-1.f) * a.y * b.x + a.x * b.y;
//}
//
//void
//Frag::breakSelf()
//{
//
//    
//    b2Vec2 childVertice_0[3];
//    b2Vec2 childVertice_1[3];
//    b2Vec2 tVertices[3];
//    
//    childVertice_0[0] = mVertice[1];
//    childVertice_0[1] = mVertice[2];
//    childVertice_0[2] = b2Vec2(
//                               (mVertice[2].x - mVertice[0].x)/2,
//                               (mVertice[2].y - mVertice[0].y)/2
//                               );
//    
//    childVertice_1[0] = mVertice[0];
//    childVertice_1[1] = mVertice[1];
//    childVertice_1[2] = b2Vec2(
//                               (mVertice[2].x - mVertice[0].x)/2,
//                               (mVertice[2].y - mVertice[0].y)/2
//                               );
//    
//    
//    
//    //        b2Vec2 a = b2Vec2(_toWorldX(posX), _toWorldY(posY));
//    b2Vec2 a = b2Vec2(_toWorldX(childVertice_0[0].x), _toWorldY(childVertice_0[0].y));
//    b2Vec2 b = b2Vec2(_toWorldX(childVertice_0[1].x), _toWorldY(childVertice_0[1].y));
//    b2Vec2 c = b2Vec2(_toWorldX(childVertice_0[2].x), _toWorldY(childVertice_0[2].y));
//    
//    //        b2Vec2 a = b2Vec2(1, 1);
//    //        b2Vec2 b = b2Vec2(2, 3);
//    //        b2Vec2 c = b2Vec2(0, 0);
//    
//    
//    //        ofVec2f vh(-1, 0);
//    //        ofVec2f vv(0, -1);
//    ofVec2f vAB(b.x - a.x, b.y - a.y);
//    ofVec2f vBC(c.x - b.x, c.y - b.y);
//    //        ofVec2f vAC(c.x - a.x, c.y - a.y);
//    
//    //        vh.normalize();
//    //        vv.normalize();
//    vAB.normalize();
//    vBC.normalize();
//    //        vAC.normalize();
//    
//    //        float d = perp_dot(b - a, c - b);
//    float d = perp_dot(vAB, vBC);
//    if(d < 0){
//        cout << "RIGHT\n";
//    }else{
//        cout << "LEFT\n";
//    }
//    
//    //        float angleHAB = acos(vh.dot(vAB));
//    //        float angleHAC = acos(vh.dot(vAC));
//    //        float angleVAB = acos(vv.dot(vAB));
//    //        float angleVAC = acos(vv.dot(vAC));
//    //        float angleABBC = acos(vAB.dot(vBC));
//    //
//    //
//    //        cout << "angleHAB : " << (-1.f) * _toDegree(angleHAB) << endl;
//    //        cout << "angleHAC : " << (-1.f) * _toDegree(angleHAC) << endl;
//    //
//    //        cout << "angleVAB : " << (-1.f) * _toDegree(angleVAB) << endl;
//    //        cout << "angleVAC : " << (-1.f) * _toDegree(angleVAC) << endl;
//    //
//    //        cout << "angleAB-BC : " << (-1.f) * _toDegree(angleABBC) << endl;
//    
//    
//    for (int j = 0; j < 3; j++){
//        //            vertices[0] = b2Vec2(_toWorldX(posX), _toWorldY(posY));
//        tVertices[0] = b2Vec2(_toWorldX(childVertice_0[0].x), _toWorldY(childVertice_0[0].y));
//        tVertices[1] = b2Vec2(_toWorldX(childVertice_0[1].x), _toWorldY(childVertice_0[1].y));
//        tVertices[2] = b2Vec2(_toWorldX(childVertice_0[2].x), _toWorldY(childVertice_0[2].y));
//    }
//    
//    
//    // To keep CCW direction.
//    if (d < 0){
//        cout << "2 and 1 changed\n" << endl;
//        tVertices[1] = b2Vec2(_toWorldX(childVertice_0[2].x), _toWorldY(childVertice_0[2].y));
//        tVertices[2] = b2Vec2(_toWorldX(childVertice_0[1].x), _toWorldY(childVertice_0[1].y));
//    }
//
//    
//    
//    // Delete current body
////    mWorld->DestroyBody(mPartialofFrags[0]);
//    
//
////    if (d < 0)
//    
//    cout <<
//    "index: " << 0 << " : " << _toPixelX(tVertices[0].x) << " / " << _toPixelY(tVertices[0].y) << "\n" <<
//    "index: " << 1 << " : " << _toPixelX(tVertices[1].x)<< " / " << _toPixelY(tVertices[1].y) << "\n" <<
//    "index: " << 2 << " : " << _toPixelX(tVertices[2].x) << " / " << _toPixelY(tVertices[2].y) << "\n" <<
//    endl;
//    
//    
//    makeb2Tri(tVertices);
////    makeb2Tri(childVertice_1);
//    
//    
//    
//}

void
Frag::render()
{

    float alpha = 255.f;
    b2Vec2 pos = mBody->GetPosition();
    float32 angle = mBody->GetAngle();
    
    if(lifeLong){
        alpha = (((float)lifeLong-(float)age)/(float)lifeLong) * 255.f;
    }

//    cout << pos.x << " / " << pos.y << endl;
//    cout << _toPixelX(pos.x)<< " / " << _toPixelY(pos.y) << endl;
//    ofPushMatrix();
//    ofTranslate(posX, posY);
//    ofSetColor(244, 122, 0);
////        ofEllipse(0, 0, 20, 20);
//    ofPopMatrix();
    
    ofPushStyle();
//    ofSetColor(0, 200, 255, alpha);
    
    ofSetColor(0, 0, 0, alpha);
    ofFill();
    ofPushMatrix();
    
    //Must use for image moving.
    ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
    ofRotate(_toDegree(angle));

    ofBeginShape();
    for (int i = 0; i < 3; i++) {
        ofVertex(mVertice[i].x * BOX2D_SCALE, mVertice[i].y * BOX2D_SCALE * (-1.f) );
    }
    ofEndShape();
    
    ofNoFill();
    ofSetColor(outlineColor);

    // Draw outline
    ofSetLineWidth(1.f);
    ofBeginShape();
    for (int i = 0; i < 3; i++) {
        ofVertex(mVertice[i].x * BOX2D_SCALE, mVertice[i].y * BOX2D_SCALE * (-1.f) );
    }
    ofEndShape();
    
    
    ofPopMatrix();
    ofPopStyle();
}


void
Frag::beOld()
{
    if (lifeLong){
        age++;
    }
    
    if (age > lifeLong){
        isAlive = false;
    }
    
}

// Update & draw
bool
Frag::update()
{
    beOld();
    
    return isAlive;
}


void
Frag::draw()
{
    
    
    
}


//osc

void
Frag::oscSendIFF(string addr, int i, float a, float b)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    m.addFloatArg(b);
    
//    sender.sendMessage(m);
    
}


void
Frag::oscSendIF(string addr, int i, float a)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    
//    sender.sendMessage(m);
    
}

void
Frag::oscSendIIFF(string addr, int i, int j, float a, float b)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addIntArg(j);
    m.addFloatArg(a);
    m.addFloatArg(b);
    
//    sender.sendMessage(m);
    
}