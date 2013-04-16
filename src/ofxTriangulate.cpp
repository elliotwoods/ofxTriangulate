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

static inline ofVec2f getNorm(const uint index, const uint width, const uint height) {
	uint x = index % width;
	uint y = index / width;
	return ofVec2f(2.0f * (float(x) + 0.5) / float(width) - 1.0f,
			2.0f * (float(y) + 0.5) / float(height) - 1.0f);
}

void ofxTriangulate::Triangulate(DataSet data, Camera camera, Projector projector, ofMesh & mesh, float maxLength) {
	
	mesh.clear();
	float maxLength2=maxLength*maxLength;
	
	DataSet::const_iterator it;
	for (it = data.begin(); it != data.end(); ++it) {
		DataSet::const_iterator::reference r = *it;
		if ((*it).active) {
			
			ofVec2f camXY = r.getCameraXYNorm();
			ofVec2f projXY = r.getProjectorXYNorm();
			
			Ray cray = camera.castCoordinate(camXY);
			Ray pray = projector.castCoordinate(projXY);
			Ray intersect = cray.intersect(pray);
			
			if (intersect.getLengthSquared() > maxLength2){
				continue;
			}
			//ofVec3f xyz = intersect.getMidpoint();
			ofVec3f xyz = intersect.s;
			
			mesh.addVertex(xyz);
			mesh.addTexCoord(camXY);
			mesh.addColor(ofFloatColor(projXY.x, projXY.y, 0.0f));
		}
	}
}

void ofxTriangulate::Triangulate(ofxGraycode::DataSet data1, ofxGraycode::DataSet data2, ofxRay::Camera & camera1, ofxRay::Camera & camera2, ofMesh & mesh, float maxLength){
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

ofVec3f ofxTriangulate::Triangulate(float cam1PixelIndex, float cam2PixelIndex, Camera & camera1, Camera & camera2) {

	ofVec2f cam1XYNorm = getNorm(cam1PixelIndex,camera1.getWidth(),camera1.getHeight());
	ofVec2f cam2XYNorm = getNorm(cam2PixelIndex,camera2.getWidth(),camera2.getHeight());
	ofxRay::Ray cray1 = camera1.castCoordinate(cam1XYNorm);
	ofxRay::Ray cray2 = camera2.castCoordinate(cam2XYNorm);
	ofxRay::Ray intersect = cray1.intersect(cray2);

	return intersect.s;
}
