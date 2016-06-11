//
//  Box.cpp
//  emptyExample
//
//  Created by Gangil Yi on 8/13/13.
//
//

// ----Headers----
#include "Box.h"


// ----Birth & Death----

Box::Box(b2World* aWorld, float x, float y)
{
    
    mWorld = aWorld;
    posX = x;
    posY = y;
    
    isThereMbodybool = true;
    
    fragLifeTimeBySec = 6.0f;
    fragLifeTime = fragLifeTimeBySec * ofGetFrameRate();
    
    
//    posX = ofGetWidth()/2;
//    posY = ofGetHeight()/2;
    
	b2BodyDef myBodyDef;
//	myBodyDef.type = b2_staticBody;
	myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(_toWorldX(posX), _toWorldY(posY));
	mBody = mWorld -> CreateBody(&myBodyDef);
    
//    cout << "addr of aWorld in Box: " << aWorld << endl;

    
	b2PolygonShape myPolygonShape;
	myPolygonShape.SetAsBox(_toWorldScale(size)/2.f, _toWorldScale(size)/2.f);
	
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &myPolygonShape;
	myFixtureDef.density = 20.f;
    myFixtureDef.restitution = 0.8f;
//    myFixtureDef.isSensor = true;
    mBody->CreateFixture(&myFixtureDef);

    
    // Frag setting
    fragOutlineColor = ofColor(255);
    
	
}

Box::~Box()
{
    if (isThereMbodybool){
        mWorld->DestroyBody(mBody);
    }
}


void
Box::clearFrags()
{
    if(!isThereMbodybool){
        for (vector<Frag*>::iterator iter = mFrags.begin(); iter != mFrags.end(); iter++) {
            delete (*iter);
        }
    }
}


void
Box::renderFrags()
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


// getter & setter

bool
Box::getIsThereMBody()
{
    return isThereMbodybool;
}


float
Box::getX()
{
    return posX;
}

float
Box::getY()
{
    return posY;
}


b2World*
Box::getWorld()
{
    return mWorld;
}

b2Body*
Box::getBody()
{
    return mBody;

}

void
Box::setX(float _posX)
{
    
}

void
Box::setY(float _posY)
{

}


void
Box::renderAtBodyPosition()
{
    
    if (isThereMbodybool){

        
        b2Vec2 pos = mBody->GetPosition();
        
        ofPushStyle();
        
    //    float alpha = (255 - age);
        float alpha = 255;
        
        if (toBlack){
            ofSetColor(0, 0, 0, MAX(0, alpha));
        }else{
            ofSetColor(255, 255, 255, MAX(0, alpha));
        }
        ofPushMatrix();
        ofTranslate(_toPixelX(pos.x), _toPixelY(pos.y));
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofRect(0, 0, size, size);
        ofPopMatrix();
        ofPopStyle();
    }else{
        renderFrags();
    }
    
}


// Update & draw
void
Box::update()
{
    age++;
}


void
Box::draw()
{
   


}


// Etc

void
Box::delMbody()
{
    //    mWorld->DestroyBody(mBody2);
    mWorld->DestroyBody(mBody);
    isThereMbodybool = false;
    
    //    oscSendIF("/pbBrek", index, fragNum);
}



bool
Box::IsInside(b2Vec2 p)
{
    /*  determine whether a point is inside a polygon or not
     *  polygon's vertices need to be sorted counterclockwise
     *  source :
     *      http://www.ecse.rpi.edu/~wrf/Research/Short_Notes/pnpoly.html
     */
    
    
    b2Vec2 pos = mBody->GetPosition();
    
    
    float dx = _toPixelX(pos.x);
//        cout << "dx: " << dx << endl;
    float dy = _toPixelY(pos.y);
//        cout << "dy: " << dy << endl;
    
    
    // Translate vertices
    tVertice[0].x = dx - size/2;
    tVertice[0].y = dy - size/2;

    tVertice[1].x = dx + size/2;
    tVertice[1].y = dy - size/2;

    tVertice[2].x = dx + size/2;
    tVertice[2].y = dy + size/2;

    tVertice[3].x = dx - size/2;
    tVertice[3].y = dy + size/2;
        
    
    // Check point in polygon
    bool isIn = false;
    for(size_t c = 0, d = N_VERTICE - 1; c < N_VERTICE; d = c++)
    {
        if(
           ( (tVertice[c].y > p.y) != (tVertice[d].y > p.y) ) &&
           (p.x < (tVertice[d].x - tVertice[c].x) * (p.y - tVertice[c].y) / (tVertice[d].y - tVertice[c].y) + tVertice[c].x) ){
            isIn = !isIn;
        }
    }
    return isIn;
}


void
Box::makeMvertice()
{

    int n = N_FRAGS / N_VERTICE;
    int t = size / n;
    
    for (int j = n * 0; j < n; j++){
        mVerticeDiv[j] = b2Vec2(0, t * j);
    }
    
    for (int j = n * 1; j < n * 2; j++){
        mVerticeDiv[j] = b2Vec2(t * (j - n * 1), t * n);
    }

    for (int j = n * 2; j < n * 3; j++){
        mVerticeDiv[j] = b2Vec2(t * n, t * (n * 3 - j));
    }

    for (int j = n * 3; j < n * 4; j++){
        mVerticeDiv[j] = b2Vec2(t * (n * 4 - j), 0);
    }
    
    mVerticeDiv[N_FRAGS] = mVerticeDiv[0];
    
    
    
//    mVerticeDiv[0] = b2Vec2(0, 0);
//    mVerticeDiv[1] = b2Vec2(0, size/n * 1);
//    mVerticeDiv[2] = b2Vec2(0, size/n * 2);
//
//    mVerticeDiv[3] = b2Vec2(0, size/n * 3);
//    mVerticeDiv[4] = b2Vec2(size/n * 1, size/n * 3);
//    mVerticeDiv[5] = b2Vec2(size/n * 2, size/n * 3);
//
//    mVerticeDiv[6] = b2Vec2(size/n * 3, size/n * 3);
//    mVerticeDiv[7] = b2Vec2(size/n * 3, size/n * 2);
//    mVerticeDiv[8] = b2Vec2(size/n * 3, size/n * 1);
//
//    mVerticeDiv[9] = b2Vec2(size/n * 3, 0);
//    mVerticeDiv[10] = b2Vec2(size/n * 2, 0);
//    mVerticeDiv[11] = b2Vec2(size/n * 1, 0);
//    
//    mVerticeDiv[12] = b2Vec2(0, 0);
    
    
//    for (int i = 0; i <= N_FRAGS; i++){
//        cout << i << " / " << mVerticeDiv[i].x << " : " << mVerticeDiv[i].y << endl;
//    }


}

float
Box::perp_dot(ofVec2f a, ofVec2f b)
{
    return (-1.f) * a.y * b.x + a.x * b.y;
}

float
Box::perp_dot(b2Vec2 a, b2Vec2 b)
{
    return (-1.f) * a.y * b.x + a.x * b.y;
}


// getter & setter
float
Box::getArea(b2Vec2* vertices, int maxVCount)
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


void
Box::breakBody()
{
    
    if (isThereMbodybool) delMbody();
    
    makeMvertice();
    
    float movX = (mBody->GetWorldCenter().x * BOX2D_SCALE);
    float movY = (mBody->GetWorldCenter().y * BOX2D_SCALE * (-1.f));
//    cout << "movX: " << movX << " / " << "movY: " << movY << endl;
    

    
    
    // Translate to (movX, movY)
    for (int i = 0; i <= N_FRAGS; i++){
        mVerticeDiv[i].x = mVerticeDiv[i].x + (movX + posX - size/2);
        mVerticeDiv[i].y = mVerticeDiv[i].y + (movY + posY - size/2);
    }
    
    
    float cx = posX + movX;
    float cy = posY + movY;
    
//    cout << "cx: " << cx << " / " << "cy: " << cy << endl;

    
    int fragIdx = 0;
    for (int i = 0; i < N_FRAGS; i++){
        
        b2Vec2 vertices[3];
        //        b2Vec2 a = b2Vec2(_toWorldX(movX), _toWorldY(movY));
        b2Vec2 a = b2Vec2(_toWorldX(cx), _toWorldY(cy));
        b2Vec2 b = b2Vec2(_toWorldX(mVerticeDiv[i].x), _toWorldY(mVerticeDiv[i].y));
        b2Vec2 c = b2Vec2(_toWorldX(mVerticeDiv[i+1].x), _toWorldY(mVerticeDiv[i+1].y));
        
        ofVec2f vAB(b.x - a.x, b.y - a.y);
        ofVec2f vBC(c.x - b.x, c.y - b.y);
        
        vAB.normalize();
        vBC.normalize();
        
        float d = perp_dot(vAB, vBC);
        
//        if(d < 0){
//                        cout << "RIGHT\n";
//        }else{
//                        cout << "LEFT\n";
//        }
        
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
        
        
        // If the area did not have minus value.
        if(getArea(&vertices[0], 3) > 0){
            Frag * aFrag = new Frag(mWorld, movX, movY, vertices, 0, fragIdx, fragOutlineColor);
            aFrag->setLifeLong(fragLifeTime); // Frag will die after n Frame. 0 means 'immortal'.
            mFrags.push_back(aFrag);
            fragIdx++;
        }
        
    }
    
    //    breakFrags();
//    pushForce(cx, cy);
    
    
}


void
Box::pushForce(float x, float y)
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

