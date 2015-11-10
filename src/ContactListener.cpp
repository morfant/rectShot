//
//  ContactListener.cpp
//  cvBody_chainShape
//
//  Created by giy on 23/10/2015.
//
//

#include "ContactListener.h"




ContactListener::ContactListener()
{
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    
}

ContactListener::~ContactListener()
{
}


void
ContactListener::BeginContact(b2Contact* contact)
{
    /* handle begin event */
//    printf("contact begin!\n");
    
    // Ball - userData = 1
    void* userData_A = contact->GetFixtureA()->GetBody()->GetUserData();
    if (userData_A) {

        switch ((int)userData_A) {
            case 1: // Ball
            {
//                printf("A, Ball contact begin!\n");
                b2Body* ball = contact->GetFixtureA()->GetBody();
                b2Vec2 pos = ball->GetPosition();
                
                ofxOscMessage m;
                m.setAddress("/fromOF_Ball");
                m.addFloatArg(_toPixelX(pos.x));
                m.addFloatArg(_toPixelY(pos.y));
                sender.sendMessage(m);
            }
                break;
                
            case 2: // Polygon body
            {
             
                
                b2Body* pBody = contact->GetFixtureA()->GetBody();
                b2Body* other = contact->GetFixtureB()->GetBody();
                
                if ((int)other->GetUserData() == LEFT) {
//                    printf("Touch LEFT end.\n");
                    pBody->SetLinearVelocity(b2Vec2(-10.f, 0));
                    
                }else if ((int)other->GetUserData() == RIGHT){
//                    printf("Touch RIGHT end.\n");
                    pBody->SetLinearVelocity(b2Vec2(10.f, 0));
                }else if ((int)other->GetUserData() == TOP){
//                    printf("Touch TOP end.\n");
                }else if ((int)other->GetUserData() == BOTTOM){
//                    printf("Touch BOTTOM end.\n");
                }
                
            }
                break;
                
            case TOP:
//                printf("A, touch TOP.\n");
                
                break;
                
            case BOTTOM:
//                printf("A, touch BOTTOM.\n");
                
                break;
                
            case LEFT:
//                printf("A, touch LEFT.\n");
                
                break;
                
            case RIGHT:
//                printf("A, touch RIGHT.\n");
                
                break;
                
                
            default:
                
                break;
        }
    }

    void* userData_B = contact->GetFixtureB()->GetBody()->GetUserData();
    if (userData_B) {
        switch ((int)userData_B) {
            case 1: // Ball
            {
//                printf("B, Ball contact begin!\n");
                b2Body* ball = contact->GetFixtureB()->GetBody();
                b2Vec2 pos = ball->GetPosition();
                
                ofxOscMessage m;
                m.setAddress("/fromOF_Ball");
                m.addFloatArg(_toPixelX(pos.x));
                m.addFloatArg(_toPixelY(pos.y));
                sender.sendMessage(m);
            }
                break;

            case 2: // Polygon body
            {
                b2Body* other = contact->GetFixtureA()->GetBody();
                
                if ((int)other->GetUserData() == LEFT) {
//                    printf("Touch LEFT end.\n");
                }else if ((int)other->GetUserData() == RIGHT){
//                    printf("Touch RIGHT end.\n");
                }else if ((int)other->GetUserData() == TOP){
//                    printf("Touch TOP end.\n");
                }else if ((int)other->GetUserData() == BOTTOM){
//                    printf("Touch RIGHT end.\n");
                }
                
            }
                break;
                
                
            default:
                
                break;
        }
    }
}



void
ContactListener::EndContact(b2Contact* contact)
{
    /* handle end event */
//    printf("contact end!\n");
    
    
}

void
ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    /* handle pre-solve event */
    
}

void
ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    /* handle post-solve event */
    
}
