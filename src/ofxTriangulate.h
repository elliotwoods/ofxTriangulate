#pragma once
//
//  ofxTriangulate.h
//  ofxTriangulate
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

//include me in your project
//and i'll include everything else

#include "ofxRay.h"
#include "ofxGraycode.h"
#include "ofMesh.h"

class ofxTriangulate {
public:
	
	///1 x Camera, 1 x Projector. Triangulate an ofxGraycode::DataSet
	static void Triangulate(ofxGraycode::DataSet, ofxRay::Camera, ofxRay::Projector, ofMesh & mesh, float maxLength = std::numeric_limits<float>::max(), bool giveColor=true, bool giveTexCoord=true);
	
	///1 x Camera, 1 x Projector. Triangulate an individual point
	static bool Triangulate(int cameraPixelIndex, int projectorPixelIndex, const ofxRay::Camera&, const ofxRay::Projector&, ofVec3f& worldXYZResult, float maxLength = std::numeric_limits<float>::max());
	
	///2 x Camera. Triangulate a single point
	static ofVec3f Triangulate(int cam1PixelIndex, int cam2PixelIndex, ofxRay::Camera & camera1, ofxRay::Camera & camera2);
	
	///2 x Camera. Triangulate a pair of ofxGraycode::DataSet's
	static void Triangulate(ofxGraycode::DataSet data1, ofxGraycode::DataSet data2, ofxRay::Camera & camera1, ofxRay::Camera & camera2, ofMesh & mesh, float maxLength);
};
