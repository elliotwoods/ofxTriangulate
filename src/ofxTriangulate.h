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
	static void Triangulate(ofxGraycode::DataSet, ofxRay::Camera, ofxRay::Projector, ofMesh & mesh, float maxLength);
};