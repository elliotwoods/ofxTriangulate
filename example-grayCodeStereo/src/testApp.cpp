#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	string path = "";
	payload.init(1024, 768);
	encoder.init(payload);



	decoder1.init(payload);
	for(int i=0;i<encoder.getFrameCount();i++){
		imgColor.loadImage(path+"capture1/"+ofToString(i)+".png");
		decoder1 << imgColor;
	}

	mean1.allocate(decoder1.getDataSet().getMean());
	mean1.loadData(decoder1.getDataSet().getMean());

	inverseMean1.allocate(decoder1.getDataSet().getMeanInverse());
	inverseMean1.loadData(decoder1.getDataSet().getMeanInverse());


	decoder2.init(payload);
	for(int i=0;i<encoder.getFrameCount();i++){
		imgColor.loadImage(path+"capture2/"+ofToString(i)+".png");
		decoder2 << imgColor;
	}

	mean2.allocate(decoder2.getDataSet().getMean());
	mean2.loadData(decoder2.getDataSet().getMean());

	inverseMean2.allocate(decoder2.getDataSet().getMeanInverse());
	inverseMean2.loadData(decoder2.getDataSet().getMeanInverse());

	ofEnableAlphaBlending();
	ofBackground(255);

	depthMap.allocate(decoder1.getDataSet().getDataInverse().getWidth(),decoder1.getDataSet().getDataInverse().getHeight(),OF_IMAGE_GRAYSCALE);
	depthMap.getPixelsRef().set(0);
	imgColor.loadImage(path+"capture2/1.png");
    mesh.setMode(OF_PRIMITIVE_POINTS);

	float zNear = -1;
	float zFar = 1000;
	float cx = 6.7843858886797614e+02;
	float cy = 3.4517466503352483e+02;
	float fx = 1.0719532879358244e+03;
	float fy = 1.0703376726883023e+03;
	float w = 1280;
	float h = 720;

	ofxRay::Camera camera1(ofVec2f(fx,fy),ofVec2f(cx,cy),zNear,zFar,w,h);
	ofxRay::Camera camera2(ofVec2f(fx,fy),ofVec2f(cx,cy),zNear,zFar,w,h);
	camera1.move(-80,0,0);
    camera2.move(80,0,0);

	for(int i=0;i<decoder1.getDataSet().getDataInverse().size();i++){
		int d1 = decoder1.getDataSet().getDataInverse()[i];
		int d2 = decoder2.getDataSet().getDataInverse()[i];
		int x = i%decoder1.getDataSet().getDataInverse().getWidth();
		int y = i/decoder1.getDataSet().getDataInverse().getWidth();
		if(decoder1.getDataSet().getActive()[d1] && decoder2.getDataSet().getActive()[d2]){
			int d2_x = d2%decoder1.getDataSet().getData().getWidth();
			int d2_y = d2/decoder2.getDataSet().getData().getWidth();
			ofVec3f world = ofxTriangulate::Triangulate(d1,d2,camera1,camera2);
			mesh.addVertex(world);
			mesh.addTexCoord(ofVec2f(d2_x,d2_y));//ofFloatColor(imgColor2.getColor(d2_x,d2_y)));
			depthMap.getPixelsRef()[x+y*depthMap.getWidth()] = (world.z*255)/zFar;
		}
	}
	depthMap.update();
	mesh.save("mesh.ply");

	ofVec3f centroid=mesh.getCentroid();

	camera.setTarget(centroid);

	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glPointSize(2);
	glEnable(GL_DEPTH_TEST);

	mode = ThreeD;
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	int height = mean1.getHeight()*640/mean1.getWidth();
	switch(mode){
	case Mean:
		mean1.draw(0,0,640,height);
		mean2.draw(640,0,640,height);
		ofSetColor(255,180);
		inverseMean1.draw(0,height,640,480);
		ofSetColor(255,180);
		inverseMean2.draw(0,height,640,480);
		break;
	case DepthMap:
		depthMap.draw(0,0);
		break;
	case ThreeD:
		camera.begin();
		imgColor.bind();
		mesh.draw();
		imgColor.unbind();
		camera.end();
		break;
	}
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		mode= (Mode)(int(mode)+ 1);
		mode= (Mode)(int(mode)%3);
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
