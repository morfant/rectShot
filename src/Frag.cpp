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

Frag::Frag(b2World* aWorld, float mx, float my, b2Vec2* vertices,
    int pbIdx, int idx, ofColor outlineCol, ofColor _fillColor,
    int catBit, int maskBit)
{

    // osc
//  sender.setup(HOST, PORT);

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
    
    outlineDraw = false;
    outlineColor = outlineCol;
    isSafeTri = false;

    fillColor = _fillColor;
    
    
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
    
    float fragArea = getArea(&vertices[0], 3);
//    printf("fragArea: %f\n", fragArea);

    
    
    if(fragArea > 0){
        triangle.Set(mVertice, 3);
    }else{
        cout << "safe tri made!!!" << endl;
        safeTriangle[0].Set(0.f, 0.f);
        safeTriangle[1].Set(0.1f, 0.1f);
        safeTriangle[2].Set(0.f, 0.1f);
        triangle.Set(safeTriangle, 3);
        isSafeTri = true;
    }
    
    
//    try {
//        triangle.Set(mVertice, 3);
//    } catch (<#catch parameter#>) {
//        <#statements#>
//    }

    
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &triangle;
    myFixtureDef.density = 0.5f;

    //00000010(2) : not to collide with darkbox
    myFixtureDef.filter.categoryBits = catBit;
    myFixtureDef.filter.maskBits = maskBit;

//    myFixtureDef.restitution = 0.01f;
//    myFixtureDef.friction = 0.7f;
    mBody->CreateFixture(&myFixtureDef);
    mBody->SetUserData((void*)fragUserData);
    
//    oscSendIIFF("/fgBorn", pBodyIndex, index, posX, posY);

    
    isNewBorn = true;
}


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
    
    outlineDraw = false;
    outlineColor = outlineCol;
    isSafeTri = false;

    //Pale blue
    fillColor = ofColor(0, 50, 120);

    //White
    // fillColor = ofColor(255, 255, 255);
    
    
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
    
    float fragArea = getArea(&vertices[0], 3);
//    printf("fragArea: %f\n", fragArea);

    
    
    if(fragArea > 0){
        triangle.Set(mVertice, 3);
    }else{
        cout << "safe tri made!!!" << endl;
        safeTriangle[0].Set(0.f, 0.f);
        safeTriangle[1].Set(0.1f, 0.1f);
        safeTriangle[2].Set(0.f, 0.1f);
        triangle.Set(safeTriangle, 3);
        isSafeTri = true;
    }
    
    
//    try {
//        triangle.Set(mVertice, 3);
//    } catch (<#catch parameter#>) {
//        <#statements#>
//    }

	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &triangle;
	myFixtureDef.density = 0.5f;

    //00000010(2) : not to collide with darkbox
    myFixtureDef.filter.categoryBits = 0x0002;
    myFixtureDef.filter.maskBits = 0x0002;

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


bool
Frag::isContactSelf()
{
    for(b2Contact* c = mWorld->GetContactList(); c; c = c->GetNext()){
        if (c->GetFixtureA()->GetBody() == mBody){
//            cout << "A is me" << endl;
            return true;
        }else if(c->GetFixtureB()->GetBody() == mBody){
//            cout << "B is me" << endl;
            return true;
        }else{
            return false;
        }
        
    }
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

    fillColor.a = alpha;
    ofSetColor(fillColor);
    
    
    if (isSafeTri) ofSetColor(255, 0, 0);
    
//    ofSetColor(0, 0, 0, alpha);
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
    
    if(outlineDraw){
        ofNoFill();
        ofSetColor(outlineColor);

        // Draw outline
        ofSetLineWidth(1.f);
        ofBeginShape();
        for (int i = 0; i < 3; i++) {
            ofVertex(mVertice[i].x * BOX2D_SCALE, mVertice[i].y * BOX2D_SCALE * (-1.f) );
        }
        ofEndShape();
    }
    
    
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
//    if(isContactSelf()){
//        outlineDraw = true;
//        
//    }else{
//        outlineDraw = false;
//    }
    
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



float
Frag::getArea(b2Vec2* vertices, int maxVCount)
{
    int i, j;
    double area = 0;
    
    for (i = 0; i < maxVCount; i++) {
        j = (i + 1) % maxVCount;
        area += vertices[i].x * vertices[j].y;
        area -= vertices[i].y * vertices[j].x;
    }
    
    area /= 2;
    return(area < 0 ? -area : area);
}