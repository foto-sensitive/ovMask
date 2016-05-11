#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		~ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Unwraps 360° media
		void unwrap();

		//Renders the sphere
		GLUquadricObj *quadric;

		//Background, Foreground and Mask Layers
		ofImage bg, fg, maskImg;
		ofVideoPlayer mask;

		//Adds camera and controls
		ofEasyCam cam;

		//FBOs and Shader for masking and texturing
		ofShader shader;
		ofFbo fboMask;
		ofFbo fboShader;
		ofFbo fboSphereTex;

		//Iterator
		int i;

		//Path to your media
		string path;
		//Determines video or image
		bool vidFormat;

		//GUI
		ofxPanel gui;
		ofxIntSlider opacity;
};
