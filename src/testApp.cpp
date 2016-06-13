#include "testApp.h"

void testApp::exit()
{
    cout << "exiting.." << endl;
    
}

//--------------------------------------------------------------
void testApp::setup(){

    //Set projection width / height
    int pjW = 1920;
    int pjH = 1080;
    
    float winPosX = (pjW - ofGetWidth())/2.f;
    float winPosY = (pjH - ofGetHeight())/2.f;
    
    if (REALTIME){
//        ofSetWindowPosition(winPosX + 1440, winPosY - 90); //Extended desktop
        ofSetWindowPosition(1440, 0);

        info = false;
        inTitle = false;
        inLastScene = false;
        blackout = false;
    }else{
        ofSetWindowPosition(0, 0);
//        ofSetWindowPosition(0, 0);
        // ofSetWindowPosition((1440-800)/2, (900-600)/2);
        info = true;
        inTitle = false;
        inLastScene = false;
        blackout = false;
    }
    
  

    //OSC
	sender.setup(HOST, PORT);
	receiver.setup(RECV_PORT);
    ofSetFrameRate(60.f);
    
    title.loadImage("img/lonewolf_title.jpg");

    shotBallMade = false;
    isShot = false;
    butPressed = false;
    bodyHit = false;
    

    // MOVIE
    curMoviePlaying = true;
    
    // Playing source select
    movShow = false;
    grayShow = false;
    blobShow = false;
    
    movie[1].loadMovie("movies/ayaSum.mov");
    movie[2].loadMovie("movies/han720.mov");
    movie[3].loadMovie("movies/sewol2.mov");
//    movie[4].loadMovie("movies/pp480.mov");    
    movie[4].loadMovie("movies/pp720.mov");
    movie[5].loadMovie("movies/me.mov");
    
    movAmp[1] = 0.5f;
    movAmp[2] = 0.1f;
    movAmp[3] = 0.1f;
    movAmp[4] = 0.2f;
    movAmp[5] = 0.5f;

    movDrawPosX = (ofGetWidth()/2.f) - (movie[curMovie].width/2.f);
    movDrawPosY = (ofGetHeight()/2.f) - (movie[curMovie].height/2.f);
    
    curMovie = 1; // 0 means using vidGrabber.
    movie[curMovie].stop();
    
    
    // Get movie Width / Height
    movRes[0] = ofVec2f(CV_CAM_WIDTH, CV_CAM_HEIGHT);
    
    for (int i = 1; i < MOVNUM; i++){
        movRes[i].x = movie[i].getWidth();
        movRes[i].y = movie[i].getHeight();
    }
    
    
    // CV
    // Threshold inverting setting.
    inverting[0] = true; // for vidGrabber
    for (int i = 1; i < MOVNUM; i++) {
        inverting[i] = false;
    }
    
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
//    threshold[6] = TH_6;
//    threshold[7] = TH_7;
//    threshold[8] = TH_8;
//    threshold[9] = TH_9;

    
    // Box2D
    aforce = 0.3f;
    touched = false;
    balls.clear();
    pBodies.clear();
    pBodyIdx = 0;
    
    // World
    aWorld = new World();
    iWorld = aWorld -> getWorld();

    // Wall - box
    int thickness = 20.f;
    left = new Wall(iWorld, 0 - thickness/2, ofGetHeight()/2, thickness, ofGetHeight());
    right = new Wall(iWorld, ofGetWidth() + thickness/2, ofGetHeight()/2, thickness, ofGetHeight());
    floor = new Wall(iWorld, ofGetWidth()/2, ofGetHeight() + thickness/2, ofGetWidth(), thickness);
    ceil = new Wall(iWorld, ofGetWidth()/2, 0 - thickness/2, ofGetWidth(), thickness);
    center = new Wall(iWorld, ofGetWidth()/2, ofGetHeight()/2, thickness/2, ofGetHeight());
    
    // vector init
    blobsPts.clear();
    blobsPtsDiv.clear();
    
    
    //TARGET MAKER
    tmOpen = false;
    targetNum[0] = 10; //aya
    targetNum[1] = 10; //han
    targetNum[2] = 10; //sewol
    targetNum[3] = 10; //park
    targetNum[4] = 10; //me
    targetNum[5] = 10; //cam
    
    fragLifeTime[0] = 40.f; //aya
    fragLifeTime[1] = 40.f; //han
    fragLifeTime[2] = 40.f; //sewol
    fragLifeTime[3] = 40.f; //park
    fragLifeTime[4] = 40.f; //me
    fragLifeTime[5] = 20.f; //cam

    curStage = 0;
    stageStartTime = ofGetElapsedTimeMillis();
    OriginDestroyed = false;
    nextStageReady = false;
    
    Faces* initDumy = new Faces();
    tMan = new Tm(iWorld, initDumy, 1000); //1000 = 1 sec
    randFace = false;
    
    pBodyOutlineColor[0] = ofColor(10, 200, 100);
    pBodyOutlineColor[1] = ofColor(250, 0, 170);
    pBodyOutlineColor[2] = ofColor(200, 250, 10);
    pBodyOutlineColor[3] = ofColor(100, 250, 200);
    pBodyOutlineColor[4] = ofColor(0, 200, 10);
    
    isFirstShot[0] = false;
    isFirstShot[1] = false;
    isFirstShot[2] = false;
    isFirstShot[3] = false;
    isFirstShot[4] = false;
    isFirstShot[5] = false;
    
    bornPoint[0] = ofPoint(630, 397); //aya
    bornPoint[1] = ofPoint(553, 441); //han
    bornPoint[2] = ofPoint(554, 449); //sewol
    bornPoint[3] = ofPoint(230, 619); //park
    bornPoint[4] = ofPoint(629, 348); //me
    bornPoint[5] = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    


    //darkBoxes
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            Box* darkBox = new Box(iWorld, 64*(i+1), 64*(j+1));
            darkBox->setToBlack(true);
            darkBoxes.push_back(darkBox);
        }
    }


    
    // OSC
    blobsSynMade = false;
    
    //to play title music
    oscSendI("/title", 1);
    

    //AUDIO - for get 'amp'
    soundPlayer.loadSound(ofToDataPath("pp.wav"));
    soundPlayer.setLoop(true);
    soundPlayer.play();
    nBandsToGet = 1;
  

}

//--------------------------------------------------------------
void testApp::update(){
    
    //GET FFT just 1 band = approximately getting 'amp'.
    ofSoundUpdate();
    float * val = ofSoundGetSpectrum(nBandsToGet);
    fftSmoothed *= 0.96f;
    if (fftSmoothed < val[0]) fftSmoothed = val[0];
    
    
    if(!inTitle){
        // Box2D world update
        float32 timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        iWorld->Step(timeStep, velocityIterations, positionIterations);
        
        
        // opencv update
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
        
        
        //TARGET MAKER
        if (tmOpen){
            
            if (randFace){
                int rand = ofRandom(curStage + 1);
                tMan->setPbody(&pBodiesOriginalCopy[rand]);
            }

            
            Faces* tBody = tMan->update();
            
            if (tBody != NULL){
                pBodies.push_back(tBody);
//                cout << "Pbody duplicated." << endl;
            }
            
            if(tMan->isEnd()){
//                cout << "Next stage ready" << endl;
                nextStageReady = true;
            }
        }
        




        // SHOOTING CHECK
        if(shotBallMade){
            delete aBall;
            shotBallMade = false;
        }
        
        
        if (isShot){
            
            for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                if ( (*iter)->getIsThereMBody() ){
                    if ( (*iter)->IsInside(b2Vec2(shot_X, shot_Y)) ){
                        (*iter)->breakBody();
                        cout << "hit Face!" << endl;
                        bodyHit = true;
                    
    //                    break;
                    }
                }
            }

            
            for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
                if ( (*iter)->getIsThereMBody() ){
                    if ( (*iter)->IsInside(b2Vec2(ofGetMouseX(), ofGetMouseY())) ){
//                        (*iter)->breakBody();
                        (*iter)->breakBody(ofGetMouseX(), ofGetMouseY());
                        cout << "hit Box!" << endl;
                        bodyHit = true;
                        
                    }
                }
            }
            
            
            for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); iter++) {
                if ( (*iter)->getIsThereMBody() ){
                    if ( (*iter)->IsInside(b2Vec2(ofGetMouseX(), ofGetMouseY())) ){
                        //                        (*iter)->breakBody();
                        (*iter)->breakBody(ofGetMouseX(), ofGetMouseY());
//                        blackBoxes.erase(iter);
                        cout << "hit Black Box!" << endl;
                        bodyHit = true;
                    }
                }
            }
            
            
            if(!bodyHit){
            
                cout << "No hit!" << endl;
                aBall = new Ball(iWorld, shot_X, shot_Y, true);
                //                    balls.push_back(aBall);
                shotBallMade = true;
            }
            
            if (curStage == STAGE_NUM - 1 && !blackout) {
                blackout = true;
            }
            
            isShot = false;
            bodyHit = false;
        }
        

        // OSC
        oscRecv();
        
        if(OriginDestroyed){
            if(!blobsSynMade){
                oscSendI("/creatBlobSyn", blobsVec.size());
                blobsSynMade = true;
            }
            sendBlobsOSC();
        }
        
    }
    




}

//--------------------------------------------------------------
void testApp::draw(){
  
    // Box isinside test
//    for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
    // for (vector<Box*>::iterator iter = blackBoxes.begin();
    //     iter != blackBoxes.end(); iter++) {
        
    //     if ( (*iter)->IsInside(b2Vec2(ofGetMouseX(), ofGetMouseY())) ){
    //         cout << "Box isinsed!!" << endl;
    //     }else{
    //         cout << "NN" << endl;
    //     }
    // }

    
    //    ofBackground(255);
    ofBackground(0);
    
    
    
    
    

    // Draw gray image.
    if (grayShow){
        grayImage.draw(movDrawPosX, movDrawPosY);
    }
    
    // Draw movie.
    if (movShow){
        if (curMovie == 0) {
            //            vidGrabber.draw(0, 0);
            movDrawPosX = (ofGetWidth()/2.f) - (vidGrabber.width/2.f);
            movDrawPosY = (ofGetHeight()/2.f) - (vidGrabber.height/2.f);
            vidGrabber.draw(movDrawPosX, movDrawPosY);
            
        }else{
            movDrawPosX = (ofGetWidth()/2.f) - (movie[curMovie].width/2.f);
            movDrawPosY = (ofGetHeight()/2.f) - (movie[curMovie].height/2.f);
            movie[curMovie].draw(movDrawPosX, movDrawPosY);
        }
    }
    
    // Draw contourFinder
    if (blobShow){
        ofPushStyle();
        //        ofPushMatrix();
        //        ofTranslate(movDrawPosX, movDrawPosY);
        ofSetLineWidth(1.0);
        
        for (int i = 0; i < contourFinder.nBlobs; i++){
            //        contourFinder.blobs[i].draw(360,540);
            contourFinder.blobs[i].draw(movDrawPosX, movDrawPosY);
            
        }
        //        ofPopMatrix();
        ofPopStyle();
    }
    
    
    // Draw Box2D walls
    left->renderAtBodyPosition();
    right->renderAtBodyPosition();
    floor->renderAtBodyPosition();
    ceil->renderAtBodyPosition();
    center->renderAtBodyPosition();


    
    
    // Draw body at cv pos
    if(pBodies.size()) drawPolygonBodies();
    

//    if (!REALTIME) {
        // Draw ball
        for (vector<Ball*>::iterator iter = balls.begin(); iter != balls.end(); iter++) {
            (*iter)->renderAtBodyPosition();
        }


    if (darkBoxes.size()){
       // cout << "size of BLACK boxes: " << blackBoxes.size() << endl;
        // Draw black boxes
        float mulForce = 40.f;
        for (vector<Box*>::iterator iter = darkBoxes.begin(); iter != darkBoxes.end(); iter++) {
            (*iter)->renderAtBodyPosition();
            // (*iter)->update();
            (*iter)->getBody()->ApplyForce(b2Vec2(fftSmoothed * mulForce, fftSmoothed * mulForce), b2Vec2(ofGetWidth()/2, ofGetHeight()/2));
            // (*iter)->getBody()->ApplyLinearImpulse(b2Vec2(fftSmoothed, fftSmoothed), b2Vec2(ofGetWidth()/2, ofGetHeight()/2));
  
        }
    }

        // Draw boxes
    if (boxes.size()){
       // cout << "size of boxes: " << boxes.size() << endl;
        for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
            (*iter)->renderAtBodyPosition();
            (*iter)->update();
        }
    }
    
    if (blackBoxes.size()){
       // cout << "size of BLACK boxes: " << blackBoxes.size() << endl;
        // Draw black boxes
        for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); iter++) {
            (*iter)->renderAtBodyPosition();
            (*iter)->update();
        }
    }
   
//    }
    
    
    // LaserPointTracking point
    if (shotPointTest){
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofFill();
        ofPushMatrix();
        ofTranslate(shot_X, shot_Y);
        ofEllipse(0, 0, 20, 20);
        ofPopMatrix();
        ofPopStyle();
    }
    
    shotPointTest = false;
    
	// finally, a report
    if (info){
        ofPushStyle();
        ofSetHexColor(0x00ffaa);
        stringstream reportStr;
        reportStr << "Mov : " << curMovie << endl
        << "threshold " << threshold[curMovie] << " (press: +/-)" << endl
        << "press ' ' to invert" << endl
        << "num blobs found " << contourFinder.nBlobs << endl
        << "num polygonBodies " << pBodies.size() << endl
        << "aforce: " << aforce << endl
        << "FPS: " << ofGetFrameRate() << endl
        << "Sum of Area: " << (getFragsArea()/(ofGetWidth()*ofGetHeight()))*100.f << endl;
        ofDrawBitmapString(reportStr.str(), 30, 40);
        ofPopStyle();
    }

//    touchingCheck();
    
    
    if (inTitle){
//        printf("inTitle true.\n");
        movShow = false;
        blobShow = false;
        grayShow = false;
        
        title.draw(
                   ofGetWidth()/2.f - title.width/2.f,
                   ofGetHeight()/2.f-title.height/2.f);
    }
    
    if (blackout) {
        ofSetColor(0);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        oscSendI("/blackout", 1);
        for (int i = 1; i < STAGE_NUM - 1; i++) {
            movie[i].setVolume(0.f);
            movie[i].stop();
        }
//        printf("blackout!\n");
    }
 
    
    
    
    //TEST POINTS
    ofSetColor(0, 255, 0);
    ofCircle(500, 500, 5);
    
    ofCircle(1000, 500, 5);
    
    ofSetColor(255, 0, 0);
    ofCircle(400, 300, 5);


    //CENTER LINE
    ofSetLineWidth(2.0);
    ofSetColor(255);
    ofLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
   
    
    // Clear no Frags box
    if (boxes.size()){
        for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); ) {
            bool hasFrags = (*iter)->getFragsRemain();
            if (!hasFrags){
                boxes.erase(iter);
                cout << "size of boxes after erase: " << boxes.size() << endl;
            }else{
                iter++;
            }
        }
    }
    
    if (blackBoxes.size()){
        for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); ) {
            bool hasFrags = (*iter)->getFragsRemain();
            if (!hasFrags){
                blackBoxes.erase(iter);
                cout << "size of blackBoxes after erase: " << blackBoxes.size() << endl;
            }else{
                iter++;
            }
        }
    }
    
    if (darkBoxes.size()){
        for (vector<Box*>::iterator iter = darkBoxes.begin(); iter != darkBoxes.end(); ) {
            bool hasFrags = (*iter)->getFragsRemain();
            if (!hasFrags){
                darkBoxes.erase(iter);
                cout << "size of blackBoxes after erase: " << blackBoxes.size() << endl;
            }else{
                iter++;
            }
        }
    }

    //AUDIO - visualize
    ofSetColor(245, 58, 135);
    ofFill();       
    ofCircle(ofGetWidth() - 200, 200, fftSmoothed * 300.0f);
    
    
}


void testApp::drawPolygonBodies(){
    
//    cout << "num of pbodies: " << pBodies.size() << endl;
    
    
    for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end();) {
        
        bool pBodyIsAlive = (*iter)->getIsAlive();
        int pidx = (*iter)->getIndex();
        int dupIdx = (*iter)->getDupIndex();
        bool pBodyIsOriginal = (*iter)->getIsOriginal();
        
//        cout << "idx: " << pidx << " pBodyIsAlive: " << pBodyIsAlive << endl;
        
        if (!pBodyIsAlive) { // When dying

            if(pBodyIsOriginal){
                videoEnd();
                OriginDestroyed = true;
                
            }
            
            delete (*iter);
            oscSendII("/pbDest", pidx, dupIdx);
            
            iter = pBodies.erase(iter);
            float fragAreaPercent = (getFragsArea()/(ofGetWidth()*ofGetHeight()))*100.f;
            oscSendF("/fgArea", fragAreaPercent);

            if(inLastScene){
                if(fragAreaPercent < 1) kBLOBNUM = 0;
                else kBLOBNUM = 4;

                //cam ths adjust
                threshold[0] = 130*(fragAreaPercent/100.f);
                
                if (fragAreaPercent > 25) {
                    movShow = true;
                }else{
                    movShow = false;
                }
            }
            
            
            
//            cout << "pbodies size: " << pBodies.size() << endl;
//            cout << "pbodiesCopy size: " << pBodiesOriginalCopy.size() << endl;
            
        }else{ // When alive
            
            if(!pBodyIsOriginal){
                bool pbIsBorn = (*iter)->getIsNewBorn();
                if(pbIsBorn){
                    oscSendIIFF("/pbBorn", pidx, dupIdx, cvBlobPos.x, cvBlobPos.y);
                    (*iter)->setIsNewBorn(false);
                }
            }

            (*iter)->renderAtBodyPosition();
            (*iter)->update();
            
            if ( !(*iter)->getIsThereMBody() && !(*iter)->getIsBreaked()){
//                cout << "pidx: " << pidx << " dupIdx: "<< dupIdx << " is breaked and disappearing" << endl;
                oscSendII("/pbBrek", pidx, dupIdx);
                float fragAreaPercent = (getFragsArea()/(ofGetWidth()*ofGetHeight()))*100.f;
                oscSendF("/fgArea", fragAreaPercent);
                
                if(inLastScene){
                    
                    if(fragAreaPercent < 1) kBLOBNUM = 0;
                    else kBLOBNUM = 4;
                    
                    //cam ths adjust
                    threshold[0] = 130*(fragAreaPercent/100.f);

                    if (fragAreaPercent > 25) {
                        movShow = true;
                    }else{
                        movShow = false;
                    }
                    
                }

                (*iter)->setIsBreaked(true);
                
                vector<Frag*>frags = *(*iter)->getFrags();
//                cout << "frags size: " << frags.size() << endl;
                
                
                for (vector<Frag*>::iterator jter = frags.begin(); jter != frags.end(); jter++){
                    
                    bool fragIsBorn = (*jter)->getIsNewBorn();
                    
                    if(fragIsBorn){
                        int fragIdx = (*jter)->getIndex();
//                        cout << "fragidx: " << fragIdx << endl;
                        oscSendII("/fgBorn", pidx, fragIdx);
                        (*jter)->setIsNewBorn(false);
                    }
                }
            }
            
            
            iter++;
        }
    }
}


void testApp::makeFaceAt(float x, float y)
{
    for (int i = 0; i < cvBlobNum; i++){
        
        // Translate real boundingrect not just drawing.
        ofRectangle tBoundingRect;
        tBoundingRect.x = blobsVec[i].boundingRect.x + movDrawPosX;
        tBoundingRect.y = blobsVec[i].boundingRect.y + movDrawPosY;
        tBoundingRect.width = blobsVec[i].boundingRect.width;
        tBoundingRect.height = blobsVec[i].boundingRect.height;
        
        if (tBoundingRect.inside(x, y)){
            selBlobRect = i + 1;
//            printf("Selected blob rect number: %d\n", selBlobRect);
//            printf("centroid X: %f / Y: %f\n",
//                   blobsVec[i].centroid.x, blobsVec[i].centroid.y);
            makeFaces(selBlobRect);
            break;
        }
    }
}




void testApp::makeFaces(int blobNum){
    
    if (blobNum != 0){ // blobNum 0 means "Nothing selected".
        //clear containers
        blobsVec.clear();
        blobsPts.clear();
        blobsPtsDiv.clear();
        blobCenterPos.clear();
        
        // get vector<ofxCvBlob>
        blobsVec = contourFinder.blobs;
        
        cvBlobNum = blobsVec.size();
        //blobsVec[0].pts = all points of 1st blob.
        
        if(cvBlobNum != 0){
            blobsPts = blobsVec[blobNum - 1].pts;
            
            // Transform blobsPts
            for (vector<ofPoint>::iterator iter = blobsPts.begin(); iter != blobsPts.end(); iter++) {
                (*iter).x = (*iter).x + movDrawPosX;
                (*iter).y = (*iter).y + movDrawPosY;
            }
            
            cvBlobPos = blobsVec[blobNum - 1].centroid + ofPoint(movDrawPosX, movDrawPosY);
//            cvBlobPos = blobsVec[blobNum - 1].centroid;
        }
        
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
            
            makeBodyAtCvPosition();
            
        }
    }
}


void testApp::makeBodyAtCvPosition(){ //Make original

    if(getArea(&blobsPtsDiv[0], kMAX_VERTICES) > 0){ // If the area did not have minus value.
        Faces * aPbody = new Faces(iWorld, &blobsPtsDiv[0], kMAX_VERTICES, cvBlobPos.x, cvBlobPos.y, pBodyIdx, true, true, ORIGINAL_DUP_IDX);
        aPbody->setFragOutlineColor(pBodyOutlineColor[curStage]);
        
        
        Faces * aPbodyCopy = new Faces(iWorld, &blobsPtsDiv[0], kMAX_VERTICES, cvBlobPos.x, cvBlobPos.y, pBodyIdx, false, false, COPY_DUP_IDX);
        aPbodyCopy->setFragOutlineColor(pBodyOutlineColor[curStage]);
//        printf("cvBlobPos x: %f, y: %f\n", cvBlobPos.x, cvBlobPos.y);
        
//        oscSendIIFF("/pbBorn", pBodyIdx, ORIGINAL_DUP_IDX, cvBlobPos.x, cvBlobPos.y);

        
        pBodies.push_back(aPbody);
        
        
//        cout << aPbody << endl;
//        cout << "pBody push_back" << endl;
        
        pBodiesOriginalCopy[curStage] = *aPbodyCopy;
        
//        for (int i = 0; i < STAGE_NUM; i++) {
//         cout << pBodiesOriginalCopy[i].getX() << " / " << pBodiesOriginalCopy[i].getY() << endl;
//        }


        pBodyIdx++;
        
        movie[curMovie].setVolume(0.f);
        printf("curMovie: %d 's muted.\n", curMovie);
//        videoEnd();
    }
    
    // Reset blobs points vector
    blobsPts.clear();
    blobsPtsDiv.clear();
    
    
}

void testApp::makeBodyAtCvPosition(b2Vec2* vertices){
    
    if(getArea(&vertices[0], kMAX_VERTICES) > 0){ // If the area did not have minus value.
        Faces * aPbody = new Faces(iWorld, &vertices[0], kMAX_VERTICES, cvBlobPos.x, cvBlobPos.y, pBodyIdx, true, true, ORIGINAL_DUP_IDX);
        
        pBodies.push_back(aPbody);
        pBodyIdx++;
    }
}

void testApp::makeBodyAtCvPosition(vector<b2Vec2> vertices){
    
    if(getArea(&vertices[0], kMAX_VERTICES) > 0){ // If the area did not have minus value.
        Faces * aPbody = new Faces(iWorld, &vertices[0], kMAX_VERTICES, cvBlobPos.x, cvBlobPos.y, pBodyIdx, true, true, ORIGINAL_DUP_IDX);
        
        pBodies.push_back(aPbody);
        pBodyIdx++;
    }
}

void testApp::resetFaces(){

    for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end();) {
        
        (*iter)->clearFrags();
        delete (*iter);
        iter = pBodies.erase(iter);
    }
    
    cout << "pBodies reset." << endl;
    cout << "pBodies size: " << pBodies.size() << endl;
    
}

// UTIL
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

float testApp::getFragsArea(){
    
    sumOfArea = 0;
    
    for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++)
    {
        if (!(*iter)->getIsThereMBody()){
            sumOfArea+=getArea((*iter)->getVertices(), kMAX_VERTICES);
        }
    };
    
    return sumOfArea; //Sum of Area of Frags.
}



// OSC
void testApp::oscRecv()
{
    // check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		if(m.getAddress() == "/laserPos"){
			shot_X = m.getArgAsFloat(0);
			shot_Y = m.getArgAsFloat(1);
            
            // check first shot
            if(isFirstShot[curStage] == false) {
                isFirstShot[curStage] = true;
                firstShotCheck(curStage);
            }else{
                
//                cout << "laserPoint_X: " << shot_X << " / laserPoint_Y: " << shot_Y << endl;
                isShot = true;
                shotPointTest = true;
            }
            
        }else if(m.getAddress() == "/gunButPressed"){
			butMsg = m.getArgAsInt32(0);
            if (butMsg){
                if(!OriginDestroyed){
                    if(isFirstShot[curStage] == true) {
                        isFirstShot[curStage] = false;
                    }
                }else{
                    tmEnable(targetNum[curStage], fragLifeTime[curStage]); // int target num
                }
            }
		}
        
        else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}else{
					msg_string += "unknown";
				}
			}
		}
	}
}


void testApp::sendBlobsOSC()
{
    int i = 0;
    for(vector<ofxCvBlob>::iterator iter = blobsVec.begin(); iter != blobsVec.end(); iter++){
        
        ofxOscMessage m;
        m.setAddress("/blobs");
        m.addFloatArg(i);
        m.addFloatArg(iter->centroid.x);
        m.addFloatArg(iter->centroid.y);
        m.addFloatArg(iter->area);
        sender.sendMessage(m);
        i++;
    }
}


void
testApp::oscSendIIFF(string addr, int i, int j, float a, float b)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addIntArg(j);
    m.addFloatArg(a);
    m.addFloatArg(b);
    sender.sendMessage(m);
}


void
testApp::oscSendIFF(string addr, int i, float a, float b)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    m.addFloatArg(b);
    sender.sendMessage(m);
}


void
testApp::oscSendIF(string addr, int i, float a)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addFloatArg(a);
    sender.sendMessage(m);
}


void
testApp::oscSendII(string addr, int i, int j)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    m.addIntArg(j);
    sender.sendMessage(m);
}


void
testApp::oscSendF(string addr, float i)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addFloatArg(i);
    sender.sendMessage(m);
}


void
testApp::oscSendI(string addr, int i)
{
    ofxOscMessage m;
    m.setAddress(addr);
    m.addIntArg(i);
    sender.sendMessage(m);
}


// UI
void testApp::touchingCheck()
{
    // Touching check
    if (boxes.size() != 0){
        b2ContactEdge* contact = aBox->getBody()->GetContactList();
        
        if (contact) {
            b2Body* other = aBox->getBody()->GetContactList()->other;
            
            for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                if ((*iter)->getIsThereMBody()){
                    
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
}


// CONDITION
void testApp::firstShotCheck(int curStage)
{
    if (isFirstShot[curStage]) {
        cout << "first shot! of " << curStage << " !!" << endl;
        
        movie[curMovie].setVolume(0.0f); // Mute sound of movie.
        
        shot_X = bornPoint[curStage].x;
        shot_Y = bornPoint[curStage].y;
        
        makeFaceAt(shot_X, shot_Y);
    }
}

// STAGING
void testApp::nextStage()
{
    cout << "NEXT stage" << endl;
    tmOpen = false;
    //            resetFaces();
    
    if (curMovie < MOVNUM - 1){
        curMovie++;
        movie[curMovie].play();
        movie[curMovie].setVolume(movAmp[curMovie]);
        
    }

    if (curStage < STAGE_NUM){
        curStage++;
    }

    movShow = true;
    blobShow = false;
    
    resetFaces();
}


void testApp::nextStage(unsigned long long time, bool enable) //NOT USING
{
    if (enable){
        unsigned long long curTime = ofGetElapsedTimeMillis();
        //    cout << "curTime: " << curTime << endl;
        
        if ((curTime - stageStartTime) > time){
            
            stageStartTime = curTime;
            
            cout << "NEXT stage" << endl;
            tmOpen = false;
            //            resetFaces();
            curMovie++;
            movie[curMovie].play();
            curStage++;
            movShow = true;
            blobShow = false;
            
            enable = false;
        }
    }
}

void testApp::videoEnd()
{
    movShow = false;
    blobShow = true;
    
    if (!curMoviePlaying){
        curMoviePlaying = true;
    }
            
}

void testApp::tmEnable(int tNum, unsigned long long lifetime)
{
    cout << "target enabled, curStage: " << curStage << endl;
    tMan->setPbody(&pBodiesOriginalCopy[curStage]);
    tMan->setDupNum(tNum);
    tMan->setFragLifeTime(lifetime);
//    tMan->start();
    tmOpen = true;
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
        // Movie select - 0 means using vidGrabber
		case '0':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 0;
//            }
			break;

		case '2':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 2;
                movie[curMovie].play();
                
                cout << "press 2" << endl;
//            }
			break;
		case '3':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 3;
                movie[curMovie].play();
//            }
			break;

		case '4':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 4;
                movie[curMovie].play();
//            }
			break;

		case '5':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 5;
                movie[curMovie].play();
//            }
			break;

		case '6':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 6;
                movie[curMovie].play();
//            }
			break;

		case '7':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 7;
                movie[curMovie].play();
//            }
			break;
            
            
            
        // Toggle threshold inverting.
		case ' ':
            if(!REALTIME){
                inverting[curMovie] = !inverting[curMovie];
            }
			break;
            
		case '+':
			threshold[curMovie]++;
			if (threshold[curMovie] > 255) threshold[curMovie] = 255;
            cout << "THS: " << threshold[curMovie] << endl;

			break;
            
		case '=':
			threshold[curMovie] ++;
			if (threshold[curMovie] > 255) threshold[curMovie] = 255;
            cout << "THS: " << threshold[curMovie] << endl;

			break;
            
		case '-':
			threshold[curMovie]--;
			if (threshold[curMovie] < 0) threshold[curMovie] = 0;
            cout << "THS: " << threshold[curMovie] << endl;
            
			break;


        // Toggle gray image play
		case 'g':
//            if(!REALTIME){
                if (grayShow) grayShow = false;
                else grayShow = true;
//            }
			break;
            
            
        // Toggle original movie play full screen
		case 'm':
//            if(!REALTIME){
                if (movShow) movShow = false;
                else{
                    grayShow = false;
                    movShow = true;
                }
//            }
			break;
            
            
        // Toggle blobs drawing.
		case 'b':
//            if(!REALTIME){
                if (blobShow) blobShow = false;
                else blobShow = true;
//            }
			break;
            
        // Toggle text info report.
		case 'i':
//            if(!REALTIME){
                if (info) info = false;
                else info = true;
//            }
			break;
            
        // Delete selected pbodies.
		case 'x':
            for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end();) {
                bool isSelected = (*iter)->getSelectState();
                
                if (isSelected) {
                    (*iter)->clearFrags();
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
            for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end();) {
                
                    (*iter)->clearFrags();
                    delete (*iter);
                    iter = pBodies.erase(iter);

            }
//            printf("After delete size: %lu\n", pBodies.size());
			break;
            

        // Apply force to pBodies.
        case 't':

            if (darkBoxes.size()){
               // cout << "size of BLACK boxes: " << blackBoxes.size() << endl;
                // Draw black boxes
                for (vector<Box*>::iterator iter = darkBoxes.begin(); iter != darkBoxes.end(); iter++) {
                    // (*iter)->getBody()->ApplyForce(b2Vec2(40.f, 40.f), b2Vec2(ofGetWidth()/2, ofGetHeight()/2));
                    (*iter)->getBody()->ApplyLinearImpulse(b2Vec2(4.f, 4.f), b2Vec2(ofGetWidth()/2, ofGetHeight()/2));
                    // (*iter)->update();
                }
            }
           
            break;

            
            
        // Add ball
        case 'a':
//            if(!REALTIME){
                aBall = new Ball(iWorld, ofGetMouseX(), ofGetMouseY(), false);
                balls.push_back(aBall);
//            }
            
            break;
            
        case 'w': //Show right screen area
            ofSetWindowPosition(1440 - 1024, 0);
            break;
            
            
        case 'e': //Show left screen area
            ofSetWindowPosition(1440, 0);
            break;
            
            
        // Clear balls
        case 'c':
//            if(!REALTIME){
                // ball
                for (vector<Ball*>::iterator iter = balls.begin(); iter != balls.end(); iter++) {
                    iWorld->DestroyBody((*iter)->getBody());
                }
                balls.clear();
            
                // box
                for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
                    if ((*iter)->getIsThereMBody()) iWorld->DestroyBody((*iter)->getBody());
                }
                boxes.clear();
            
                // blackbox
                for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); iter++) {
                    if ((*iter)->getIsThereMBody()) iWorld->DestroyBody((*iter)->getBody());
                }
                blackBoxes.clear();
            
//            }
			break;
            
        case OF_KEY_UP:
            if(!REALTIME) aforce = aforce + 0.05f;
            break;

        case OF_KEY_DOWN:
            if(!REALTIME) aforce = aforce - 0.05f;
            break;
            
        case 'q':
            isShot = true;
            break;
            
        case 'h': //simulaton 'h'itting body
            
//             for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
//                 bool isSelected = (*iter)->getSelectState();
                
//                 if (isSelected && (*iter)->getIsThereMBody()) {
//                     (*iter)->breakBody();
//                     break;
             
// //                    cout << "world center: " << (*iter)->getBody()->GetWorldCenter().x << " / " << (*iter)->getBody()->GetWorldCenter().y << endl;
//                 }
//             }
            
 
            if (boxes.size() > 0){
                cout << "Hit a box!" << endl;
                for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
                    if ((*iter)->getIsThereMBody()) {
                        (*iter)->breakBody();
                        break;
                    }
                }
                
            }else{
                cout << "Hit a blackBox!" << endl;
                for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); iter++) {
                    if ((*iter)->getIsThereMBody()) {
                        (*iter)->breakBody();
                        break;
                    }
                }
            }
            
            
            break;
            
//        case OF_KEY_LEFT:
//            cout << "count: " << iWorld->GetBodyCount() << endl;
//            cout << "list: " << iWorld->GetBodyList() << endl;
//            break;
            

        case 's': //video stop
//            if (curMoviePlaying){
//                movie[curMovie].stop();
//                curMoviePlaying = false;
//            }else{
//                movie[curMovie].play();
//                curMoviePlaying = true;
//            }
            
            
            inTitle = false;
            bBox = new Box(iWorld, ofGetWidth()*1/4, ofGetHeight()/2.f);
            boxes.push_back(bBox);
            // cout << "size of boxes after construct" << boxes.size() << endl;
            break;
            
        case 'z':
//            for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
//                iWorld->DestroyBody((*iter)->getBody());
//            }
//
            if (boxes.size()){
                iWorld->DestroyBody((boxes.back())->getBody());
                boxes.pop_back();
            }
            
            break;
            
        case 'd':
            if (boxes.size() > 0){
                boxes.back()->setToBlack(true);
                blackBoxes.push_back(boxes.back());
                cout << "size after pop back: " << boxes.size() << endl;
                boxes.pop_back();
            }


            if (darkBoxes.size() > 0){
                darkBoxes.back()->setToBlack(true);
                blackBoxes.push_back(darkBoxes.back());
                cout << "size after pop back: " << darkBoxes.size() << endl;
                darkBoxes.pop_back();
            }
            
            break;
            
        case 'j':
            
            blackout = true;
            
            break;
            
		case '1':
            inTitle = false;
            grayShow = false;
            movShow = true;
            
            if (curMovie != 0) movie[curMovie].stop();
			curMovie = 1;
            movie[curMovie].play();
            movie[curMovie].setVolume(movAmp[curMovie]);
			break;
            
            
        case 'o': //Make targets
            //            if(OriginDestroyed){
            tmEnable(targetNum[curStage], fragLifeTime[curStage]); // int target num
            //            }
            
            break;
            
            
        case 'n': //next step
            
            // Free previous blobs synths.
            oscSendI("/creatBlobSyn", 0);
            
            OriginDestroyed = false;
            blobsSynMade = false;
            blobShow = false;
            
            
            
            
            if (!inTitle){
                
                if(nextStageReady && curStage < STAGE_NUM - 2){
                    nextStage();
                    nextStageReady = false;
                    
                }else if(nextStageReady && curStage == STAGE_NUM - 2){
                    
                    if (curMovie != 0) movie[curMovie].stop();
                    curStage++;
                    curMovie = 0;
                    movShow = true;
                    blobShow = true;
                }
            }
            
            
            break;
            
            
        case 'r':
            randFace = !randFace;
            
            // Free previous blobs synths.
            oscSendI("/creatBlobSyn", 0);
            
            if (!inLastScene) inLastScene = true;
            
            if (curMovie != 0) movie[curMovie].stop();
            curMovie = 0;
            movShow = false;
            
            OriginDestroyed = true;
            blobsSynMade = false;
            
            
            
            if (randFace) printf("RandFace - ON!\n");
            else printf("RandFace - OFF!\n");

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
//    Ball * aBall = new Ball(iWorld, x, y, false);
//    balls.push_back(aBall);


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    
    if (button == 0){
        
        cout << "MousePos x: " << x << " / y: " << y << endl;
        
        
//        makeFaceAt(x, y);
//        cout << "makefaceat x: " << x << " / y: " << y << endl;
//        selBlobRect = 0;
    }
        
    
    //    printf("Num of pbodies: %lu\n", pBodies.size());
        
        
        // For polygon body selection
//        if (boxes.size() == 0){
//            aBox = new Box(iWorld, ofGetMouseX(), ofGetMouseY());
//            boxes.push_back(aBox);
//        }
    

    // Right click
    if (button == 2){
        
//        aBall = new Ball(iWorld, x, y, true);
//        balls.push_back(aBall);
//        cout << pBodies[0]->IsInside( b2Vec2(x, y) ) << endl;
    }
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    if (button == 0){
        // Reset booleans.
        selBlobRect = 0;
        touched = false;
        
        // Delete touch box(sensor object).
//        delete boxes[0];
//        boxes.clear();
        
    }else if(button == 2){
//        delete balls[0];
//        balls.clear();
    }
    

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
