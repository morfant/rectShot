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
    isAlive = true;
    isThereMbodybool = true;

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    
    index = idx;
    
    // Set Userdata
    pBodyUserData = POLYGON_BODY;
    smallBodyUserData = OT_BODY;
    
    curSection = 0;
    curPointofSection = 0;

    // Store for later use
    for (int i = 0; i < kMAX_VERTICES; i++) {
        mVertice[i] = vertices[i];
//        printf("vertice cp check from: %f, %f\n", vertices[i].x, vertices[i].y);
//        
//        printf("vertice cp check to: %f, %f\n", mVertice[i].x, mVertice[i].y);
    }
    
    // outline tarcker
    rotSpd = 1.0f;
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
    //defaultColor = ofColor(0, 200, 25);
    defaultColor = ofColor(0, 200, 0);
    //selectedColor = ofColor(200, 10, 20);
    selectedColor = ofColor(0, 0, 20);
    
    // sub body
	b2BodyDef myBodyDef2;
//	myBodyDef2.type = b2_dynamicBody;
	myBodyDef2.type = b2_staticBody;
//	myBodyDef2.type = b2_kinematicBody;
    myBodyDef2.position.Set(0, 0);
	mBody2 = mWorld -> CreateBody(&myBodyDef2);
    
    mBody2_rad = 5.f;
    

    // Circle
    b2CircleShape myCircleShape;
    myCircleShape.m_p.Set(0, 0);
    myCircleShape.m_radius = _toWorldScale(mBody2_rad/2.f);
    
    myFixtureDef.shape = &myCircleShape;
	myFixtureDef.density = 100.f;
    myFixtureDef.restitution = 1.01f;
    myFixtureDef.friction = 0.7f;
    mBody2->CreateFixture(&myFixtureDef);
    mBody2->SetUserData((void*)smallBodyUserData);
    
//    cout << "count: " <<mWorld->GetBodyCount() << endl;
//    cout << "list: " << mWorld->GetBodyList() << endl;

    
	
}

PolygonBody::~PolygonBody()
{
    ofxOscMessage m;
    m.setAddress("/fromOF_pBody");
    m.addIntArg(index);    
    m.addFloatArg(-1);
    m.addFloatArg(-1);
    
    sender.sendMessage(m);
    
    if (isThereMbodybool){
        mWorld->DestroyBody(mBody2);
        mWorld->DestroyBody(mBody);
    }

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



bool
PolygonBody::IsInside(b2Vec2 p)
{
    /*determine whether a point is inside a polygon or not
     *  polygon's vertices need to be sorted counterclockwise
     *  source :
     *      http://www.ecse.rpi.edu/~wrf/Research/Short_Notes/pnpoly.html
     */
    
    
    b2Vec2 pos = mBody->GetPosition();
    
    
    float dx = _toPixelX(pos.x) - ofGetWidth()/2.f;
//    cout << "dx: " << dx << endl;
    float dy = _toPixelY(pos.y) - ofGetHeight()/2.f;
//    cout << "dy: " << dy << endl;

    
    // Translate vertices
    for (int i = 0; i < maxVertexCount; i++) {
    
        tVertice[i].x = (mVertice[i].x + dx);
        tVertice[i].y = (mVertice[i].y + dy);
//        cout << "tVertice: " << i << " : "<< tVertice[i].x << " / " << tVertice[i].y << endl;

    }
    
    // Check point in polygon
    bool ans = false;
    for(size_t c = 0, d = kMAX_VERTICES - 1; c < kMAX_VERTICES; d = c++)
    {
        if(
           ( (tVertice[c].y > p.y) != (tVertice[d].y > p.y) ) &&
           (p.x < (tVertice[d].x - tVertice[c].x) * (p.y - tVertice[c].y) / (tVertice[d].y - tVertice[c].y) + tVertice[c].x) ){
            ans = !ans;
        }
    }
    return ans;
}


void
PolygonBody::breakBody()
{
    
    if (isThereMbodybool) delMbody();

    
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
//        cout << i << ": " <<  mVerticeDiv[i].x << " / " << mVerticeDiv[i].y << endl;
    }
    

    for (int i = 0; i < sampledSize - 1; i++){
        b2Vec2 vertices[3];
//        b2Vec2 a = b2Vec2(_toWorldX(movX), _toWorldY(movY));
        b2Vec2 a = b2Vec2(_toWorldX(cx), _toWorldY(cy));
        b2Vec2 b = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
        b2Vec2 c = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));

//        b2Vec2 a = b2Vec2(1, 1);
//        b2Vec2 b = b2Vec2(2, 3);
//        b2Vec2 c = b2Vec2(0, 0);
        

//        ofVec2f vh(-1, 0);
//        ofVec2f vv(0, -1);
        ofVec2f vAB(b.x - a.x, b.y - a.y);
        ofVec2f vBC(c.x - b.x, c.y - b.y);
//        ofVec2f vAC(c.x - a.x, c.y - a.y);
        
//        vh.normalize();
//        vv.normalize();
        vAB.normalize();
        vBC.normalize();
//        vAC.normalize();

//        float d = perp_dot(b - a, c - b);
        float d = perp_dot(vAB, vBC);
        if(d < 0){
//            cout << "RIGHT\n";
        }else{
//            cout << "LEFT\n";
        }
        
//        float angleHAB = acos(vh.dot(vAB));
//        float angleHAC = acos(vh.dot(vAC));
//        float angleVAB = acos(vv.dot(vAB));
//        float angleVAC = acos(vv.dot(vAC));
//        float angleABBC = acos(vAB.dot(vBC));
//        
//        
//        cout << "angleHAB : " << (-1.f) * _toDegree(angleHAB) << endl;
//        cout << "angleHAC : " << (-1.f) * _toDegree(angleHAC) << endl;
//
//        cout << "angleVAB : " << (-1.f) * _toDegree(angleVAB) << endl;
//        cout << "angleVAC : " << (-1.f) * _toDegree(angleVAC) << endl;
//        
//        cout << "angleAB-BC : " << (-1.f) * _toDegree(angleABBC) << endl;
        
        
        for (int j = 0; j < 3; j++){
//            vertices[0] = b2Vec2(_toWorldX(movX), _toWorldY(movY));
            vertices[0] = b2Vec2(_toWorldX(cx), _toWorldY(cy));
            vertices[1] = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
            vertices[2] = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));
        }

        // To keep CCW direction.
        if (d < 0){
//            cout << "2 and 1 changed\n" << endl;
            vertices[1] = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));
            vertices[2] = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
        }
        
        
//        cout << "triangle " << i << " : " << "\n" <<
//        vertices[0].x << " / " << vertices[0].y << "\n" <<
//        vertices[1].x << " / " << vertices[1].y << "\n" <<
//        vertices[2].x << " / " << vertices[2].y << "\n" <<
//        endl;

        Frag * aFrag = new Frag(mWorld, movX, movY, vertices);
        aFrag->setLifeLong(0); // Frag will die after n Frame. 0 means 'immortal'.
        mFrags.push_back(aFrag);
    
    }
    
//    breakFrags();
    pushForce(cx, cy);
    
    
}


//void
//PolygonBody::breakFrags()
//{
//    for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
//        (*iter)->breakSelf();
//
//    }
//}


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

bool
PolygonBody::isThereMBody()
{
    return isThereMbodybool;
}

bool
PolygonBody::getIsAlive()
{
    return isAlive;
}


vector<Frag*> *
PolygonBody::getFrags()
{
    return &mFrags;
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

int
PolygonBody::getIndex()
{
    return index;
}



b2Vec2
PolygonBody::getVertex(int idx)
{
    return mPts[idx];
}


b2Vec2*
PolygonBody::getVertices()
{
    return mVertice;
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

b2Body*
PolygonBody::getSmallBody()
{
    return mBody2;
    
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
PolygonBody::delMbody()
{    
    mWorld->DestroyBody(mBody2);
    mWorld->DestroyBody(mBody);
    isThereMbodybool = false;
}


void
PolygonBody::pushForce(float x, float y)
{
    
//    cout << "cx: " << x << " / " << "cy: " << y << endl;
    
    for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
        
        float forceMul = 0.005f;
        b2Vec2 centerPoint = (*iter)->getBody(0)->GetWorldCenter();
        
//        cout << "idx: " << i << " - centerPoint of frags: " << _toPixelX(centerPoint.x) << " / " << _toPixelY(centerPoint.y) << endl;
        
        (*iter)->getBody(0)->ApplyForce(
                                   b2Vec2( (x - _toPixelX(centerPoint.x)) * forceMul,
                                          -1.f * (y - _toPixelY(centerPoint.y)) * forceMul ),
                                   b2Vec2(x, y));
    }
    
}


void
PolygonBody::renderFrags()
{
    if (!isThereMbodybool){
        
        for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end();) {
            bool fragIsAlive = (*iter)->update();
            
            if (!fragIsAlive) {
                delete (*iter);
                iter = mFrags.erase(iter);
                isAlive = false;
                
            }else{
                (*iter)->render();
                iter++;
            }
        }
    }
    
}


void
PolygonBody::renderAtBodyPosition()
{
    if (isThereMbodybool){


    //    b2Vec2 pos2 = mBody2->GetPosition();
    //    float32 pangle = mBody2->GetAngle();
    //    cout << "x: " << (pos2.x * BOX2D_SCALE) << " / y: " << (pos2.y * BOX2D_SCALE * -1) << " / angle: " << pangle << endl;

    //    mBody->SetTransform(pos2, pangle);
        b2Vec2 pos = mBody->GetPosition();
//        cout << "pos: " << pos.x << pos.y <<endl;
        float32 angle = mBody->GetAngle();
        
    //    printf("pbody angle: %f\n", angle);
        
    //    printf("pbody pos: %f, %f\n", pos.x, pos.y);
    //    printf("pbody pos TO PIXEL: %f, %f\n", _tovPixelX(pos.x), _tovPixelY(pos.y));
//        

        ofFill();
        ofSetColor(255, 0, 0);
        ofEllipse(tVertice[0].x, tVertice[0].y, 10, 10);
        ofSetColor(255, 100, 0);
        ofEllipse(tVertice[10].x, tVertice[10].y, 10, 10);
        ofSetColor(255, 200, 0);
        ofEllipse(tVertice[50].x, tVertice[50].y, 10, 10);
//
        
        
        ofPushStyle();
    //    ofSetColor(0, 200, 25); //Set Polygon body color

        if (selected) {
            ofSetColor(selectedColor);
        }else{
            ofSetColor(defaultColor);
        }

        ofPushMatrix();
        ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y)); //Must use for image moving.
        
        
        
        
        
        
    //    ofSetColor(199, 199, 199);
    //    ofSetColor(0);
//        ofEllipse(0, 0, 50, 50);
        ofRotate(_toDegree(angle));
        ofBeginShape();
        for (int i = 0; i < maxVertexCount; i++) {
    //        ofVertex(_tovPixelX(mPts[i].x), _tovPixelY(mPts[i].y));
            ofVertex(mPts[i].x * BOX2D_SCALE, mPts[i].y * BOX2D_SCALE * (-1.f) );
        }
        
        ofEndShape();
        
        ofPopMatrix();
        ofPopStyle();
    }else{
    
        // Draw fragments
        
        renderFrags();
    }
}


void PolygonBody::getSection()
{
    if (isThereMbodybool){
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
}


// Update & draw
void
PolygonBody::update()
{
    if (isThereMbodybool){
        b2Vec2 bodyPos = mBody->GetPosition();
    //    cout << "bodypos: x: " << bodyPos.x << " y: " << bodyPos.y << endl;
    //    cout << "bodypos: x: " << _toPixelX(bodyPos.x) << " y: " << _toPixelY(bodyPos.y) << endl;
        ofVec2f curPoint = ofVec2f(mVertice[curSection].x, mVertice[curSection].y);
        ofVec2f nextPoint = ofVec2f(
                mVertice[curSection].x + (addDist[curSection].x * rotSpd * (curPointofSection + 1)),
                mVertice[curSection].y + (addDist[curSection].y * rotSpd * (curPointofSection + 1)));
        
        float distBetweenPoint = ofDist(curPoint.x, curPoint.y, nextPoint.x, nextPoint.y);

        if (distBetweenPoint < dists[curSection]){
            
//            ofxOscMessage m;
//            m.setAddress("/fromOF_pBody");
//            m.addIntArg(index);
//            m.addFloatArg(bodyPos.x * BOX2D_SCALE + nextPoint.x);
//            m.addFloatArg(bodyPos.y * BOX2D_SCALE + nextPoint.y);
//            sender.sendMessage(m);
            
            
            ofPushStyle();
            ofSetColor(10, 200, 100);
            ofFill();
            
            ofPushMatrix();
            ofTranslate(bodyPos.x * BOX2D_SCALE, bodyPos.y * BOX2D_SCALE * (-1));
            ofSetColor(20, 200, 150);
            ofCircle(nextPoint.x, nextPoint.y, mBody2_rad);
            
            // Outline
            vector<ofPoint> lines;
            for (int i = 0; i < curSection + 1; i++){
                ofPoint point = ofPoint(mVertice[i].x, mVertice[i].y);
                lines.push_back(point);
            }
            
            ofPolyline outline(lines);
            ofSetLineWidth(2.f);
            outline.draw();
            ofLine(curPoint.x, curPoint.y, nextPoint.x, nextPoint.y);
            
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
}


void
PolygonBody::draw()
{
   


}