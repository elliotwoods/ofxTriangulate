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
