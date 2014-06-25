#pragma once

#include "ofMain.h"
#include "ofxGraycode.h"
#include "ofxRay.h"
#include "ofxTriangulate.h"

using namespace ofxGraycode;

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxRay::Camera camera1;
		ofxRay::Camera camera2;

		PayloadGraycode payload;
		Decoder decoder1, decoder2;
		ofVboMesh mesh;
		ofTexture mean1, inverseMean1, mean2, inverseMean2;
		ofImage depthMap;
		ofEasyCam easyCam;

		ofImage imgColor1;
		ofImage imgColor2;
		ofImage colorFromProjector;

		enum Mode{
			Mean=0,
			DepthMap,
			ThreeD,
			ThroughCamera1,
			ThroughCamera2
		}mode;
};
