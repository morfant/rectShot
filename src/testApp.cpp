#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);

    ofSetWindowPosition(0, 0);
    
    camUse = false;
	frameByframe = false;
    movPlay = false;
    movPlaySmall = false;
    info = true;
    grayPlay = false;
    drawBlob = true;
    
    pBodyIdx = 0;
    
    // Threshold inverting setting.
    inverting[0] = true; // for vidGrabber
    for (int i = 1; i < MOVNUM; i++) {
        inverting[i] = false;
    }
    
    
	// Uncomment this to show movies with alpha channels
	// fingerMovie.setPixelFormat(OF_PIXELS_RGBA);
    
    curMovie = 3; // 0 means using vidGrabber.
    
    movie[1].loadMovie("movies/aya.mov");
    movie[2].loadMovie("movies/aya2.mov");
    movie[3].loadMovie("movies/aya3.mov");
    movie[4].loadMovie("movies/ayaVater.mov");
    movie[5].loadMovie("movies/ayaundsister.mov");
    movie[6].loadMovie("movies/test.mov");
    
    
    
    // Get movie Width / Height
    movRes[0] = ofVec2f(CV_CAM_WIDTH, CV_CAM_HEIGHT);
    
    for (int i = 1; i < MOVNUM; i++){
        movRes[i].x = movie[i].getWidth();
        movRes[i].y = movie[i].getHeight();
        movie[i].setVolume(0.0f); // Mute sound of movie.
    }

    movie[curMovie].play();
    
    
    // OPEN CV
    // Using cam
    ofSetLogLevel(OF_LOG_VERBOSE);
//    vidGrabber.listDevices();
    vidGrabber.setDeviceID(0);
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(CV_CAM_WIDTH, CV_CAM_HEIGHT);
    
    // Using movie files
    colorImg.allocate(movRes[curMovie].x, movRes[curMovie].y);
    grayImage.allocate(movRes[curMovie].x, movRes[curMovie].y);
    grayBg.allocate(movRes[curMovie].x, movRes[curMovie].y);
    grayDiff.allocate(movRes[curMovie].x, movRes[curMovie].y);
    
    
    // Set cv threshold
    threshold[0] = TH_CAM;
    threshold[1] = TH_1;
    threshold[2] = TH_2;
    threshold[3] = TH_3;
    threshold[4] = TH_4;
    threshold[5] = TH_5;
    threshold[6] = TH_6;

    
    // Box2D
    aforce = 0.3f;
    touched = false;
    // Make & init containder - balls
    balls.clear();
    pBodies.clear();
    
    // World
    aWorld = new World();
    iWorld = aWorld -> getWorld();
    contactlistener = new ContactListener();
    iWorld->SetContactListener(contactlistener);

    /*
    // Wall - edge body
    left = new Wall(iWorld, b2Vec2(0, 0), b2Vec2(0, ofGetHeight()), ofGetHeight());
    right = new Wall(iWorld, b2Vec2(ofGetWidth(), 0), b2Vec2(ofGetWidth(), ofGetHeight()), ofGetHeight());
    floor = new Wall(iWorld, b2Vec2(0, ofGetHeight()), b2Vec2(ofGetWidth(), ofGetHeight()), ofGetWidth());
    ceil = new Wall(iWorld, b2Vec2(0, 0), b2Vec2(ofGetWidth(), 0), ofGetWidth());
    */
    

    // Wall - box
    int thickness = 40;
    left = new Wall(iWorld, 0, ofGetHeight()/2, thickness, ofGetHeight());
    right = new Wall(iWorld, ofGetWidth(), ofGetHeight()/2, thickness, ofGetHeight());
    floor = new Wall(iWorld, ofGetWidth()/2, ofGetHeight(), ofGetWidth(), thickness);
    ceil = new Wall(iWorld, ofGetWidth()/2, 0, ofGetWidth(), thickness);
    
    
    // vector init
    blobsPts.clear();
    blobsPtsDiv.clear();
    
    // make balls
//    for (int i = 0; i < 1; i++){
//        Ball * aBall = new Ball(iWorld, ofGetWidth()/2.0f + i, ofGetHeight()/2.0f - i);
//        balls.push_back(aBall);
//    }
    
    
    // OSC
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    // Box2D world update
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
    iWorld->Step(timeStep, velocityIterations, positionIterations);
    
    // Polygon bodies update
    for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
//        (*iter)->getBody()->SetLinearVelocity(b2Vec2(1.f, 0));
    }
    
    
    // opencv update
	ofBackground(200, 200, 200);
    bool bNewFrame = false;



    //cam
    if (curMovie == 0) {
        vidGrabber.update();
        bNewFrame = vidGrabber.isFrameNew();
    }else{
        //movie
        movie[curMovie].update();
        bNewFrame = movie[curMovie].isFrameNew();
    }

    

	if (bNewFrame){

        // Resize allocated images.
        colorImg.resize(movRes[curMovie].x, movRes[curMovie].y);
        grayImage.resize(movRes[curMovie].x, movRes[curMovie].y);
    
        if (curMovie == 0){// For vidGrabber
            vidGrabber.update();
            colorImg.setFromPixels(vidGrabber.getPixels(), movRes[curMovie].x, movRes[curMovie].y);
        }else{// For vidPlayer
            movie[curMovie].update();
            colorImg.setFromPixels(movie[curMovie].getPixels(), movRes[curMovie].x, movRes[curMovie].y);
        }
    
        grayImage.setFromColorImage(colorImg);
        grayImage.threshold(threshold[curMovie], inverting[curMovie]);
        contourFinder.findContours(grayImage, kMIN_BLOBAREA, (movRes[curMovie].x * movRes[curMovie].y)/3, kBLOBNUM, true);
        

        blobsVec.clear(); //Init containers with clearing.
        blobsVec = contourFinder.blobs; //Get vector<ofxCvBlob>
        cvBlobNum = blobsVec.size();
        
	}
    
    
    // OSC
    sendBlobsOSC();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0, 0, 0);
    
    // Draw movie.
    if (movPlay){
        if (curMovie == 0) {
            vidGrabber.draw(0, 0);
        }else{
            movie[curMovie].draw(0, 0);
        }
    }

    
    // Draw gray image.
    if (grayPlay){
        grayImage.draw(0,0);
    }

    // Draw contourFinder
    if (drawBlob){
        ofPushStyle();
        ofSetLineWidth(1.0);
        
        for (int i = 0; i < contourFinder.nBlobs; i++){
    //        contourFinder.blobs[i].draw(360,540);
            contourFinder.blobs[i].draw(0, 0);
        }
        ofPopStyle();
    }

    
    // Draw ball
    for (vector<Ball*>::iterator iter = balls.begin(); iter != balls.end(); iter++) {
        (*iter)->renderAtBodyPosition();
    }

    
    // Draw boxes
    for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
        (*iter)->renderAtBodyPosition();
    }
    
    
    // Draw Box2D walls
    left->renderAtBodyPosition();
    right->renderAtBodyPosition();
    floor->renderAtBodyPosition();
    ceil->renderAtBodyPosition();

    
    
    
    // Draw body at cv pos
    if(pBodies.size()){
        drawPolygonBodies();
//        polyLinerUpdate();
    }
    
//    
//    if (movPlaySmall){
//        // Right bottom rect.
//        ofPushMatrix();
//        ofTranslate(ofGetWidth() - 360, ofGetHeight() - 280);
//        ofScale(360.f / movRes[curMovie].x, 280.f / movRes[curMovie].y);
//        movie[curMovie].draw(0, 0);
//        ofPopMatrix();
//    }

    
    // Breakbody test
    for (int i = 0; i < kSAMPLING_INTV; i++) {
        for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
            
            bool isSelect = (*iter)->getSelectState();
            
            if(isSelect){
                b2Vec2* tvec = (*iter)->getBreakArray();
                b2Vec2 pBodypos = b2Vec2((*iter)->getX(), (*iter)->getY());
//                cout << pBodypos.x << " / " << pBodypos.y << endl;
                float cx = (*iter)->getX();
                float cy = (*iter)->getY();
                
                
                // draw points
                for(int i = 0; i < kSAMPLING_INTV; i++)
                {
                    ofSetColor(255);
                    ofFill();
                    ofEllipse(tvec[i].x, tvec[i].y, 2, 2);
                }
                
                // draw triangles
                ofSetColor(0, 0, 250);
                ofFill();
                
                ofPushMatrix();
//                ofTranslate((pBodypos.x) * BOX2D_SCALE, (pBodypos.y) * BOX2D_SCALE * (-1.f));
                ofTranslate((*iter)->getBody()->GetWorldCenter().x * BOX2D_SCALE, (*iter)->getBody()->GetWorldCenter().x * BOX2D_SCALE * (-1.f));
                
                //cout << (*iter)->getBody()->GetWorldCenter().x * BOX2D_SCALE << " / " << (*iter)->getBody()->GetWorldCenter().x * BOX2D_SCALE * (-1.f) << endl;
                
                
                ofEllipse(cx, cy, 100, 100);
                ofPopMatrix();
                
//                cout <<pBodypos.x << "/" << pBodypos.y << endl;
                
//                for (int i = 0; i < kSAMPLING_INTV - 1; i++){
//                    ofSetColor(255);
//                    ofNoFill();
//                    ofTriangle(_toPixelX(pBodypos.x), _toPixelY(pBodypos.y),
//                               tvec[i].x, tvec[i].y,
//                               tvec[i+1].x, tvec[i+1].y);
//                }
                
            }

        }
    }

	// finally, a report
    if (info){
        ofPushStyle();
        ofSetHexColor(0x00ffaa);
        stringstream reportStr;
        reportStr << "Mov : " << curMovie+1 << endl
        << "threshold " << threshold[curMovie] << " (press: +/-)" << endl
        << "press ' ' to invert" << endl
        << "num blobs found " << contourFinder.nBlobs << endl
        << "num polygonBodies " << pBodies.size() << endl
        << "aforce: " << aforce << endl
        << "FPS: " << ofGetFrameRate();
        ofDrawBitmapString(reportStr.str(), 30, 40);
        ofPopStyle();
    }
    
    
    // Touching check
    if (boxes.size() != 0){
        b2ContactEdge* contact = aBox->getBody()->GetContactList();
        
        if (contact) {
            b2Body* other = aBox->getBody()->GetContactList()->other;
            
            for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {

                bool isSelect = (*iter)->getSelectState();
                if ((*iter)->getBody() == other && touched == false){
                    
                    if (isSelect) (*iter)->setSelectState(false);
                    else (*iter)->setSelectState(true);
                    
                    // Latch
                    touched = true;
                }
            }
        }
    }
    
    
    
}

void testApp::drawPolygonBodies(){
    
    ofColor(0, 255, 0);
    ofFill();
    
    for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
        (*iter)->renderAtBodyPosition();
        (*iter)->update();
    }
    
    
}

void testApp::sendBlobsOSC()
{
    int i = 0;
    for(vector<ofxCvBlob>::iterator iter = blobsVec.begin(); iter != blobsVec.end(); iter++){
        
        ofxOscMessage m;
        m.setAddress("/fromOF");
        m.addFloatArg(i);
        m.addFloatArg(iter->centroid.x);
        m.addFloatArg(iter->centroid.y);
        m.addFloatArg(iter->area);
        sender.sendMessage(m);
        i++;
    }
}


void testApp::makePolygonBody(int blobNum){
    
    if (blobNum != 0){ // blobNum 0 means "Nothing selected".
        //clear containers
        blobsVec.clear();
        blobsPts.clear();
        blobsPtsDiv.clear();
        rBlobsPtsDiv.clear();
        blobCenterPos.clear();
        
        // get vector<ofxCvBlob>
        blobsVec = contourFinder.blobs;
        
        cvBlobNum = blobsVec.size();
        //blobsVec[0].pts = all points of 1st blob.
        
        if(cvBlobNum != 0){
            blobsPts = blobsVec[blobNum - 1].pts;
            cvBlobPos = blobsVec[blobNum - 1].centroid;
        }
        
        //        for (int i = 0; i < blobsPts.size(); i++){
        //            cout << "blobsPts[" << i << "] : " << blobsPts[i] << endl;
        //        }
        
        
        if(blobsPts.size() > 0){
            
            // divNum = sampling interval
            if (blobsPts.size() > kMAX_VERTICES){
                divNum = blobsPts.size()/kMAX_VERTICES;
            }else{
                divNum = 1;
            }
            
            // Make blobsPtsDiv
            b2Vec2 temp = b2Vec2(0, 0);
            temp.x = blobsPts[0].x;
            temp.y = blobsPts[0].y;
            
            // Add first point of blob polygon shape.
            blobsPtsDiv.push_back(temp);
            
            for (int i = 1; i < (kMAX_VERTICES - 1); i++) {
                b2Vec2 temp = b2Vec2(0, 0);
                temp.x = blobsPts[divNum * i].x;
                temp.y = blobsPts[divNum * i].y;
                blobsPtsDiv.push_back(temp);
            }
            
            // Add end point of blob polygon shape
            temp.x = blobsPts[blobsPts.size() - 1].x;
            temp.y = blobsPts[blobsPts.size() - 1].y;
            blobsPtsDiv.push_back(temp);
            
            
            // Reset Box2d polygon bodies
//            if(pBodies.size() != 0) resetPolygonBody();
            
            // Make new Box2d polygon bodies
//            if(pBodies.size() == 0)
            makeBodyAtCvPosition();
            
        }
    }
    
    
}


void testApp::makeBodyAtCvPosition(){

    for (vector<b2Vec2>::iterator ir = blobsPtsDiv.end(); ir != blobsPtsDiv.begin(); ir--) {
        rBlobsPtsDiv.push_back(*(ir));
    }
    
//    cout << "b[0] / r[7] " << blobsPtsDiv[0].x << " / " << rBlobsPtsDiv[7].x << endl;
    
    
    if(getArea(&blobsPtsDiv[0], kMAX_VERTICES) > 0){ // If the area did not have minus value.
        PolygonBody * aPbody = new PolygonBody(iWorld, &blobsPtsDiv[0], kMAX_VERTICES, cvBlobPos.x, cvBlobPos.y, pBodyIdx);
        
//        printf("cvBlobPos x: %f, y: %f\n", cvBlobPos.x, cvBlobPos.y);
        
        pBodies.push_back(aPbody);
        pBodyIdx++;
    }
    
    
    
    // Reset blobs points vector
    blobsPts.clear();
    blobsPtsDiv.clear();
    
    
}

void testApp::resetPolygonBody(){
    
    // clear b2Body
    for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
        iWorld->DestroyBody((*iter)->getBody()); //b2Body*
    }
    
    // clear circle
    pBodies.clear();
    
}

// I don't know how it works.
float testApp::getArea(b2Vec2* vertices, int maxVCount){

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

// OSC
//--------------------------------------------------------------
void testApp::oscSendMsg(string addr, float data)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addFloatArg(data);
    sender.sendMessage(m);
}


void testApp::oscSendMsg2(string addr, ofVec2f data)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addFloatArg(data.x);
    m.addFloatArg(data.y);
    sender.sendMessage(m);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){

        // Movie select - 0 means using vidGrabber
		case '0':
            if (curMovie != 0) movie[curMovie].stop();
            curMovie = 0;
			break;
            
		case '1':
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 1;
            movie[curMovie].play();
			break;

		case '2':
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 2;
            movie[curMovie].play();
			break;

		case '3':
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 3;
            movie[curMovie].play();
			break;

		case '4':
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 4;
            movie[curMovie].play();
			break;

		case '5':
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 5;
            movie[curMovie].play();
			break;

		case '6':
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 6;
            movie[curMovie].play();
			break;
            
            
        // Toggle threshold inverting.
		case ' ':
            inverting[curMovie] = !inverting[curMovie];
			break;
            
		case '+':
			threshold[curMovie]++;
			if (threshold[curMovie] > 255) threshold[curMovie] = 255;
			break;
            
		case '=':
			threshold[curMovie] ++;
			if (threshold[curMovie] > 255) threshold[curMovie] = 255;
			break;
            
		case '-':
			threshold[curMovie]--;
			if (threshold[curMovie] < 0) threshold[curMovie] = 0;
			break;


        // Toggle gray image play
		case 'g':
            if (grayPlay) grayPlay = false;
            else grayPlay = true;
			break;

		case 'G':
            if (grayPlay) grayPlay = false;
            else grayPlay = true;
			break;
            
            
        // Toggle original movie play full screen
		case 'm':
            if (movPlay) movPlay = false;
            else{
                grayPlay = false;
                movPlay = true;
            }
			break;
            
		case 'M':
            if (movPlay) movPlay = false;
            else{
                grayPlay = false;
                movPlay = true;
            }
			break;
            
            
        // Toggle blobs drawing.
		case 'b':
            if (drawBlob) drawBlob = false;
            else drawBlob = true;
			break;
            
		case 'B':
            if (drawBlob) drawBlob = false;
            else drawBlob = true;
			break;

        // Toggle text info report.
		case 'i':
            if (info) info = false;
            else info = true;
			break;
            
		case 'I':
            if (info) info = false;
            else info = true;
			break;
            
        // Delete selected pbodies.
		case 'x':
            for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end();) {
                bool isSelected = (*iter)->getSelectState();
                
                if (isSelected) {
                    delete (*iter);
                    iter = pBodies.erase(iter);
                }else{
                    iter++;
                }
            }
//            printf("After delete size: %lu\n", pBodies.size());
			break;

        // Delete all polygon bodies
		case 'X':
            for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end();) {
                    delete (*iter);
                    iter = pBodies.erase(iter);

            }
//            printf("After delete size: %lu\n", pBodies.size());
			break;
            

        // Apply force to pBodies.
        case 't':
            for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                bool isSelected = (*iter)->getSelectState();
                
                if (isSelected) {
                    b2Vec2 pBodypos = b2Vec2((*iter)->getX(), (*iter)->getY());
                    (*iter)->getBody()->ApplyForce(
                                                   b2Vec2((ofGetMouseX() - pBodypos.x)*aforce, (pBodypos.y - ofGetMouseY())*aforce),
                                                   b2Vec2(ofGetMouseX(), ofGetMouseY()));
                }
            }
            
            break;

            
        case 'T':
            for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                
                b2Vec2 pBodypos = b2Vec2((*iter)->getX(), (*iter)->getY());
                //                printf("pBody x: %f, y: %f\n", pBodypos.x, pBodypos.y);
                
                (*iter)->getBody()->ApplyForce(
                                               b2Vec2((ofGetMouseX() - pBodypos.x)*aforce, (pBodypos.y - ofGetMouseY())*aforce),
                                               b2Vec2(ofGetMouseX(), ofGetMouseY()));
            }
            
            break;
            
            
        // Add ball
        case 'a':
            
            aBall = new Ball(iWorld, ofGetMouseX(), ofGetMouseY());
            balls.push_back(aBall);
            
            break;
            
            
        // Clear balls
        case 'c':

            // clear b2Body
            for (vector<Ball*>::iterator iter = balls.begin(); iter != balls.end(); iter++) {
                iWorld->DestroyBody((*iter)->getBody());
            }
            
            // clear circle
            balls.clear();
			break;
            
        case OF_KEY_UP:
            aforce = aforce + 0.05f;
            break;

        case OF_KEY_DOWN:
            aforce = aforce - 0.05f;
            break;
            
        case 'h': //simulaton 'h'itting body
            for (vector<PolygonBody*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                bool isSelected = (*iter)->getSelectState();
                
                if (isSelected) {
                    b2Vec2 pBodypos = b2Vec2((*iter)->getX(), (*iter)->getY());
                    (*iter)->breakBody(pBodypos.x, pBodypos.y);
                    
                    cout << "world center: " << (*iter)->getBody()->GetWorldCenter().x << " / " << (*iter)->getBody()->GetWorldCenter().y << endl;
                    

                }
            }
            
            break;
            


	}
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
	switch (key){
    
    }

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    

//    printf("mouse x: %d, y: %d\n", ofGetMouseX(), ofGetMouseY());

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    // Add balls
//    Ball * aBall = new Ball(iWorld, x, y);
//    balls.push_back(aBall);


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    for (int i = 0; i < cvBlobNum; i++){
        if (blobsVec[i].boundingRect.inside(x, y)){
            selBlobRect = i + 1;
//            printf("Selected blob rect number: %d\n", selBlobRect);
            makePolygonBody(selBlobRect);
        }
    }
    
    selBlobRect = 0;
    
//    printf("Num of pbodies: %lu\n", pBodies.size());
    
    
    // For polygon body selection
    if (boxes.size() == 0){
        aBox = new Box(iWorld, ofGetMouseX(), ofGetMouseY());
        boxes.push_back(aBox);
    }
    

    


}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    // Reset booleans.
    selBlobRect = 0;
    touched = false;
    
    // Delete touch box(sensor object).
    delete boxes[0];
    boxes.clear();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
