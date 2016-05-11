#include "ofApp.h"

//-------------------------------------------------------------
ofApp::~ofApp() {

}

//--------------------------------------------------------------
void ofApp::setup(){

	//Load Shaders
#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if (ofIsGLProgrammableRenderer()) {
		shader.load("shadersGL3/shader");
	}
	else {
		shader.load("shadersGL2/shader");
	}
#endif

	//Prepare quadric for sphere
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	//Load Demo Mask, Background, Foreground
	mask.load("mask.avi");
	mask.setLoopState(OF_LOOP_NORMAL);
	mask.play();
	fg.loadImage("ocean.jpg");
	bg.loadImage("palace.jpg");

	//Determines if mask is a video or image, starting of with a video
	vidFormat = true;

	int width = bg.getWidth();
	int height = bg.getHeight();
	
	//Allocate memory to FBO's
	fboMask.allocate(width, height);
	fboShader.allocate(width, height);
	fboSphereTex.allocate(width, height);

	// Clear the FBO's
	// otherwise it will bring some junk with it from the memory
	fboMask.begin();
	ofClear(0, 0, 0, 255);
	fboMask.end();
	fboShader.begin();
	ofClear(0, 0, 0, 255);
	fboShader.end();
	fboSphereTex.begin();
	ofClear(0, 0, 0, 255);
	fboSphereTex.end();

	//Place camera inside sphere at 0,0,0
	cam.setAutoDistance(false);

	//GUI
	gui.setup();
	gui.setPosition(ofGetWidth()*0.0138, ofGetHeight()*0.02);
	gui.add(opacity.setup("Opacity", 200, 0, 255));
}

//--------------------------------------------------------------
void ofApp::update(){
		mask.update();
}

//--------------------------------------------------------------
void ofApp::draw(){




	//Begin alpha mask FBO which we draw into.
	fboMask.begin();
    
	//Determine weather masking with still image or video
	if(vidFormat)
	mask.draw(0, 0, bg.getWidth(), bg.getHeight());
	else maskImg.draw(0, 0, bg.getWidth(), bg.getHeight());

	fboMask.end();




	//Begin FBO containing the shader
	fboShader.begin();
	// Clearing mask alpha to make it transparent
	ofClear(0, 0, 0, 0);

	shader.begin();
	//Pass Mask FBO to the shader
	shader.setUniformTexture("maskTex", fboMask.getTextureReference(), 1);

	//Draws Foreground
	fg.draw(0, 0);

	shader.end();

	fboShader.end();




    //Begin FBO we will use to texture our sphere
	fboSphereTex.begin();
	//Draw the background image
	bg.draw(0, 0);
	ofSetColor(255, opacity);
	//Then draw the masked fbo on top
	fboShader.draw(0, 0);
	ofSetColor(255);
	fboSphereTex.end();




	//Place camera inside the sphere
	cam.begin();
	//Map the Masked, 360° texture to sphere
	unwrap();
	cam.end();




	//GUI
	ofSetColor(0, 130);
	ofRectMode(OF_RECTMODE_CORNER);
	ofRect(9, 9, 357, 110);
	ofSetColor(255);
	ofDrawBitmapString("Drag & drop videos and  pictures  onto the", 20, 70);
	ofDrawBitmapString("window to unwrap 360° media. Drag the mouse", 20, 90);
	ofDrawBitmapString("across the screen to look around.", 20, 110);
	ofDrawBitmapString(path, 20, ofGetHeight()*0.97);

	gui.draw();

}

//--------------------------------------------------------------
void ofApp::unwrap() {

	ofSetColor(255, 255, 255);

	//Bind video or image
	fboSphereTex.getTextureReference().bind();

	ofPushMatrix();
	ofRotateX(90);

	//Apply operations to texture matrix
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();

	//Scale texture to sphere
	ofScale(fboSphereTex.getWidth(), fboSphereTex.getHeight());

	//Apply operations to model matrix
	glMatrixMode(GL_MODELVIEW);

	//Enable depth comparisons, needed so you see the correct planes from your perspective
	glEnable(GL_DEPTH_TEST);
	//Disables the use of ARB textures, has to be disabled for drawing sphere
	ofDisableArbTex();

	gluSphere(quadric, 200, 100, 100);

	//Disables depth comparisons, needed for performing texture operations
	glDisable(GL_DEPTH_TEST);
	//Enables the use of ARB textures, needed to bind textures
	ofEnableArbTex(); //needed for textures to work with gluSphe

					  //Apply operations to texture matrix
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	//Unbind video or image
	fboSphereTex.getTextureReference().unbind();

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') {
		mask.loadMovie("mask.avi");
		mask.play();
	}
	if (key == '2'){
		mask.loadMovie("mask2.avi");
    	mask.play();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo info) {

	//Get dragged file path
	path = info.files[0];
	//Get extension
	ofFile file(ofToDataPath(path));

	
	//Loads movie if video is dropped
	if (file.getExtension() == "avi") {
		mask.loadMovie(info.files[0]);
		mask.play();
		vidFormat = true;
	}


	//Loads image if picture is dropped
	if (file.getExtension() == "bmp" || file.getExtension() == "jpg" || file.getExtension() == "png") {
		maskImg.loadImage(info.files[0]);
		vidFormat = false;
	}
}