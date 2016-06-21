#include "testApp.h"

void testApp::exit()
{
    cout << "exiting.." << endl;
    
}

//--------------------------------------------------------------
void testApp::setup(){


    //File
    // fileToRead = ofToDataPath("file.txt");
    fileToRead = ofToDataPath("verticeData.txt");

    //Set projection width / height
    int pjW = 1920;
    int pjH = 1080;
    
    float winPosX = (pjW - ofGetWidth())/2.f;
    float winPosY = (pjH - ofGetHeight())/2.f;
    
    if (REALTIME){
//        ofSetWindowPosition(winPosX + 1440, winPosY - 90); //Extended desktop
        
        windowPosition = ofPoint(1440, 0);
        ofSetWindowPosition(windowPosition.x, windowPosition.y);
        

        info = false;
        inTitle = false;
        inLastScene = false;
        blackout = false;
    }else{
        
        windowPosition = ofPoint(0, 0);
        ofSetWindowPosition(windowPosition.x, windowPosition.y);

        // cout << ofGetWidth() << endl;
        // ofSetWindowPosition(ofGetWidth(), windowPosition.y);

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


    for (int i = 1; i < MOVNUM; i++){
        if (i == 7 || i == 9){
            movie[i].loadMovie("movies/" + ofToString(i) + ".mov");
        }else{
            movie[i].loadMovie("movies/" + ofToString(i) + ".mp4");
        }
    };

   // movie[11].loadMovie("movies/01.mp4");

  
    movAmp[1] = 1.0f;
    movAmp[2] = 0.1f;
    movAmp[3] = 0.1f;
    movAmp[4] = 0.2f;
    movAmp[5] = 0.5f;

    movDrawPosX = (ofGetWidth()/4.f) - (movie[curMovie].width/2.f);
    movDrawPosY = (ofGetHeight()/2.f) - (movie[curMovie].height/2.f);
    
    curMovie = 1; // 0 means using vidGrabber.
    movie[curMovie].stop();
    
    
    // Get movie Width / Height
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
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
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
    threshold[7] = TH_7;
    threshold[8] = TH_8;
    threshold[9] = TH_9;
    threshold[10] = TH_10;
    threshold[11] = TH_11;

    
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

    //speziell
    center = new Wall(iWorld, ofGetWidth()/2, ofGetHeight()/2, thickness/2, ofGetHeight(),
        CENTERWALL_CATE_BIT, CENTERWALL_MASK_BIT);
    
    // vector init
    blobsPts.clear();
    blobsPtsDiv.clear();
    
  
    //PREPARE DARK BOXES!
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 6; j++){
            //00000001(1)
            Box* darkBox = new Box(iWorld, 64*(i+1), 64*(j+1),
                DARKBOX_CATE_BIT, DARKBOX_MASK_BIT);
            darkBox->setToBlack(true);
            darkBoxes.push_back(darkBox);
        }
    }


    //FRAG
    fragColor = ofColor(70, 140, 60);

    
    // OSC
    blobsSynMade = false;
    
    //to play title music
    oscSendI("/title", 1);
    

    //AUDIO - Active mic input interation
    // 0 output channels, 
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    soundStream.listDevices();
    
    //if you want to set a different device id 
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    leftSig.assign(bufferSize, 0.0);
    rightSig.assign(bufferSize, 0.0);
    
    bufferCounter   = 0;
    smoothedVol     = 0.0;
    scaledVol       = 0.0;

    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);


    //AUDIO - for get 'amp' from file
    // soundPlayer.loadSound(ofToDataPath("pp.wav"));
    // soundPlayer.setLoop(true);
    // // soundPlayer.play();
    // nBandsToGet = 1;
  

}

//--------------------------------------------------------------
void testApp::update(){

    //AUDIO
    //lets scale the vol up to a 0-1 range 
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);

    //GET FFT just 1 band = approximately getting 'amp'.
    // ofSoundUpdate();
    // float * val = ofSoundGetSpectrum(nBandsToGet);
    // fftSmoothed *= 0.96f;
    // if (fftSmoothed < val[0]) fftSmoothed = val[0];
    
    
    if(!inTitle){
        // Box2D world update
        float32 timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        iWorld->Step(timeStep, velocityIterations, positionIterations);
        
        
        // opencv update
        bool bNewFrame = false;

        //cam
        movie[curMovie].update();
        bNewFrame = movie[curMovie].isFrameNew();

        if (bNewFrame){

            // Resize allocated images.
            colorImg.resize(movRes[curMovie].x, movRes[curMovie].y);
            grayImage.resize(movRes[curMovie].x, movRes[curMovie].y);
        
            movie[curMovie].update();
            colorImg.setFromPixels(movie[curMovie].getPixels(), movRes[curMovie].x, movRes[curMovie].y);
            grayImage.setFromColorImage(colorImg);
            grayImage.threshold(threshold[curMovie], inverting[curMovie]);
            
            contourFinder.findContours(grayImage, kMIN_BLOBAREA, (movRes[curMovie].x * movRes[curMovie].y)/3, kBLOBNUM, true);

            blobsVec.clear(); //Init containers with clearing.
            blobsVec = contourFinder.blobs; //Get vector<ofxCvBlob>
            cvBlobNum = blobsVec.size();
        }


        //OSC - num of boxes
        int curNumBoxes = boxes.size();
        if (numBoxes != curNumBoxes){
            oscSendI("/boxNum", curNumBoxes);

            numBoxes = curNumBoxes;
        }

        //OSC - canBeHit status
        for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
            if ( (*iter)->getIsThereMBody() ){
                bool hittable = (*iter)->getCanBeHitUpdated();
                if (hittable){
                    oscSendII("/canHit", (*iter)->getIndex(), (int)(*iter)->getCanBeHit());
                }
            }
        }
            

        // Previous superball shooting check
        if(shotBallMade){
            delete aBall;
            balls.clear();
            shotBallMade = false;
        }
        
        
        //SHOOTING CHECK
        if (isShot){
            
            for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                if ( (*iter)->getIsThereMBody() ){
                    // if ( (*iter)->IsInside(b2Vec2(shot_X, shot_Y)) ){
                    if ( (*iter)->IsInside(b2Vec2(ofGetMouseX(), ofGetMouseY())) ){

                        cout << "hit Face!" << endl;

                        // Consider 'life' of face object
                        int faceLife = (*iter)->getLife();
                        if (faceLife > 0) (*iter)->shot();
                        else (*iter)->breakBody(ofGetMouseX(), ofGetMouseY());
                        // else (*iter)->breakBody(shot_X, shot_Y);
                        
                        //Make forec at shot position
                        bodyHit = true;
                    }
                }
            }

            
            for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
                if ( (*iter)->getIsThereMBody() ){
                    // if ( (*iter)->IsInside(b2Vec2(shot_X, shot_Y)) ){
                    if ( (*iter)->IsInside(b2Vec2(ofGetMouseX(), ofGetMouseY())) ){

                        cout << "hit Box!" << endl;

                        (*iter)->breakBody(ofGetMouseX(), ofGetMouseY());
                        // (*iter)->breakBody(shot_X, shot_Y);
                        bodyHit = true;
                    }
                }
            }
            
            
            for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); iter++) {
                if ( (*iter)->getIsThereMBody() ){
                    // if ( (*iter)->IsInside(b2Vec2(shot_X, shot_Y)) ){
                    if ( (*iter)->IsInside(b2Vec2(ofGetMouseX(), ofGetMouseY())) ){

                        cout << "hit Black Box!" << endl;

                        (*iter)->breakBody(ofGetMouseX(), ofGetMouseY());
                        bodyHit = true;
                    }
                }
            }
            
            
            if(!bodyHit){
            
                cout << "No hit!" << endl;
                aBall = new Ball(iWorld, shot_X, shot_Y, true);
                // aBall = new Ball(iWorld, ofGetMouseX(), ofGetMouseY(), true);

                // b2Body* ballBody = aBall->getBody();
                // ballBody->ApplyLinearImpulse(b2Vec2(-200.f, -200.f),
                //     b2Vec2(ofGetMouseX(), ofGetMouseY()));

                // balls.push_back(aBall);

                shotBallMade = true;
            }
            
            // Reset booleans
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

    // Draw Box2D walls
    left->renderAtBodyPosition();
    right->renderAtBodyPosition();
    floor->renderAtBodyPosition();
    ceil->renderAtBodyPosition();
    center->renderAtBodyPosition();


    ofBackground(0, 0, 0);
    
    //RIGHT SIDE OF SCREEN
    ofSetColor(148, 211, 230);
    // ofSetColor(10, 10, 10);
    ofRect(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    
   
    // Draw movie.
    if (movShow){
      movie[curMovie].draw(movDrawPosX, movDrawPosY);
        // movie[curMovie].draw(0, 0);

    }
    

    
    
    // Draw ball
    for (vector<Ball*>::iterator iter = balls.begin(); iter != balls.end(); iter++) {
        (*iter)->renderAtBodyPosition();
    }

    // Like a darkmaterial in the outer space
    if (darkBoxes.size()){
       // cout << "size of BLACK boxes: " << blackBoxes.size() << endl;
        // Draw black boxes
        float mulForce = 40.f;
        for (vector<Box*>::iterator iter = darkBoxes.begin(); iter != darkBoxes.end(); iter++) {
            (*iter)->renderAtBodyPosition();
            // (*iter)->update();
            // (*iter)->getBody()->ApplyForce(b2Vec2(fftSmoothed * mulForce, fftSmoothed * mulForce), b2Vec2(ofGetWidth()/2, ofGetHeight()/2));
            // (*iter)->getBody()->ApplyLinearImpulse(b2Vec2(fftSmoothed, fftSmoothed), b2Vec2(ofGetWidth()/2, ofGetHeight()/2));

            // (*iter)->getBody()->ApplyForce(b2Vec2(scaledVol * mulForce, scaledVol * mulForce),
                // b2Vec2(ofGetWidth()/4, ofGetHeight()/2));
            (*iter)->getBody()->ApplyLinearImpulse(b2Vec2(scaledVol, scaledVol),
                b2Vec2(ofGetWidth()/2, ofGetHeight()/2));
  
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
   

    // DRAW BODIES 
    if(pBodies.size()) drawPolygonBodies();
    

    
    // SHOW LaserPointTracking point
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
   
    
    // Clear no Frags bodies
    if (pBodies.size()){
        for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); ) {
            bool hasFrags = (*iter)->getFragsRemain();
            if (!hasFrags){
                delete (*iter);
                pBodies.erase(iter);
                cout << "size of pBodies after erase: " << pBodies.size() << endl;
            }else{
                iter++;
            }
        }
    }

    if (boxes.size()){
        for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); ) {
            bool hasFrags = (*iter)->getFragsRemain();
            if (!hasFrags){
                delete (*iter);
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
                delete (*iter);
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
                delete (*iter);
                darkBoxes.erase(iter);
                cout << "size of blackBoxes after erase: " << blackBoxes.size() << endl;
            }else{
                iter++;
            }
        }
    }

    // Draw grayimage.
    if (grayShow){
        grayImage.draw(movDrawPosX, movDrawPosY);
    }
 
    // Draw contourFinder
    if (blobShow){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(movDrawPosX, movDrawPosY);
        ofSetLineWidth(2.0);
        
        for (int i = 0; i < contourFinder.nBlobs; i++){
            // contourFinder.blobs[i].draw(movDrawPosX, movDrawPosY);
            contourFinder.blobs[i].draw(0, 0);
            
        }
        ofPopMatrix();
        ofPopStyle();
    }
 
    //AUDIO - visualize
    ofSetColor(245, 58, 135);
    ofFill();       
    // ofCircle(ofGetWidth() - 200, 200, fftSmoothed * 300.0f);
    ofCircle(ofGetWidth() - 200, 200, scaledVol * 300.0f);


    //WINDOW SLIDE
    if (slideRight){
        if (slideWindowInTime(ofPoint(ofGetWidth()/2, 0), ofPoint(ofGetWidth(), 0), 20)){
            cout << windowPosition.x << endl;
            slideRight = false;
        }
    }else if (slideLeft){
        if (slideWindowInTime(ofPoint(ofGetWidth(), 0), ofPoint(ofGetWidth()/2, 0), 20)){
            cout << windowPosition.x << endl;
            slideLeft = false;
        }
    }
    
     
    if (inTitle){
//        printf("inTitle true.\n");
        movShow = false;
        blobShow = false;
        grayShow = false;
        
        title.draw(
        ofGetWidth()/4.f - title.width/2.f,
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
    
}

/* ================ FUNCTIONS ================ */
void testApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0; 

    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume   
    for (int i = 0; i < bufferSize; i++){
        leftSig[i]     = input[i*2]*0.5;
        rightSig[i]    = input[i*2+1]*0.5;

        curVol += leftSig[i] * leftSig[i];
        curVol += rightSig[i] * rightSig[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :) 
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :) 
    curVol = sqrt( curVol );

    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
}


bool testApp::slideWindowInTime(ofPoint bPos, ofPoint ePos, float nframe)
{

    bool isDone = false;

    cout << "slide? " << endl;
    // dir 0 : large -> small
    // dir 1 : small -> large
    int dir = 1;
    if (bPos.x > ePos.x) dir = 0;

    cout << "dir?" << dir << endl;

    float d = bPos.distance(ePos);
    int dist = (int)d;
    
    cout << "dist" << dist << endl;

    int distToMove = 1 + (int)d/nframe;

    cout << "distToMove" << distToMove << endl;

    if (dir){
        if (windowPosition.x >= bPos.x && windowPosition.x <= ePos.x){
            int nextPos = windowPosition.x + distToMove;
            if (nextPos > ePos.x) nextPos = ePos.x;
            windowPosition.x = nextPos;
        }else{
            return false;
        }
    }else{
        if (windowPosition.x <= bPos.x && windowPosition.x > ePos.x){
            int nextPos = windowPosition.x - distToMove;
            windowPosition.x = MAX(nextPos, ePos.x);
        }else{
            return false;
        }
    }

    ofSetWindowPosition(windowPosition.x, 0);

    if (windowPosition.x == ePos.x){
        isDone = true;
    }

    return isDone;

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


            saveFaceVerticeToFile(selBlobRect, ofToString(curMovie) + ".txt");

            
            //FIXME: Fix body created position
//            makeBodyAt(x, y);

            break;
        }
    }
}


//Update 'blobsPtsDiv', 'cvBlobNum'
void testApp::makeFaceVertice(int blobNum){
    
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
            cvBlobPos = blobsVec[blobNum - 1].centroid;
        }
        
        if(blobsPts.size() > 0){
            
            // divNum = sampling interval
            if (blobsPts.size() > kMAX_VERTICES){
                divNum = blobsPts.size()/kMAX_VERTICES;
            }else{
                divNum = 1;
            }
            
            // Make blobsPtsDiv
            b2Vec2 temp = b2Vec2(blobsPts[0].x, blobsPts[0].y);
            
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

        }
    }
}


void testApp::saveFaceVerticeToFile(int blobNum, string fileName){

    //update blobPtsDiv, cvBlobPos
    makeFaceVertice(blobNum);
    string dataStr = "";
    
    // Add vertices - Assume that it's center point is (0, 0)
    for (vector<b2Vec2>::iterator iter = blobsPtsDiv.begin(); iter != blobsPtsDiv.end(); iter++) {

        string data_X = ofToString((*iter).x - cvBlobPos.x);
        string data_Y = ofToString((*iter).y - cvBlobPos.y);

        dataStr.append(data_X);
        dataStr.append("/");
        dataStr.append(data_Y);
        dataStr.append(",");

    }

    // cout << "content of blobsPtsDiv" << dataStr << endl; 

    ofFile newfile(ofToDataPath(fileName), ofFile::WriteOnly);
    newfile.create();
    
    newfile << dataStr;
    newfile.close();
}




void testApp::makeBodyAt(float posX, float posY){

    int maxVCount = blobsPtsDiv.size();

    vector<b2Vec2> vertices;
    for (vector<b2Vec2>::iterator iter = blobsPtsDiv.begin(); iter != blobsPtsDiv.end(); iter++) {
        b2Vec2 tPt = b2Vec2((*iter).x + posX, (*iter).y + posY);
        vertices.push_back(tPt);
    }

    if(getArea(&vertices[0], maxVCount) > 0){ // If the area did not have minus value.
        // Faces * aPbody = new Faces(iWorld, &vertices[0], maxVCount, posX, posY);
        Faces * aPbody = new Faces(iWorld, &vertices[0], maxVCount, posX, posY,
            fragLife, fragColor);
        pBodies.push_back(aPbody);
    } 
}


void testApp::makeBodyFromFile(ofFile argFile, float posX, float posY){

    string strFromFile = "";
    argFile >> strFromFile;

    //Split str to each pairs
    vector<string> pairStr = ofSplitString(strFromFile, ",");

    //Vertices translate
    vector<b2Vec2> vertices;
    for (vector<string>::iterator iter = pairStr.begin(); iter != pairStr.end() - 1; iter++) {
        vector<string> pt = ofSplitString((*iter), "/"); //split X and Y
        b2Vec2 tPt = b2Vec2(ofToFloat(pt[0]) + posX, ofToFloat(pt[1]) + posY);
        vertices.push_back(tPt);
    }

   if(getArea(&vertices[0], vertices.size()) > 0){ // If the area did not have minus value.
       // Faces * aPbody = new Faces(iWorld, &vertices[0], kMAX_VERTICES, posX, posY);
       Faces * aPbody = new Faces(iWorld, &vertices[0], kMAX_VERTICES, posX, posY,
       fragLife, fragColor);
       pBodies.push_back(aPbody);
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


/* ================================ UTIL ================================ */

float testApp::ofToFloat(string str) {
    istringstream stream(str);
    float result;
    stream >> result;
    return result;
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
        case '1':
            inTitle = false;
            grayShow = false;
            movShow = false;
            blobShow = true;

            if (curMovie != 0) movie[curMovie].stop();
            curMovie = 1;

            movDrawPosX = (ofGetWidth()/4.f) - (movie[curMovie].width/2.f);
            movDrawPosY = (ofGetHeight()/2.f) - (movie[curMovie].height/2.f);
  
            movie[curMovie].play();
            movie[curMovie].setVolume(movAmp[curMovie]);
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

        case '8':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 8;
                movie[curMovie].play();
//            }
            break;

        case '9':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 9;
                movie[curMovie].play();
//            }
            break;

         case '0':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 10;
                movie[curMovie].play();
//            }
            break;

         case '`':
//            if(!REALTIME){
                if (curMovie != 0) movie[curMovie].stop();
                curMovie = 11;
                movie[curMovie].play();
//            }
            break;
                      
            
            
        // Toggle threshold inverting.
		case ' ':
            if (curMoviePlaying){
                movie[curMovie].stop();
                curMoviePlaying = false;
            }else{
                movie[curMovie].play();
                curMoviePlaying = true;
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
            


        case 'v':
            // makeBodyFromFile(fileToRead, ofGetMouseX(), ofGetMouseY());
            makeBodyFromFile(ofToString(curMovie)+".txt", ofGetMouseX(), ofGetMouseY());
            break;

        case 'y':
            slideLeft = true;
            slideRight = false;
            break;

        case 'u':
            slideRight = true;
            slideLeft = false;
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
                    if ((*iter)->getIsThereMBody()){
                        iWorld->DestroyBody((*iter)->getBody());
                        delete (*iter);
                    }
                }
                boxes.clear();
            
                // blackbox
                for (vector<Box*>::iterator iter = blackBoxes.begin(); iter != blackBoxes.end(); iter++) {
                    if ((*iter)->getIsThereMBody()){
                        iWorld->DestroyBody((*iter)->getBody());
                        delete (*iter);
                    } 
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

         case OF_KEY_LEFT:
            //Delete previous Faces
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

            if (curFace >= 1){
                cout << "FACE NUM: " << "vertices/"+ofToString(curFace)+".txt" << endl;
                makeBodyFromFile("vertices/"+ofToString(curFace)+".txt", ofGetMouseX(), ofGetMouseY());
                curFace--;
            }
 
            break;

         case OF_KEY_RIGHT:
            //Delete previous Faces
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

            if (curFace <= FACENUM){
                cout << "FACE NUM: " << "vertices/"+ofToString(curFace)+".txt" << endl;
                makeBodyFromFile("vertices/"+ofToString(curFace)+".txt", ofGetMouseX(), ofGetMouseY());
                curFace++;
            }

            break;
             
        case 'q':
            isShot = true;
            break;
            
        case 'h': //simulaton 'h'itting body
            
            if (pBodies.size()){
                cout << "Hit a pBody!" << endl;
                for (vector<Faces*>::iterator iter = pBodies.begin(); iter != pBodies.end(); iter++) {
                    if ((*iter)->getIsThereMBody()) {
                        (*iter)->breakBody();
                        break;
                    }
                }
            }
            
 
            if (boxes.size() > 0){
                cout << "Hit a box!" << endl;
                for (vector<Box*>::iterator iter = boxes.begin(); iter != boxes.end(); iter++) {
                    if ((*iter)->getIsThereMBody()) {
                        (*iter)->breakBody();
                        break;
                    }
                }
                
            }else if (blackBoxes.size() > 0) {
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
          
            inTitle = false;
            //0x00000111(7)
            bBox = new Box(iWorld, ofGetWidth()*1/4, ofGetHeight()/2.f,
                BOX_CATE_BIT, BOX_MASK_BIT, boxIdx);
            boxes.push_back(bBox);
            boxIdx++;
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
        
       makeFaceAt(x, y);
       cout << "makefaceat x: " << x << " / y: " << y << endl;
       selBlobRect = 0;
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
