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
void ofxTriangulate::Triangulate(DataSet data, Camera camera, Projector projector, ofMesh & mesh, float maxLength, bool giveColor, bool giveTexCoord) {
	
	mesh.clear();
	float maxLength2=maxLength*maxLength;
	
	DataSet::const_iterator it;
	for (it = data.begin(); it != data.end(); ++it) {
		DataSet::const_iterator::reference r = *it;
		if ((*it).active) {
			ofVec2f camXY = r.getCameraXYNorm();
			ofVec2f projXY = r.getProjectorXYNorm();
			
			camXY = camera.undistortCoordinate(camXY);
			
			Ray cray = camera.castCoordinate(camXY);
			Ray pray = projector.castCoordinate(projXY);
			Ray intersect = cray.intersect(pray);
			
			const auto lengthSquared = intersect.getLengthSquared();
			if (lengthSquared > maxLength2){
				continue;
			}
			ofVec3f xyz = intersect.getMidpoint();
			
			mesh.addVertex(xyz);
			
			if (giveColor) {
				mesh.addColor(ofFloatColor(projXY.x, projXY.y, 0.0f));
			}
			
			if (giveTexCoord) {
				mesh.addTexCoord(camXY);
			}
		}
	}
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
void ofxTriangulate::Triangulate(DataSet data1, DataSet data2, Camera & camera1, Camera & camera2, ofMesh & mesh, float maxLength){
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
ofVec3f ofxTriangulate::Triangulate(int cam1PixelIndex, int cam2PixelIndex, Camera & camera1, Camera & camera2) {

	ofVec2f cam1XYNorm = camera1.getCoordinateFromIndex(cam1PixelIndex);
	ofVec2f cam2XYNorm = camera2.getCoordinateFromIndex(cam2PixelIndex);
	ofxRay::Ray cray1 = camera1.castCoordinate(cam1XYNorm);
	ofxRay::Ray cray2 = camera2.castCoordinate(cam2XYNorm);
	ofxRay::Ray intersect = cray1.intersect(cray2);

	return intersect.getMidpoint();
}
