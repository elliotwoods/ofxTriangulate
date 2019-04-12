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
	static void Triangulate(const ofxGraycode::DataSet &, const ofxRay::Camera &, const ofxRay::Projector &, ofMesh & mesh, float maxLength = std::numeric_limits<float>::max(), bool giveColor=true, bool giveTexCoord=true);
	///1 x Camera, 1 x Projector. Triangulate a point in a ofxGraycode::DataSet
	static bool Triangulate(ofxGraycode::DataSet::const_iterator& it, const ofxRay::Camera &, const ofxRay::Projector&, glm::vec3 & worldXYZResult, float maxLength2 = std::numeric_limits<float>::max());

	///1 x Camera, 1 x Projector. Triangulate an ofxGraycode::DataSet with a custom undistort function
	static void Triangulate(const ofxGraycode::DataSet &, const ofxRay::Camera &, const ofxRay::Projector &, ofMesh & mesh, float maxLength, bool giveColor, bool giveTexCoord, const function<glm::vec2(const glm::vec2 &)> &);
	///1 x Camera, 1 x Projector. Triangulate a point in a ofxGraycode::DataSet with a custom undistort function
	static bool Triangulate(ofxGraycode::DataSet::const_iterator& it, const ofxRay::Camera &, const ofxRay::Projector&, glm::vec3 & worldXYZResult, float maxLength2, const function<glm::vec2(const glm::vec2 &)> &);

	
	///1 x Camera, 1 x Projector. Triangulate an individual point by index
	static bool Triangulate(int cameraPixelIndex, int projectorPixelIndex, const ofxRay::Camera&, const ofxRay::Projector&, glm::vec3 & worldXYZResult, float maxLength = std::numeric_limits<float>::max());
	
	///2 x Camera. Triangulate a single point
	static glm::vec3 Triangulate(int cam1PixelIndex, int cam2PixelIndex, const ofxRay::Camera & camera1, const ofxRay::Camera & camera2);
	
	///2 x Camera. Triangulate a pair of ofxGraycode::DataSet's
	static void Triangulate(const ofxGraycode::DataSet & data1, const ofxGraycode::DataSet & data2, const ofxRay::Camera & camera1, const ofxRay::Camera & camera2, ofMesh & mesh);

	static void Triangulate(const vector<glm::vec2> & cameraPointsA, const vector<glm::vec2> & cameraPointsB, const ofxRay::Camera & cameraA, const ofxRay::Camera & cameraB, vector<glm::vec3> & worldSpacePoints, float maxLength = std::numeric_limits<float>::max());
};
