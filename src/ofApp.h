#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "Octree.h"
#include <glm/gtx/intersect.hpp>
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent2(ofDragInfo dragInfo);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void initLightingAndMaterials();
        void populateLeafNodesList(TreeNode &node);
		void savePicture();
		void toggleSelectTerrain();
		void setCameraTarget(char key);
        void setUpLighting();
        void setUpCameras();
        void setUpEmitters();
        void loadData();
        void initLevelColors();
        void calcLanderAltitude();
        void collisionDetect();
		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
		bool raySelectWithOctree(ofVec3f &pointRet);
        void loadExhaustVBO();
        void loadExplosionVBO();
        void playExhaustSound();
        void playSuccessfulLandingSound();
        void playGameMusic();
        void checkForLanding();
        void drawStaticStrings();
        void drawLanderData();
        void setUpPlayerData();
        void setUpGui();
        void handleExplosion();
        void updatePlayerData();
        void updateCameras();
		glm::vec3 getMousePointOnPlane(glm::vec3 p , glm::vec3 n);

		ofEasyCam cam;
        ofEasyCam onBoardCam, frontCam;
        ofEasyCam landerTrackingCam;
        ofEasyCam *mainCamera;
    
        ofTrueTypeFont sansFont, sansFontLarge;
        
        Particle player;
        ParticleEmitter explosion;
    
    
    
        ofImage background;
        ofTexture exhaustParticleTex, explosionParticleTex;
        ofShader exhaustParticleShader, explosionParticleShader;
        ofVbo exhaustVBO, explosionVBO;
        bool backgroundLoaded = false;
    
        int explosionCount = 0;
        int successfulLandingPlayCount = 0;
    
        float altitude = 0;
        bool showAltitude = true;
        bool updatedAltitude = false;
    
    
		ofxAssimpModelLoader moon, lander;

		Box boundingBox, landerBounds;
		Box testBox;
        Box landingArea;
		vector<Box> colBoxList;
		bool bLanderSelected = false;
		Octree octree;
		TreeNode selectedNode;
		glm::vec3 mouseDownPos, mouseLastPos;
		bool bInDrag = false;

        ofxFloatSlider thrustForce;
        ofxFloatSlider restitution;
        ofxToggle collisionVisualizer;
		ofxPanel gui;
        ofColor landerBoundingBoxColor;

        
        ParticleEmitter landerExhaust;
        TurbulenceForce *turbForce;
        TurbulenceForce *landerTurbulance;
        GravityForce *gravityForce;
        ImpulseRadialForce *radialForce;

    
        Particle *plyr;
        ParticleSystem *playrSys;
        ofLight keyLight, fillLight, landerLight;
    
        ofSoundPlayer exhaustSound, gameMusic, explosionSound, successfulLanding;
        
		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bPointSelected;
		bool bHide;
		bool pointSelected = false;
		bool bLanderLoaded;
		bool bTerrainSelected;
        bool playExhaust = false;
        bool isExhaustThrusting = false;
        bool gameWon = false;
        bool gameLost = false;
        bool gameOver = false;
        bool landingForceTooGreat = false;
        
        int totalFuel = 120000; //the player has 120000 milli-seconds (2 minutes) of fuel

	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		vector<Box> bboxList;
        vector<Box> collidedBoxes;
		const float selectionRange = 4.0;
};
