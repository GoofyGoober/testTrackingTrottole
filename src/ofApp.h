#pragma once

#include "ofMain.h"
#include "ofxPS3EyeGrabber.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{

	public:
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
  ofxOscMessage msgOsc(int x, int y, int index);
  void  setupContourFinder();
  ofParameter<float> threshold;
  ofParameter<float> minArea;
  ofParameter<float> maxArea;
  ofParameter<bool> bInvert;
    ofxCv::ContourFinder contourFinder;
    ofVideoGrabber grabber;
  ofxCv::TrackingColorMode trackingColorMode;
  ofColor targetColor;
  ofxPanel gui;
  ofParameterGroup group;
  void setupGUI();
  cv::Mat   getROIImage();
  ofxOscSender sender;
  ofxOscBundle bundle;
//  ofParamete<float>
  
  
  ofParameter<float> ROIx;
  ofParameter<float> ROIy;
  ofParameter<float> ROIwidth;
  ofParameter<float> ROIheight;
  ofParameter<bool> drawWebCam;
  cv::Mat                   cam_mat;
  cv::Mat                   crop;
};
