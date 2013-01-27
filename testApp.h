#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "SynapseStreamer.h"

#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------
class testApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void drawFboTest(); // draws some test graphics into the two fbos

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofSoundPlayer songA;
		ofSoundPlayer songB;
		ofSoundPlayer songC;
		ofSoundPlayer songD;

		ofTrueTypeFont font;
		ofxOscSender sender;
		SynapseStreamer synapseStreamer;

		//8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
		ofFbo rgbaFbo; // with alpha
		
};

