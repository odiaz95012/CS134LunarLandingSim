#include "ofApp.h"
#include "Util.h"



void ofApp::setup(){
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;
    loadData();
    ofEnableSmoothing();
    ofEnableDepthTest();
    
    mainCamera = &cam; // cam is initially the moving cam

    setUpGui();


	octree.create(moon.getMesh(0), 20);
    
    setUpLighting();

	testBox = Box(Vector3(3, 3, 0), Vector3(5, 5, 2));

    
    setUpPlayerData();
    
    ofVec3f landingMin(-25,0,-25);
    ofVec3f landingMax(25, 0, 25);
    landingArea = Box(Vector3(landingMin.x, landingMin.y, landingMin.z), Vector3(landingMax.x, landingMax.y, landingMax.z));
}
void ofApp::setUpGui(){
    gui.setup();
    gui.add(thrustForce.setup("Thrust Force", 500, 0, 5000));
    gui.add(restitution.setup("Restitution", 0.5, 0, 100));
    bHide = false;
}
void ofApp::setUpPlayerData(){
    turbForce = new TurbulenceForce(ofVec3f(-5, -5, -5), ofVec3f(5, 5, 5));
    gravityForce = new GravityForce(ofVec3f(0, -1.84, 0)); // gravity for moon


    
    plyr = new Particle();
    playrSys = new ParticleSystem();
    
    plyr->position = glm::vec3(0, 50 , 0);
    plyr->radius = 0.1;
    plyr->damping = 0.1;
    plyr->lifespan = INFINITY;
    playrSys->add(*plyr);
    
    landerTurbulance = new TurbulenceForce(ofVec3f(-3,-3,-3), ofVec3f(3,3,3));
    playrSys->addForce(gravityForce);
    playrSys->addForce(landerTurbulance);
    

    setUpEmitters();
    
    setUpCameras();


    ofVec3f min = lander.getSceneMin() + lander.getPosition();
    ofVec3f max = lander.getSceneMax() + lander.getPosition();

    landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}
void ofApp::setUpEmitters(){
    landerExhaust.setLanderPointer(&lander);
    landerExhaust.setVelocity(glm::vec3(0,0,0));
    landerExhaust.setPosition(lander.getPosition());
    landerExhaust.setOneShot(true);
    landerExhaust.setLifespan(1);
    landerExhaust.setEmitterType(RingEmitter);
    landerExhaust.setGroupSize(100);
    landerExhaust.setParticleColor(ofColor::orangeRed);
    landerExhaust.sys->addForce(gravityForce);
    
    radialForce = new ImpulseRadialForce(100.0);
    radialForce->setHeight(4.0);
    explosion.setVelocity(glm::vec3(0,0,0));
    explosion.setPosition(glm::vec3(0,5,0));
    explosion.setOneShot(true);
    explosion.setRandomLife(true);
    explosion.setEmitterType(RadialEmitter);
    explosion.setGroupSize(1000);
    explosion.setParticleColor(ofColor::orangeRed);
    explosion.sys->addForce(gravityForce);
    explosion.sys->addForce(radialForce);
    explosion.sys->addForce(turbForce);
    
}
void ofApp::setUpLighting(){
    
    landerLight.setup();
    landerLight.enable();
    landerLight.setOrientation(glm::vec3(0,-1,0)); // aim down onto terrain
    landerLight.setAttenuation(1.0, 0.005, 0.001);
    landerLight.setPointLight();
    glm::vec3 landerPos = lander.getPosition();
    landerLight.setPosition(landerPos.x, landerPos.y + 1, landerPos.z);
    

    
    keyLight.setup();
    keyLight.enable();
    keyLight.setAreaLight(1, 1);
    keyLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
    keyLight.setDiffuseColor(ofFloatColor(1, 1, 1));
    keyLight.setSpecularColor(ofFloatColor(1, 1, 1));
    keyLight.setOrientation(ofQuaternion(-35, ofVec3f(1, 0, 0)));
    keyLight.rotate(-45, ofVec3f(0, 1, 0));
    keyLight.setPosition(-28, 10, 30);


    fillLight.setup();
    fillLight.enable();
    fillLight.setSpotlight();
    fillLight.setScale(.05);
    fillLight.setSpotlightCutOff(18);
    fillLight.setAttenuation(2, .001, .001);
    fillLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
    fillLight.setDiffuseColor(ofFloatColor(1, 1, 1));
    fillLight.setSpecularColor(ofFloatColor(1, 1, 1));
    fillLight.setOrientation(ofQuaternion(-15, ofVec3f(1, 0, 0)));
    fillLight.rotate(30, ofVec3f(0, 1, 0));
    fillLight.setPosition(28, 10, 50);

    
}
void ofApp::loadData(){
    if(!moon.loadModel("geo/moon-houdini.obj")){
        cout << "Failed to load the moon terrain model." << endl;
        ofExit();
    }
    moon.setScaleNormalization(false);
    if(background.load("images/starsBackground.jpeg")) backgroundLoaded = true;
    else{
        cout << "Failed to load the background image" << endl;
        ofExit();
    }
    if(lander.loadModel("geo/lander.obj")){
        lander.setScaleNormalization(false);
        lander.setPosition(0, 50 , 0);
        lander.setRotation(0, 180, 0, 1, 0); // make the ladder of the spacecraft the front of the lander
        bLanderLoaded = true;
    } else{
        cout << "Failed to load the lander model." << endl;
        ofExit();
    }
    if(!sansFont.load("OpenSans-Regular.ttf", 12)){
        cout << "Failed to load font." << endl;
        ofExit();
    }
    if(!sansFontLarge.load("OpenSans-Regular.ttf", 24)){
        cout << "Failed to load font." << endl;
        ofExit();
    }
    if(!exhaustSound.load("sounds/rocket_thrust_sound.mp3")){
        cout << "Failed to load the exhaust sound." << endl;
        ofExit();
    }
    exhaustSound.setLoop(true);
    if(!gameMusic.load("sounds/game-music.mp3")){
        cout << "Failed to load the game music." << endl;
        ofExit();
    }
    gameMusic.setLoop(true);
    if(!explosionSound.load("sounds/explosion.mp3")){
        cout << "Failed to load the explosion sound effect." << endl;
        ofExit();
    }
    if(!successfulLanding.load("sounds/eagleLanded.mp3")){
        cout << "Failed to load the eagle landed sound." << endl;
        ofExit();
    }
    ofDisableArbTex();
    
    //load textures
    if(!ofLoadImage(exhaustParticleTex, "images/fire.png")){
        cout << "Failed to load exhaust particles texture." << endl;
        ofExit();
    }
    if(!ofLoadImage(explosionParticleTex, "images/star.png")){
        cout << "Failed to load explosion particle texture." << endl;
        ofExit();
    }
    //load the exhaust shader
#ifdef TARGET_OPENGLES
    exhaustParticleShader.load("shaders_gles/shader");
#else
    exhaustParticleShader.load("shaders/shader");
#endif

}
void ofApp::playExhaustSound(){
    if(playExhaust && !exhaustSound.isPlaying())
        exhaustSound.play();
    else if (!playExhaust && exhaustSound.isPlaying())
        exhaustSound.stop();
}
void ofApp::playGameMusic(){
    if(!gameOver && !gameMusic.isPlaying())
        gameMusic.play();
    else if (gameOver && gameMusic.isPlaying())
        gameMusic.stop();
}
void ofApp::playSuccessfulLandingSound(){
    if(gameWon && !successfulLanding.isPlaying() && successfulLandingPlayCount < 1){
        gameMusic.stop();
        successfulLanding.play();
        successfulLandingPlayCount++;
    }
    else if (!gameWon && successfulLanding.isPlaying())
        successfulLanding.stop();
}
void ofApp::setUpCameras(){
    cam.setDistance(30);
    cam.setNearClip(.1);
    cam.setFov(65.5);
    cam.setPosition(0, 20, 30);
    ofSetVerticalSync(true);
    cam.disableMouseInput();
    
    
    onBoardCam.setNearClip(.1);
    onBoardCam.setFov(65.5);
    onBoardCam.disableMouseInput();
    onBoardCam.lookAt(glm::vec3(0,-1,0));
    
    

    glm::vec3 landerPos = playrSys->particles[0].position;
    frontCam.setNearClip(.1);
    frontCam.setPosition(landerPos.x, landerPos.y + 10, landerPos.z - 15);
    frontCam.setDistance(30);
    frontCam.lookAt(glm::vec3(0,0,-1));
    
    landerTrackingCam.setDistance(70);
    landerTrackingCam.setFov(65.5);
    landerTrackingCam.disableMouseInput();
    landerTrackingCam.setPosition(50, 20, 0);
    
    
    onBoardCam.setDistance(landerPos.y);
    onBoardCam.setPosition(glm::vec3(landerPos.x, landerPos.y + 5, landerPos.z));
    landerTrackingCam.lookAt(landerPos);
}
void ofApp::loadExhaustVBO(){
    if(landerExhaust.sys->particles.size() < 1) return;
    

    vector<ofVec3f> sizes;
    vector<ofVec3f> points;
    for(int i = 0; i < landerExhaust.sys->particles.size(); i++) {
        points.push_back(landerExhaust.sys->particles[i].position);
        sizes.push_back(ofVec3f(landerExhaust.particleRadius));
    }
    // upload the data to the vbo
    //
    int total = (int)points.size();
    exhaustVBO.clear();
    exhaustVBO.setVertexData(&points[0], total, GL_STATIC_DRAW);
    exhaustVBO.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
void ofApp::loadExplosionVBO(){
    if(explosion.sys->particles.size() < 1) return;
    

    vector<ofVec3f> sizes;
    vector<ofVec3f> points;
    for(int i = 0; i < explosion.sys->particles.size(); i++) {
        points.push_back(explosion.sys->particles[i].position);
        sizes.push_back(ofVec3f(explosion.particleRadius));
    }
    // upload the data to the vbo
    //
    int total = (int)points.size();
    explosionVBO.clear();
    explosionVBO.setVertexData(&points[0], total, GL_STATIC_DRAW);
    explosionVBO.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}
void ofApp::handleExplosion(){
    explosion.update();
    explosion.position = lander.getPosition();
    if(landingForceTooGreat && explosionCount < 1){
        explosion.sys->reset();
        explosion.start();
        explosionSound.play();
        explosionCount++;
        gameLost = true;
        gameMusic.stop();
    }
}
void ofApp::updatePlayerData(){
    if(!bLanderSelected && totalFuel >= 0){ // if the lander isn't being dragged, then we move it with physics if there is still fuel
        playrSys->update();
        glm::vec3 particlePos = playrSys->particles[0].position;
        lander.setPosition(particlePos.x, particlePos.y, particlePos.z);
        landerExhaust.setPosition(lander.getPosition());
        glm::vec3 landerPos = lander.getPosition();
        landerLight.setPosition(landerPos.x, landerPos.y + 1, landerPos.z);
    }
    else{ // if the user is dragging the lander to the starting position
        playrSys->particles[0].position = lander.getPosition();
    }
}
void ofApp::updateCameras(){
    glm::vec3 landerPos = lander.getPosition();
    landerTrackingCam.lookAt(ofVec3f(landerPos.x + 20, landerPos.y, landerPos.z));
    onBoardCam.setDistance(landerPos.y);
    frontCam.setPosition(landerPos.x, landerPos.y + 100, landerPos.z + 15);
    onBoardCam.setPosition(landerPos);
}
void ofApp::update() {
    ofSeedRandom();
    //playGameMusic();
    if(!gameMusic.isPlaying())
        gameMusic.play();
    playSuccessfulLandingSound();

    handleExplosion();
    updatePlayerData();

    updateCameras();
    
    
    collisionDetect();



    
    landerExhaust.update();
    
    playExhaustSound();
    
    checkForLanding();
    
    calcLanderAltitude();
    
    if(isExhaustThrusting) totalFuel -= 1;
    
    if(totalFuel <= 0) // player ran out of fuel, game over
        gameOver = true;
    

}
void ofApp::checkForLanding(){
    glm::vec3 landerPos = lander.getPosition();
    //check if lander landed within the HD landing area
    if(landerPos.x > -25 && landerPos.x < 25 && landerPos.y < 0.05 && landerPos.z > -25 && landerPos.z < 25){
        if(totalFuel > 0 && !landingForceTooGreat){ // successful landing, player won
            gameWon = true;
            gameOver = true;
        }}
}
void ofApp::collisionDetect(){

    ofVec3f min = lander.getSceneMin() + lander.getPosition();
    ofVec3f max = lander.getSceneMax() + lander.getPosition();
    landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
    
    vector<TreeNode> collidedLeafNodes;
    octree.intersect(landerBounds, octree.root, collidedBoxes, collidedLeafNodes);
    
    
    if(collidedBoxes.size() == 0) return; // if no collision occurred, then just return
    // Resolve collision
    glm::vec3 avgNorm = glm::vec3(0,0,0);
    for(int i = 0; i < collidedLeafNodes.size(); i++){
        avgNorm += octree.mesh.getNormal(collidedLeafNodes[i].points[0]);
    }
    avgNorm = glm::normalize(avgNorm);

    glm::vec3 landerVel = playrSys->particles[0].velocity;
    float relativeVelocity = glm::dot(-landerVel, avgNorm);
    if(relativeVelocity > 0) {
        glm::vec3 impulseForce = ((float) restitution + 1) * (relativeVelocity * avgNorm);
        if(impulseForce.y > 1.0){ // player crash landed
            landingForceTooGreat = true;
            gameLost = true;
            gameWon = false;
            gameOver = true;
        }
        playrSys->particles[0].forces += impulseForce * ofGetFrameRate();
    }
}


void ofApp::calcLanderAltitude(){
    
    
    glm::vec3 landerPos = lander.getPosition();

    const float rayStep = 0.1f; // distance to move ray at each step
    float distance = 0.0f; // total distance traveled along the ray

    // Create a ray from the lander position to the terrain position
    Vector3 origin(landerPos.x, landerPos.y, landerPos.z);
    Vector3 direction(0, -1, 0); // ray will always be going downwards for altitude calc
    Ray ray(origin, direction);

    // Move the ray until it intersects with the terrain
    TreeNode altDetNode;
    while (octree.intersect(ray, octree.root, altDetNode)) {
        // Get the height of the terrain at the intersection point
        glm::vec3 terrainVert = octree.mesh.getVertex(altDetNode.points[0]);
        float terrainHeight = terrainVert.y;

        // Calculate the distance traveled along the ray at this step
        float stepDistance = rayStep * glm::length(glm::vec3(ray.direction.x(), ray.direction.y(), ray.direction.z()));
        
        // Accumulate the total distance and update the ray position for the next step
        distance += stepDistance;
        ray.origin = ray.origin + ray.direction * stepDistance;

        // Update the altitude if the terrain height is higher than the current altitude
        if (terrainHeight > altitude) {
            altitude = terrainHeight;
        }
    }

    // Add the total distance traveled along the ray to get the final altitude
    altitude -= distance;
    altitude = roundf(altitude * 1000000.0f) / 1000000.0f;
}
void ofApp::drawStaticStrings(){
    if(backgroundLoaded) background.draw(glm::vec2(0,0), ofGetWidth(), ofGetHeight());
    if (!bHide) gui.draw();
    //allow user the option to see the spacecraft's altitude
    if(showAltitude){
        string aglStr = "Altitude (AGL): " + std::to_string(-altitude);
        ofSetColor(ofColor::white);
        sansFont.drawString(aglStr, 20, 30);
    }
    string fuel = "Available Fuel: " + std::to_string(totalFuel) + " / 120000";
    ofSetColor(ofColor::white);
    sansFont.drawString(fuel, ofGetWidth() - 350, 30);
    if(gameLost){
        string gameLostMessage = "Unsucessful landing, Game over.";
        sansFontLarge.drawString(gameLostMessage, ofGetWidth() / 2.0 - 200, 200);
    }
    if(gameWon && !landingForceTooGreat){
        string gameWonMessage = "Congratulations on the successful landing!!!";
        sansFontLarge.drawString(gameWonMessage, ofGetWidth() / 2.0 - 300, 200);
    }
}
void ofApp::drawLanderData(){
    ofPushMatrix();

        glm::vec3 landerPos = playrSys->particles[0].position;
        glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), landerPos);
        glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(playrSys->particles[0].rotation), glm::vec3(0, 1, 0));
        glm::mat4 transform = translateToOrigin * rotateY;
    ofMultMatrix(transform);
    if(bLanderSelected){ // draw lander bounding box if it's selected or in collision visualizer mode
        ofSetColor(ofColor::orange);
        ofNoFill();
        Octree::drawBox(landerBounds);
    }

    if(!gameLost){ // if the lander crash landed, don't draw the lander
        lander.drawFaces();
        landerExhaust.draw();
        
        exhaustParticleShader.begin();
        
        ofSetColor(ofColor::orange);
        exhaustParticleTex.bind();
        
        exhaustVBO.draw(GL_POINTS, 0, (int) landerExhaust.sys->particles.size());
        exhaustParticleTex.unbind();
        
        exhaustParticleShader.end();
        
    }
    
    explosion.draw();

    exhaustParticleShader.begin();

    explosionParticleTex.bind();
    explosionVBO.draw(GL_POINTS, 0, (int) explosion.sys->particles.size());
    explosionParticleTex.unbind();
    
    exhaustParticleShader.end();

    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::draw() {

    loadExhaustVBO();
    loadExplosionVBO();
    
	glDepthMask(GL_FALSE);
    drawStaticStrings();
    glDepthMask(GL_TRUE);
    
    ofEnableLighting();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    
    mainCamera->begin();
    

	ofPushMatrix();
    moon.drawFaces(); // draw moon terrain
    ofSetColor(ofColor::red);
    ofNoFill();
    Octree::drawBox(landingArea);
    ofSetColor(ofColor::white);


    drawLanderData();
    
    
    

    ofSetColor(ofColor::white);
    
	ofPopMatrix();


    
    mainCamera->end();
    

    ofDisablePointSprites();
    ofDisableBlendMode();
    ofEnableAlphaBlending();
    ofDisableLighting();
    
    
}





void ofApp::keyPressed(int key) {

	switch (key) {
	case 'x':
        showAltitude = !showAltitude;
        break;
    case 'h':
        bHide = !bHide;
        break;
	case 'C':
	case 'c':
		if (mainCamera->getMouseInputEnabled()) mainCamera->disableMouseInput();
		else mainCamera->enableMouseInput();
		break;
	case 'l': // make the moving camera look at the lander
        setCameraTarget(key);
        break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'o':
        mainCamera = &onBoardCam;
        break;
	case 'p':
        mainCamera = &cam;
		break;
	case 't':
        mainCamera = &landerTrackingCam;
        break;
    case 'm':
        mainCamera = &frontCam;
        break;
	case 'u': // make the moving camera look at the selected point on the surface
        setCameraTarget(key);
        break;
	case 'k':
        setCameraTarget(key); // move the camera to the selected position
		break;
	case 'r':
		cam.reset();
		break;
	case ' ':
            explosion.sys->reset();
            explosion.start();
		break;
	case OF_KEY_ALT:
        mainCamera->enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
    case OF_KEY_UP: // move up
        {
            isExhaustThrusting = true;
            glm::vec3 landerHeading = playrSys->particles[0].heading();
            glm::vec3 xAxis = glm::vec3(1,0,0);
            glm::vec3 perpen = glm::cross(landerHeading, xAxis);
            playrSys->particles[0].forces = perpen * (float) thrustForce;
            landerExhaust.sys->reset();
            landerExhaust.start();
            playExhaust = true;

        }
        break;
    case 'a': // move left
        {
            isExhaustThrusting = true;
            glm::vec3 landerHeading = playrSys->particles[0].heading();
            glm::vec3 yAxis = glm::vec3(0,1,0);
            glm::vec3 perpen = glm::cross(landerHeading, yAxis);
            playrSys->particles[0].forces = perpen * (float) thrustForce;
            landerExhaust.sys->reset();
            landerExhaust.start();
            playExhaust = true;
        }
        break;
    case 'd': //move right
        {
            isExhaustThrusting = true;
            glm::vec3 landerHeading = playrSys->particles[0].heading();
            glm::vec3 yAxis = glm::vec3(0,1,0);
            glm::vec3 perpen = glm::cross(landerHeading, yAxis);
            playrSys->particles[0].forces = perpen * -(float) thrustForce;
            landerExhaust.sys->reset();
            landerExhaust.start();
            playExhaust = true;
        }
        break;
    case OF_KEY_DOWN: // move down
        {
            isExhaustThrusting = true;
            glm::vec3 landerHeading = playrSys->particles[0].heading();
            glm::vec3 xAxis = glm::vec3(1,0,0);
            glm::vec3 perpen = glm::cross(landerHeading, xAxis);
            playrSys->particles[0].forces = perpen * -(float) thrustForce;
            landerExhaust.sys->reset();
            landerExhaust.start();
            playExhaust = true;
        }
        break;
    case 'w': // move forward
        {
            isExhaustThrusting = true;
            playrSys->particles[0].forces = playrSys->particles[0].heading() * (float) thrustForce;
            landerExhaust.sys->reset();
            landerExhaust.start();
            playExhaust = true;
        }
        break;
    case 's': // move back
        {
            isExhaustThrusting = true;
            playrSys->particles[0].forces = playrSys->particles[0].heading() * -(float) thrustForce;
            landerExhaust.sys->reset();
            landerExhaust.start();
            playExhaust = true;
        }
        break;
    case OF_KEY_LEFT: // turn left
        playrSys->particles[0].angularForce = 5000.0;
        break;
    case OF_KEY_RIGHT: // turn right
        playrSys->particles[0].angularForce = -5000.0;
        break;
	default:
		break;
	}
}



void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}


void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
        mainCamera->disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_UP:
        {
            isExhaustThrusting = false;
            playrSys->particles[0].forces = glm::vec3(0,0,0);
            playExhaust = false;
        }
		break;
    case OF_KEY_DOWN:
        {
            isExhaustThrusting = false;
            playrSys->particles[0].forces = glm::vec3(0,0,0);
            playExhaust = false;
        }
        break;
    case 'w':
        {
            playrSys->particles[0].forces = glm::vec3(0,0,0);
            isExhaustThrusting = false;
            playExhaust = false;
        }
        break;
    case 's':
        {
            isExhaustThrusting = false;
            playrSys->particles[0].forces = glm::vec3(0,0,0);
            playExhaust = false;
        }
        break;
    case OF_KEY_LEFT:
        playrSys->particles[0].angularForce = 0;
        break;
    case OF_KEY_RIGHT:
        playrSys->particles[0].angularForce = 0;
        break;
    case 'a':
        {
            isExhaustThrusting = false;
            playrSys->particles[0].forces = glm::vec3(0,0,0);
            playExhaust = false;
        }
        break;
    case 'd':
        {
            isExhaustThrusting = false;
            playrSys->particles[0].forces = glm::vec3(0,0,0);
            playExhaust = false;
        }
        break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

	
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
    if(mainCamera->getMouseInputEnabled()) return;


	// if rover is loaded, test for selection
	//
	if (bLanderLoaded) {
		glm::vec3 origin = mainCamera->getPosition();
		glm::vec3 mouseWorld = mainCamera->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000);
		if (hit) {
			bLanderSelected = true;
			mouseDownPos = getMousePointOnPlane(lander.getPosition(), mainCamera->getZAxis());
			mouseLastPos = mouseDownPos;
			bInDrag = true;
		}
		else {
			bLanderSelected = false;
		}
	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

bool ofApp::raySelectWithOctree(ofVec3f &pointRet) {
	ofVec3f mouse(mouseX, mouseY);
	ofVec3f rayPoint = mainCamera->screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - mainCamera->getPosition();
	rayDir.normalize();
	Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
		Vector3(rayDir.x, rayDir.y, rayDir.z));

	pointSelected = octree.intersect(ray, octree.root, selectedNode);

	if (pointSelected) {
		pointRet = octree.mesh.getVertex(selectedNode.points[0]);

	}
	return pointSelected;
}




//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//

    if(mainCamera->getMouseInputEnabled()) return;

	if (bInDrag) {

		glm::vec3 landerPos = lander.getPosition();

		glm::vec3 mousePos = getMousePointOnPlane(landerPos, mainCamera->getZAxis());
		glm::vec3 delta = mousePos - mouseLastPos;
	
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z);
		mouseLastPos = mousePos;

		ofVec3f min = lander.getSceneMin() + lander.getPosition();
		ofVec3f max = lander.getSceneMax() + lander.getPosition();

		Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));

		colBoxList.clear();
		octree.intersect(bounds, octree.root, colBoxList);



	}
	else {
		ofVec3f p;
		raySelectWithOctree(p);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget(char key) {
    if(key == 'u' && pointSelected){
        glm::vec3 lookPos = octree.mesh.getVertex(selectedNode.points[0]);
        cam.lookAt(lookPos);
        mainCamera = &cam;
    }else if (key == 'l' && pointSelected){
        cam.lookAt(lander.getPosition());
        mainCamera = &cam;
    }else if(key == 'k' && pointSelected){
        glm::vec3 newPos = octree.mesh.getVertex(selectedNode.points[0]);
        cam.setPosition(newPos.x, newPos.y + 20, newPos.z);
        mainCamera = &cam;
    }
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent2(ofDragInfo dragInfo) {

	ofVec3f point;
	mouseIntersectPlane(ofVec3f(0, 0, 0), mainCamera->getZAxis(), point);
	if (lander.loadModel(dragInfo.files[0])) {
		lander.setScaleNormalization(false);
		lander.setPosition(1, 1, 0);

		bLanderLoaded = true;
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		cout << "Mesh Count: " << lander.getMeshCount() << endl;
	}
	else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	ofVec2f mouse(mouseX, mouseY);
	ofVec3f rayPoint = mainCamera->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	ofVec3f rayDir = rayPoint - mainCamera->getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		lander.setPosition(0, 0, 0);
		//cout << "number of meshes: " << lander.getNumMeshes() << endl;
		bboxList.clear();
		for (int i = 0; i < lander.getMeshCount(); i++) {
			bboxList.push_back(Octree::meshBounds(lander.getMesh(i)));
		}

		//		lander.setRotation(1, 180, 1, 0, 0);

				// We want to drag and drop a 3D object in space so that the model appears 
				// under the mouse pointer where you drop it !
				//
				// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
				// once we find the point of intersection, we can position the lander/lander
				// at that location.
				//

				// Setup our rays
				//
		glm::vec3 origin = mainCamera->getPosition();
		glm::vec3 camAxis = mainCamera->getZAxis();
		glm::vec3 mouseWorld = mainCamera->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);

			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}

//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays
	//
	glm::vec3 origin = mainCamera->getPosition();
	glm::vec3 camAxis = mainCamera->getZAxis();
	glm::vec3 mouseWorld = mainCamera->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}
