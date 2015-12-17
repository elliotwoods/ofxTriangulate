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
	
	Ray cray = camera.castCoordinate(camXY);
	Ray pray = projector.castCoordinate(projXY);
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
