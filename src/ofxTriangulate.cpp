//
//  ofxTriangulate.cpp
//  ofxTriangulate
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxTriangulate.h"

using namespace ofxRay;
using namespace ofxGraycode;

//--------
void ofxTriangulate::Triangulate(const DataSet & data, const Camera & camera, const Projector & projector, ofMesh & mesh, float maxLength, bool giveColor, bool giveTexCoord) {
	
	mesh.clear();
	float maxLength2=maxLength*maxLength;
	ofVec3f xyz;
	
	DataSet::const_iterator it;
	for (it = data.begin(); it != data.end(); ++it) {
		DataSet::const_iterator::reference r = *it;
		if ((*it).active) {
			if(Triangulate(it, camera, projector, xyz, maxLength2)) {
				mesh.addVertex(xyz);
				
				if (giveColor) {
					auto cameraNorm = (*it).getCameraXYNorm();
					mesh.addColor(ofFloatColor(cameraNorm.x, cameraNorm.y, 0.0f));
				}
				
				if (giveTexCoord) {
					auto projXY = (*it).getProjectorXYNorm();
					mesh.addTexCoord(projXY);
				}
				
			}
		}
	}
}

//--------
bool ofxTriangulate::Triangulate(ofxGraycode::DataSet::const_iterator& it, const ofxRay::Camera& camera, const ofxRay::Projector& projector, ofVec3f& worldResult, float maxLength2) {
	ofVec2f camXY = (*it).getCameraXYNorm();
	ofVec2f projXY = (*it).getProjectorXYNorm();
	
	Ray cray = camera.castCoordinate(camXY); // give me the 3D ray for a camera pixel
	Ray pray = projector.castCoordinate(projXY); // give me the 3D ray for a projector pixel
	Ray intersect = cray.intersect(pray);
	
	const auto lengthSquared = intersect.getLengthSquared();
	if (lengthSquared > maxLength2) {
		return false;
	}
	worldResult = intersect.getMidpoint();
	return true;
}

//--------
bool ofxTriangulate::Triangulate(int cameraPixelIndex, int projectorPixelIndex, const ofxRay::Camera& camera, const ofxRay::Projector& projector, ofVec3f& worldXYZResult, float maxLength) {
	const ofVec2f cameraCoordinate = camera.getCoordinateFromIndex(cameraPixelIndex);
	const ofVec2f projectorCoordinate = projector.getCoordinateFromIndex(projectorPixelIndex);
	
	Ray cameraRay = camera.castCoordinate(cameraCoordinate);
	Ray projectorRay = projector.castCoordinate(projectorCoordinate);
	Ray intersect = cameraRay.intersect(projectorRay);
	
	if (intersect.getLength() <= maxLength) {
		worldXYZResult = intersect.getMidpoint();
		return true;
	} else {
		return false;
	}
}

//--------
void ofxTriangulate::Triangulate(const DataSet & data1, const DataSet & data2, const Camera & camera1, const Camera & camera2, ofMesh & mesh, float maxLength){
	mesh.clear();
	float maxLength2=maxLength*maxLength;

	for(int i=0;i<data1.getDataInverse().size();i++){
		int d1 = data1.getDataInverse()[i];
		int d2 = data2.getDataInverse()[i];
		if(data1.getActive()[d1] && data2.getActive()[d2]){
			ofVec3f world = Triangulate(d1,d2,camera1,camera2);
			if(world.lengthSquared()<maxLength2)
				mesh.addVertex(world);
		}
	}
}

//--------
ofVec3f ofxTriangulate::Triangulate(int cam1PixelIndex, int cam2PixelIndex, const Camera & camera1, const Camera & camera2) {

	ofVec2f cam1XYNorm = camera1.getCoordinateFromIndex(cam1PixelIndex);
	ofVec2f cam2XYNorm = camera2.getCoordinateFromIndex(cam2PixelIndex);
	ofxRay::Ray cray1 = camera1.castCoordinate(cam1XYNorm);
	ofxRay::Ray cray2 = camera2.castCoordinate(cam2XYNorm);
	ofxRay::Ray intersect = cray1.intersect(cray2);

	return intersect.getMidpoint();
}

//----------
void ofxTriangulate::Triangulate(const vector<ofVec2f> & cameraPointsA, const vector<ofVec2f> & cameraPointsB, const ofxRay::Camera & cameraA, const ofxRay::Camera & cameraB, vector<ofVec3f> & worldSpacePoints, float maxLength /*= std::numeric_limits<float>::max()*/) {
	if (cameraPointsA.size() != cameraPointsB.size()) {
		ofLogError("ofxTriangulate") << "input lengths do not match";
		return;
	}

	auto size = cameraPointsA.size();

	vector<ofxRay::Ray> cameraRaysA;
	cameraA.castPixels(cameraPointsA, cameraRaysA);

	vector<ofxRay::Ray> cameraRaysB;
	cameraB.castPixels(cameraPointsB, cameraRaysB);

	auto maxLengthSquared = maxLength * maxLength;

	worldSpacePoints.clear();
	for (size_t i = 0; i < size; i++) {
		auto intersection = cameraRaysA[i].intersect(cameraRaysB[i]);
		if (intersection.getLengthSquared() > maxLengthSquared) {
			worldSpacePoints.push_back(ofVec3f());
		}
		else {
			worldSpacePoints.push_back(intersection.getStart());
		}
	}
}
