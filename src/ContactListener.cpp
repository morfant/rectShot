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

    // Adjust pushing force of virtual bullet
    forceMul = 1000.f;

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
                
            case 11: // SuperBall
            {
                //                printf("A, Ball contact begin!\n");
                b2Body* ball = contact->GetFixtureA()->GetBody();
                b2Vec2 pos = ball->GetPosition();
                
                ofxOscMessage m;
                m.setAddress("/fromOF_Ball");
                m.addFloatArg(_toPixelX(pos.x));
                m.addFloatArg(_toPixelY(pos.y));
                sender.sendMessage(m);
                
                // Make pushing force
                b2Body* self = contact->GetFixtureA()->GetBody();
                b2Body* other = contact->GetFixtureB()->GetBody();
                b2Vec2 selfPos = self->GetPosition();
                b2Vec2 otherPos = other->GetPosition();
                
                //                cout << "self pos: " << selfPos.x << " / " << selfPos.y << endl;
                //                cout << "other pos: " << otherPos.x << " / " << otherPos.y << endl;
                
                other->ApplyForce(b2Vec2(
                                         (selfPos.x - otherPos.x) * forceMul,
                                         (otherPos.y - selfPos.y) * forceMul),
                                  selfPos);
            }
                break;
                
                
            case 2: // Polygon body
            {
//                printf("A, PB contact begin!\n");
                
                b2Body* self = contact->GetFixtureA()->GetBody();
                b2Body* other = contact->GetFixtureB()->GetBody();
                int otherIs = (int)other->GetUserData();
                
                if (otherIs == LEFT) {
                    //                    printf("Touch LEFT end.\n");
                }else if (otherIs == RIGHT){
                    //                    printf("Touch RIGHT end.\n");
                }else if (otherIs == TOP){
                    //                    printf("Touch TOP end.\n");
                }else if (otherIs == BOTTOM){
                    //                    printf("Touch BOTTOM end.\n");
                }
                
            }
                break;
                
            case 3: // Outline tracker of Polygon body
            {
                
//                printf("A, OT contact begin!\n");
                
                b2Body* self = contact->GetFixtureA()->GetBody();
                b2Body* other = contact->GetFixtureB()->GetBody();
                int otherIs = (int)other->GetUserData();
                
                if (otherIs == LEFT) {
                    //                    printf("Touch LEFT end.\n");
                }else if (otherIs == RIGHT){
                    //                    printf("Touch RIGHT end.\n");
                }else if (otherIs == TOP){
                    //                    printf("Touch TOP end.\n");
                }else if (otherIs == BOTTOM){
                    //                    printf("Touch BOTTOM end.\n");
                }else if (otherIs == BALL){
//                    printf("A, OT meet ball.\n");
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

            case 11: // SuperBall
            {
                //                printf("B, Ball contact begin!\n");
                b2Body* ball = contact->GetFixtureB()->GetBody();
                b2Vec2 pos = ball->GetPosition();
                
                ofxOscMessage m;
                m.setAddress("/fromOF_Ball");
                m.addFloatArg(_toPixelX(pos.x));
                m.addFloatArg(_toPixelY(pos.y));
                sender.sendMessage(m);
                
                
                // Make pushing force
                b2Body* self = contact->GetFixtureB()->GetBody();
                b2Body* other = contact->GetFixtureA()->GetBody();
                b2Vec2 selfPos = self->GetPosition();
                b2Vec2 otherPos = other->GetPosition();
                
                //                cout << "self pos: " << selfPos.x << " / " << selfPos.y << endl;
                //                cout << "other pos: " << otherPos.x << " / " << otherPos.y << endl;
                
                other->ApplyForce(b2Vec2(
                                         (selfPos.x - otherPos.x) * forceMul,
                                         (otherPos.y - selfPos.y) * forceMul),
                                  selfPos);
            }
                break;
                
            case 2: // Polygon body
            {
                b2Body* self = contact->GetFixtureB()->GetBody();
                b2Body* other = contact->GetFixtureA()->GetBody();
                int otherIs = (int)other->GetUserData();
                
                if (otherIs == LEFT) {
                    //                    printf("Touch LEFT end.\n");
                }else if (otherIs == RIGHT){
                    //                    printf("Touch RIGHT end.\n");
                }else if (otherIs == TOP){
                    //                    printf("Touch TOP end.\n");
                }else if (otherIs == BOTTOM){
                    //                    printf("Touch BOTTOM end.\n");
                }
                
            }
                break;
                
            case 3: // Outline tracker of Polygon body
            {
//                printf("B, OT contact begin!\n");
                
                b2Body* self = contact->GetFixtureB()->GetBody();
                b2Body* other = contact->GetFixtureA()->GetBody();
                int otherIs = (int)other->GetUserData();
                
                if (otherIs == LEFT) {
                    //                    printf("Touch LEFT end.\n");
                }else if (otherIs == RIGHT){
                    //                    printf("Touch RIGHT end.\n");
                }else if (otherIs == TOP){
                    //                    printf("Touch TOP end.\n");
                }else if (otherIs == BOTTOM){
                    //                    printf("Touch BOTTOM end.\n");
                }else if (otherIs == BALL){
//                    printf("B, OT meet ball.\n");
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
