//
//  Faces.cpp
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Headers----
#include "Faces.h"


// ----Birth & Death----
Faces::Faces()
{
    cout << "Dummy Faces initialized." << endl;

    isReal = false;
    isOriginal = true;
    
}


Faces::Faces(b2World* aWorld, b2Vec2* vertices,
    int maxVCount, float cx, float cy)
{

    for (int i = 0; i < kMAX_VERTICES; i++) {
        mVertice[i] = vertices[i];
    }
    
    mWorld = aWorld;
    posX = cx;
    posY = cy;
    maxVertexCount = maxVCount;
    index = 0;
    dupIndex = 0;
    isOriginal = 0;
    isBreaked = false;
    isReal = true;
    
    fragLifeTimeBySec = 12.0f;
    fragLifeTime = fragLifeTimeBySec * ofGetFrameRate();
    
    
    if (isReal){
        
        isAlive = true;
        isThereMbodybool = true;
        fragNum = kMAX_VERTICES/kSAMPLING_INTV;

        pBodyUserData = POLYGON_BODY;
        smallBodyUserData = OT_BODY;
        
        curSection = 0;
        curPointofSection = 0;

        // outline tarcker
        rotSpd = 1.0f;
        audioLen = 0.f;
        getSection();
        
        
        for (int i = 0; i < maxVertexCount; i++) {
            mPts[i].x = _toWorldX(mVertice[i].x);
            mPts[i].y = _toWorldY(mVertice[i].y);
        }
        
        
        b2BodyDef myBodyDef;
        // myBodyDef.type = b2_dynamicBody;
        myBodyDef.type = b2_staticBody;
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
    //  b2FixtureDef myFixtureDef2;
        
    #ifdef POLYGON_BODY
    //  myFixtureDef2.shape = &myPolygonShape;
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
        selected = true;
        //contactedColor = ofColor(0, 200, 25);
        contactedColor = ofColor(0, 200, 0);
//        normalColor = ofColor(0, 200, 25);
        normalColor = ofColor(0, 50, 120);
        outliner_rad = 5.f;
        fragOutlineColor = ofColor(255);
        
        isNewBorn = true;
    }
    
}


//USING ONE
Faces::Faces(b2World* aWorld, b2Vec2* vertices,
    int maxVCount, float cx, float cy, int _life, ofColor _fragFillColor)
{
    life = _life;
    fragFillColor = _fragFillColor;

    for (int i = 0; i < maxVCount; i++) {
        mVertice[i] = vertices[i];
    }
    
    mWorld = aWorld;
    posX = cx;
    posY = cy;
    maxVertexCount = maxVCount;
    index = 0;
    dupIndex = 0;
    isOriginal = 0;
    isBreaked = false;
    isReal = true;
    
    fragLifeTimeBySec = 14.0f; // 0 means no dying.
    fragLifeTime = fragLifeTimeBySec * ofGetFrameRate();
    
    
    if (isReal){
        
        isAlive = true;
        isThereMbodybool = true;
        fragNum = maxVertexCount/kSAMPLING_INTV;

        pBodyUserData = POLYGON_BODY;
        smallBodyUserData = OT_BODY;
        
        curSection = 0;
        curPointofSection = 0;

        // outline tarcker
        rotSpd = 1.0f;
        audioLen = 0.f;
        getSection();
        
        
        for (int i = 0; i < maxVertexCount; i++) {
            mPts[i].x = _toWorldX(mVertice[i].x);
            mPts[i].y = _toWorldY(mVertice[i].y);
        }
        
        
        b2BodyDef myBodyDef;
        // myBodyDef.type = b2_dynamicBody;
        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(0, 0);
    //    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
    //    myBodyDef.linearVelocity.Set(4.0f, 0);
        mBody = mWorld -> CreateBody(&myBodyDef);
    //    mBody2 = mWorld -> CreateBody(&myBodyDef);
        
        
    #ifdef POLYGON_BODY
        // Polygon body
        b2PolygonShape myPolygonShape;
        for (int i = 0; i < maxVertexCount - 1; i++) {
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
    //  b2FixtureDef myFixtureDef2;
        
    #ifdef POLYGON_BODY
    //  myFixtureDef2.shape = &myPolygonShape;
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
        selected = true;
        //contactedColor = ofColor(0, 200, 25);
        contactedColor = ofColor(0, 200, 0);
//        normalColor = ofColor(0, 200, 25);
        normalColor = ofColor(0, 50, 120);
        outliner_rad = 5.f;
        fragOutlineColor = ofColor(255);
        
        isNewBorn = true;
    }
    
}


Faces::Faces(b2World* aWorld, b2Vec2* vertices,
    int maxVCount, float xx, float yy, int idx,
    bool _isReal, bool isOrigin, int dupIdx)
{
    

    for (int i = 0; i < kMAX_VERTICES; i++) {
        mVertice[i] = vertices[i];
    }
    
    mWorld = aWorld;
    posX = xx;
    posY = yy;
    maxVertexCount = maxVCount;
    index = idx;
    dupIndex = dupIdx;
    isOriginal = isOrigin;
    isBreaked = false;
    isReal = _isReal;
    
    fragLifeTimeBySec = 12.0f;
    fragLifeTime = fragLifeTimeBySec * ofGetFrameRate();
    
    
    if (isReal){
        
        isAlive = true;
        isThereMbodybool = true;
        fragNum = kMAX_VERTICES/kSAMPLING_INTV;

    //	// open an outgoing connection to HOST:PORT
    //    sender = new ofxOscSender();
    //	sender->setup(HOST, PORT);
        
        
        // Set Userdata
//        if (isOriginal){
//            pBodyUserData = (index * 100) + POLYGON_BODY;
//        }else{
//            pBodyUserData = (index * 10000) + (dupIndex * 100) + POLYGON_BODY;
//        }
        pBodyUserData = POLYGON_BODY;
        smallBodyUserData = OT_BODY;
        
        curSection = 0;
        curPointofSection = 0;

        
        // outline tarcker
        rotSpd = 1.0f;
        audioLen = 0.f;
        getSection();
        
        
        
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
        selected = true;
        //contactedColor = ofColor(0, 200, 25);
        contactedColor = ofColor(0, 200, 0);
//        normalColor = ofColor(0, 200, 25);
        normalColor = ofColor(0, 50, 120);

        outliner_rad = 5.f;
        
        fragOutlineColor = ofColor(255);
        
        /*
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
        */
        
    //    cout << "count: " <<mWorld->GetBodyCount() << endl;
    //    cout << "list: " << mWorld->GetBodyList() << endl;
        
        isNewBorn = true;
    }
	
}

Faces::~Faces()
{
    
//    oscSendIFF("/pbDest", index, -1, -1);
    
    if (isThereMbodybool){
//        mWorld->DestroyBody(mBody2);
//        cout << "pb dest" << endl;
        mWorld->DestroyBody(mBody);
    }

}



float
Faces::perp_dot(ofVec2f a, ofVec2f b)
{
    return (-1.f) * a.y * b.x + a.x * b.y;
}

float
Faces::perp_dot(b2Vec2 a, b2Vec2 b)
{
    return (-1.f) * a.y * b.x + a.x * b.y;
}



bool
Faces::IsInside(b2Vec2 p)
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
Faces::breakBody(float hitX, float hitY)
{
    
    if (isThereMbodybool) delMbody();

    
    float movX = (mBody->GetWorldCenter().x * BOX2D_SCALE);
    float movY = (mBody->GetWorldCenter().y * BOX2D_SCALE * (-1.f));

    float cx = hitX + movX;
    float cy = hitY + movY;
    // float cx = posX + movX;
    // float cy = posY + movY;
//    float movX = _toPixelX(mBody->GetWorldCenter().x) + posX;
//    float movY = _toPixelY(mBody->GetWorldCenter().y) + posY;
    
    
//    float movX = posX;
//    float movY = posY;
    
    
    // get intervald virtices
    
    // Add first point of blob polygon shape.
    b2Vec2 first = b2Vec2(0, 0);
    first.x = mVertice[0].x + movX;
    first.y = mVertice[0].y + movY;
    mVerticeDiv[0] = first;
    
    // Add middle points of blob polygon shape.
    for (int i = 1; i < (fragNum - 1); i++) {
        b2Vec2 temp = b2Vec2(0, 0);
        temp.x = mVertice[kSAMPLING_INTV * i].x + movX;
        temp.y = mVertice[kSAMPLING_INTV * i].y + movY;
        mVerticeDiv[i] = temp;
    }
    
    // Add end point of blob polygon shape
    b2Vec2 last = b2Vec2(0, 0);
    last.x = mVertice[kMAX_VERTICES - 1].x + movX;
    last.y = mVertice[kMAX_VERTICES - 1].y + movY;
    mVerticeDiv[fragNum - 1] = last;
    
    for (int i = 0; i < fragNum; i++){
//        cout << i << ": " <<  mVerticeDiv[i].x << " / " << mVerticeDiv[i].y << endl;
    }
    

    int fragIdx = 0;
    for (int i = 0; i < fragNum - 1; i++){
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

        // If the area did not have minus value.
        if(getArea(&vertices[0], 3) > 0){
            // Frag * aFrag = new Frag(mWorld, movX, movY, vertices, index, fragIdx, fragOutlineColor);
            Frag * aFrag = new Frag(mWorld, movX, movY, vertices,
                index, fragIdx, fragOutlineColor, fragFillColor, FRAG2_CATE_BIT, FRAG2_MASK_BIT);
            aFrag->setLifeLong(fragLifeTime); // Frag will die after n Frame. 0 means 'immortal'.
            mFrags.push_back(aFrag);
            fragIdx++;
        }
    
    }
    
//    breakFrags();
    pushForce(cx, cy);
    
    
}


void
Faces::breakBody()
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
    
    // Add first point of blob polygon shape.
    b2Vec2 first = b2Vec2(0, 0);
    first.x = mVertice[0].x + movX;
    first.y = mVertice[0].y + movY;
    mVerticeDiv[0] = first;
    
    // Add middle points of blob polygon shape.
    for (int i = 1; i < (fragNum - 1); i++) {
        b2Vec2 temp = b2Vec2(0, 0);
        temp.x = mVertice[kSAMPLING_INTV * i].x + movX;
        temp.y = mVertice[kSAMPLING_INTV * i].y + movY;
        mVerticeDiv[i] = temp;
    }
    
    // Add end point of blob polygon shape
    b2Vec2 last = b2Vec2(0, 0);
    last.x = mVertice[kMAX_VERTICES - 1].x + movX;
    last.y = mVertice[kMAX_VERTICES - 1].y + movY;
    mVerticeDiv[fragNum - 1] = last;
    
    for (int i = 0; i < fragNum; i++){
//        cout << i << ": " <<  mVerticeDiv[i].x << " / " << mVerticeDiv[i].y << endl;
    }
    

    int fragIdx = 0;
    for (int i = 0; i < fragNum - 1; i++){
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

        // If the area did not have minus value.
        if(getArea(&vertices[0], 3) > 0){
            // Frag * aFrag = new Frag(mWorld, movX, movY, vertices, index, fragIdx, fragOutlineColor);
            Frag * aFrag = new Frag(mWorld, movX, movY, vertices,
                index, fragIdx, fragOutlineColor, fragFillColor, FRAG2_CATE_BIT, FRAG2_MASK_BIT);
            aFrag->setLifeLong(fragLifeTime); // Frag will die after n Frame. 0 means 'immortal'.
            mFrags.push_back(aFrag);
            fragIdx++;
        }
    
    }
    
//    breakFrags();
    pushForce(cx, cy);
    
    
}

//void
//Faces::breakFrags()
//{
//    for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
//        (*iter)->breakSelf();
//
//    }
//}


b2Vec2*
Faces::getBreakArray()
{
    return mVerticeDiv;
}


void
Faces::shot()
{
    if (life){
        life--;
        cout << "face life: " << life << endl;
        //Change normalColor as Hittingcolor
        ofColor hittingColor = ofColor(200, 50, 20);
        normalColor = hittingColor;
    }
}


void
Faces::setContact(bool cont)
{
    isContacted = cont;
}

void
Faces::setSelectState(bool isSelected)
{
    selected = isSelected;
}


int
Faces::getLife()
{
    return life;
}

bool
Faces::getSelectState()
{
    return selected;
}

bool
Faces::getIsThereMBody()
{
    return isThereMbodybool;
}

bool
Faces::getIsAlive()
{
    return isAlive;
}

bool
Faces::getIsOriginal()
{
    return isOriginal;
}

bool
Faces::getIsReal()
{
    return isReal;
}

bool
Faces::getIsNewBorn()
{
    return isNewBorn;
}


ofColor
Faces::getFragOutlineColor()
{
    return fragOutlineColor;
}

bool
Faces::getFragsRemain()
{
    return isFragsRemain;
}

vector<Frag*> *
Faces::getFrags()
{
    return &mFrags;
}

void
Faces::clearFrags()
{
    if(!isThereMbodybool){
        cout << "Faces clearFrags()" << endl;
        for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
            delete (*iter);
        }
    }
}

// getter & setter
float
Faces::getArea(b2Vec2* vertices, int maxVCount)
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
Faces::getX()
{
//    return _toPixelX(mBody->GetPosition().x);
//    return (mBody->GetPosition().x);
    return posX;
}

float
Faces::getY()
{
//    return _toPixelY(mBody->GetPosition().y);
//    return (mBody->GetPosition().y);
    return posY;
}

int
Faces::getIndex()
{
    return index;
}


int
Faces::getDupIndex()
{
    return dupIndex;
}

bool
Faces::getIsBreaked()
{
    return isBreaked;
}



b2Vec2
Faces::getVertex(int idx)
{
    return mPts[idx];
}


b2Vec2*
Faces::getVertices()
{
    return mVertice;
}


b2World*
Faces::getWorld()
{
    return mWorld;
}

b2Body*
Faces::getBody()
{
    return mBody;

}

//b2Body*
//Faces::getSmallBody()
//{
//    return mBody2;
//    
//}

void
Faces::setX(float _posX)
{
    
}

void
Faces::setY(float _posY)
{

}


void
Faces::setVertices(b2Vec2* vertices)
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
Faces::setAudioLen(float len)
{
    audioLen = len;
//    cout << "len: " << audioLen << endl;

    float outlineLen = 0;
    for (int i = 0; i < kMAX_VERTICES - 1; i++) {
        outlineLen +=
        ofDist(
               mVertice[i].x, mVertice[i].y,
               mVertice[i+1].x, mVertice[i+1].y);
    }
    cout << "OutlineLen: " << outlineLen << endl;
    
    // outlineLen/fps = sec
    float sec = outlineLen/35.f;
    cout << "sec: " << sec << endl;
    
    rotSpd = sec/audioLen;
    cout << "rotSpd: " << rotSpd << endl;
    
}


void
Faces::setContactColor(ofColor color)
{
    contactedColor = color;
}

void
Faces::setIsNewBorn(bool born)
{
    isNewBorn = born;
}


void
Faces::setIsBreaked(bool isBreak)
{
    isBreaked = isBreak;
}

void
Faces::setFragOutlineColor(ofColor foc)
{
    fragOutlineColor = foc;
}


void
Faces::setFragLifeTimeBySec(unsigned long long lifetime)
{
    fragLifeTimeBySec = lifetime;
    fragLifeTime = fragLifeTimeBySec * ofGetFrameRate();
}



void
Faces::delMbody()
{    
//    mWorld->DestroyBody(mBody2);
    mWorld->DestroyBody(mBody);
    isThereMbodybool = false;
    
//    oscSendIF("/pbBrek", index, fragNum);
}


void
Faces::pushForce(float x, float y)
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
Faces::renderFrags()
{
    if (!isThereMbodybool){
        
        if (mFrags.size()){
            for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end();) {
                bool fragIsAlive = (*iter)->update();
                
                if (!fragIsAlive) {
                    delete (*iter);
                    iter = mFrags.erase(iter);
                    
                    
                }else{
                    (*iter)->render();
                    iter++;
                }
            }
        }else{
//            cout << "All frags gone" << endl;
            isFragsRemain = false;
        }
        
    }
    
}


void
Faces::renderAtBodyPosition()
{
    if (isThereMbodybool){

        b2Vec2 pos = mBody->GetPosition();
//        cout << "pos: " << pos.x << pos.y <<endl;
        float32 angle = mBody->GetAngle();
        
    //    printf("pbody angle: %f\n", angle);
    //    printf("pbody pos: %f, %f\n", pos.x, pos.y);
    //    printf("pbody pos TO PIXEL: %f, %f\n", _tovPixelX(pos.x), _tovPixelY(pos.y));

        
        ofPushStyle();
    //    ofSetColor(0, 200, 25); //Set Polygon body color

//        if (!isContacted) {
            ofSetColor(normalColor);
//        }else{
//            ofSetColor(contactedColor);
//        }

        ofPushMatrix();
        ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y)); //Must use for image moving.
        
        ofRotate(_toDegree(angle));

        // Draw polygon shape
        ofBeginShape();
        for (int i = 0; i < maxVertexCount; i++) {
            ofVertex(mPts[i].x * BOX2D_SCALE, mPts[i].y * BOX2D_SCALE * (-1.f) );
        }
        
        ofEndShape();
        
        ofPopMatrix();
        ofPopStyle();

        //reset normal color
        normalColor = ofColor(0, 50, 120);

    }else{
        // Draw fragments
        renderFrags();
    }
}


void Faces::getSection()
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
Faces::update()
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
            // point
            // ofCircle(nextPoint.x, nextPoint.y, outliner_rad);
            
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
        
    }
}


void
Faces::draw()
{
   


}