#include "testApp.h"

#define wt_hand 0.20//.125
#define wt_knee 0.20//.175
#define wt_torso 0.1//.25
#define wt_head 0.1//.15

int sleep_check =0;
int counter = 0;

int fadeAmnt = 10;

vector<double> curPos_LH;
vector<double> oldPos_LH;

vector<double> curPos_RH;
vector<double> oldPos_RH;

vector<double> curPos_LK;
vector<double> oldPos_LK;

vector<double> curPos_RK;
vector<double> oldPos_RK;

vector<double> curPos_T;
vector<double> oldPos_T;

vector<double> curPos_H;
vector<double> oldPos_H;

vector <double> leftHand;
vector< double > rightHand;

#define samples 15

double s_check[samples];
int i=0, k=0;
int song_count=2;

float song_lower = 0.75, song_upper = 1.25;
double dist_x, dist_y, dist_z;
double speed;
double D=0, cur_avg=0, old_avg;
int flag = 0;

ofSoundPlayer song[5];

//--------------------------------------------------------------
void testApp::setup(){

	song[0].loadSound("song1.mp3");
	song[1].loadSound("song2.mp3");
	song[2].loadSound("song3.mp3");
	song[3].loadSound("song4.mp3");
	song[4].loadSound("song5.mp3");

	song[0].setVolume(1);
	song[1].setVolume(1);
	song[2].setVolume(1);
	song[3].setVolume(1);
	song[4].setVolume(1);

	ofBackground(0, 0, 0);
	ofSetFrameRate(60);

	//allocate our fbos. 
	//providing the dimensions and the format for the,
	rgbaFbo.allocate(810, 530, GL_RGBA); // with alpha, 8 bits red, 8 bits green, 8 bits blue, 8 bits alpha, from 0 to 255 in 256 steps	
	
	rgbaFbo.begin();
	ofClear(255,255,255, 0);
    rgbaFbo.end();

	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
	synapseStreamer.openSynapseConnection();
	synapseStreamer.trackAllJoints(false);

	synapseStreamer.trackLeftHand(true);
	synapseStreamer.trackRightHand(true);

	synapseStreamer.trackLeftKnee(true);
	synapseStreamer.trackRightKnee(true);

	synapseStreamer.trackTorso(true);
	synapseStreamer.trackHead(true);

}

//--------------------------------------------------------------
void testApp::update(){
	ofEnableAlphaBlending();
	synapseStreamer.parseIncomingMessages();
	if(synapseStreamer.getNewMessage()){

		 leftHand = synapseStreamer.getLeftHandJoint();
		 rightHand = synapseStreamer.getRightHandJoint();

		 vector< double > leftKnee = synapseStreamer.getLeftKneeJoint();
		 vector< double > rightKnee = synapseStreamer.getRightKneeJoint();

		 vector< double > torso = synapseStreamer.getTorsoJoint();
		 vector< double > head = synapseStreamer.getHeadJoint();

		//Drawing some graphics into the fbo
		rgbaFbo.begin();
			drawFboTest();
		rgbaFbo.end();

		 /*--------------------------------------------------------------------------------*/
		 if(!song[song_count].getIsPlaying())
			 song[song_count].play();

		 if(counter==0)
		 {
			 D=0;
			 curPos_LH = leftHand;
			 curPos_RH = rightHand;
			 curPos_LK = leftKnee;
			 curPos_RK = rightKnee;
			 curPos_T = torso;
			 curPos_H = head;
		 }
		 

		 if(counter>0)
		 {
			 oldPos_LH = curPos_LH;
			 curPos_LH = leftHand;

			 oldPos_RH = curPos_RH;
			 curPos_RH = rightHand;

			 oldPos_LK = curPos_LK;
			 curPos_LK = leftKnee;

			 oldPos_RK = curPos_RK;
			 curPos_RK = rightKnee;

			 oldPos_T = curPos_T;
			 curPos_T = torso;

			 oldPos_H = curPos_H;
			 curPos_H = head;

			 //Get distance b/w cur and old
			 dist_x = curPos_LH[0]-oldPos_LH[0];
			 dist_y = curPos_LH[1]-oldPos_LH[1];
			 dist_z = curPos_LH[2]-oldPos_LH[2];
			 D += wt_hand*(sqrt(pow(dist_x,2) + pow(dist_y,2)));

			 dist_x = curPos_RH[0]-oldPos_RH[0];
			 dist_y = curPos_RH[1]-oldPos_RH[1];
			 dist_z = curPos_RH[2]-oldPos_RH[2];
			  D += wt_hand*(sqrt(pow(dist_x,2) + pow(dist_y,2)));

			 dist_x = curPos_LK[0]-oldPos_LK[0];
			 dist_y = curPos_LK[1]-oldPos_LK[1];
			 dist_z = curPos_LK[2]-oldPos_LK[2];
			  D += wt_knee*(sqrt(pow(dist_x,2) + pow(dist_y,2)));

			 dist_x = curPos_RK[0]-oldPos_RK[0];
			 dist_y = curPos_RK[1]-oldPos_RK[1];
			 dist_z = curPos_RK[2]-oldPos_RK[2];
			  D += wt_knee*(sqrt(pow(dist_x,2) + pow(dist_y,2)));

			 dist_x = curPos_T[0]-oldPos_T[0];
			 dist_y = curPos_T[1]-oldPos_T[1];
			 dist_z = curPos_T[2]-oldPos_T[2];
			  D += wt_torso*(sqrt(pow(dist_x,2) + pow(dist_y,2)));

			 dist_x = curPos_H[0]-oldPos_H[0];
			 dist_y = curPos_H[1]-oldPos_H[1];
			 dist_z = curPos_H[2]-oldPos_H[2];
			  D += wt_head*(sqrt(pow(dist_x,2) + pow(dist_y,2)));
			  
		 }
	 
		 counter++;

		 if(counter==10)
		 {
			 counter=0;

			 //PREVIOUS BUTTON
					if(rightHand[0] > 530 && rightHand[1] > 270)	//topmost right
					{
						if(song_count<4)
						 {
								 if(song[song_count].getIsPlaying())
								 {
									 song[song_count].stop();
									 song[song_count+1].play();
									 song_count++;
								 }
						}
					}

					//NEXT BUTTON
					if(leftHand[0] < -885 && leftHand[1] > 270)	//topmost left
					{
						if(song_count>0)
						 {
								 if(song[song_count].getIsPlaying())
								 {
									 song[song_count].stop();
									 song[song_count-1].play();
									 song_count--;
								 }
						}
					}

			 speed=D/10;
			 s_check[k] = speed;
			 k++;

			 if(k>samples-1)
			 {
				 k=0;			 

				 old_avg = cur_avg;
				 cur_avg = 0;

				 for(i=0;i<samples;i++)
					 cur_avg += s_check[i];

				 cur_avg = cur_avg/samples;
				 
					 if(cur_avg>=10 && cur_avg<=14)
					 {
						 if(old_avg < 10)
							 if(song[song_count].getSpeed() <= song_upper)
								song[song_count].setSpeed(song[song_count].getSpeed()+0.10);

						 if(old_avg >=14)
							 if(song[song_count].getSpeed() >= song_lower)
								song[song_count].setSpeed(song[song_count].getSpeed()-0.10);	 
					 }
				 
					  if(cur_avg>=14 && cur_avg<=20)
					 {
						 if(old_avg < 14)
							 if(song[song_count].getSpeed()+0.10 <= song_upper)
								 song[song_count].setSpeed(song[song_count].getSpeed()+0.10);

						 if(old_avg >=20)
							 if(song[song_count].getSpeed()-0.10 >= song_lower)
								song[song_count].setSpeed(song[song_count].getSpeed()-0.10);	 
					 }

					 if(cur_avg <10)
					 {
						if(song[song_count].getSpeed() - 0.10 >= song_lower)
						{			 
							  song[song_count].setSpeed(song[song_count].getSpeed()-0.10);
						}
					 }

					 if(cur_avg <6)
					 {
						 if(song_count>0)
						 {
							 if(song[song_count].getIsPlaying())
							 {
								 song[song_count].stop();
								 song[song_count-1].play();
								 song_count--;
							 }
						 }
					 }

					 if(cur_avg >20)
					 {
						 if(song_count<4)
						 {
							 if(song[song_count].getIsPlaying())
							 {
								 song[song_count].stop();
								 song[song_count+1].play();
								 song_count++;
							 }
						 }
					 }

			 }
		 }

	}
}


//--------------------------------------------------------------
void testApp::drawFboTest(){

	ofFill();
	ofSetColor(255,255,255, fadeAmnt);
	ofRect(0,0,810,530);

	// Draw graphics	
	ofNoFill();
	ofSetColor(255,255,255);
	
	//we draw a cube in the center of the fbo and rotate it based on time
	ofPushMatrix();
		ofTranslate(400,250,0);
		ofRotate(ofGetElapsedTimef()*30, 1,0,0.5);
		ofBox(0,0, 100);
	ofPopMatrix();
	
	//also draw based on our hand positions
	ofFill();	   
	
	//ofSetColor(255,0,0);
	ofSetColor((int)leftHand[0]%255,(int)leftHand[1]%255,(int)leftHand[2]%255);
	ofCircle((((int)leftHand[0] + 1350)/3) %810, -((leftHand[1]-600)/3), 50);
	
	//ofSetColor(0,255,0);
	ofSetColor((int)rightHand[0]%255,(int)rightHand[1]%255,(int)rightHand[2]%255);
	ofCircle((((int)rightHand[0] + 1350)/3) % 810, -((rightHand[1]-600)/3), 50);

	ofSetColor(0,0,0);
	//we move a line across the screen based on the time
	//the %400 makes the number stay in the 0-400 range. 
	int shiftX   = (ofGetElapsedTimeMillis() / 8 ) % 400;
	ofRect(shiftX, rgbaFbo.getHeight()-30, 810, 30);

}

//--------------------------------------------------------------
void testApp::draw(){

	ofDrawBitmapString("!! BEAT THE BEAT !!", 350, 50);
	ofDrawBitmapString("NEXT SONG", 650, 50);
	ofDrawBitmapString("PREVIOUS SONG", 50, 50);
	

	ofSetColor(255,255,255);  	
    rgbaFbo.draw(0,0);
}

