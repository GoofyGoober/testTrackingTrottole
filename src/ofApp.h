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
		void mousePressed(int x, int y, int button);

        void gainChanged(int & circleResolution);
        void colorChanged(ofColor & colorNew);
        void setupGrabber();
        void searchBlobs(ofxCv::ContourFinder &_finder, int i);
        void setupFinder(ofxCv::ContourFinder &_finder, int a);
        void setupWebcamGui(ofParameterGroup &_group);
        void setupMaskGui(ofParameterGroup &_group);
        void getCenterAndSendOsc(ofxCv::ContourFinder _finder, int _numCentroid, int _channel);

  ofxOscMessage msgOsc(int x, int y, int index, bool accesoSpento);
  void  setupContourFinder();
  ofParameter<float> threshold;
  ofParameter<float> minArea;
  ofParameter<float> maxArea;
    
    int NUM_OF_FINDERS = 6;
    int CANALE_TARGET = 0;
  
  // webcam gui
  ofParameter<int> exp;
  ofParameter<int> framerate;
  ofParameter<int> redBalance;
  ofParameter<int> greenBalance;
  ofParameter<int> blueBalance;
  ofParameter<int> brightness;
  ofParameter<int> gain;
  ofParameter<int> contrast;


  ofParameter<bool> bInvert;
  ofParameter<bool>PICK_COLOR;

    
  vector<ofxCv::ContourFinder> countoursFinders;
  vector< ofParameter<int> > thresholds;
  vector< ofParameter<int> > minAreas;
  vector< ofParameter<int> > maxAreas;
  vector< ofParameter<ofColor> > colors;
  ofxCv::ContourFinder contourFinder;
  ofVideoGrabber grabber;
  ofxCv::TrackingColorMode trackingColorMode;
  ofColor targetColor;
  ofxPanel gui;
  ofxPanel guiPerTrottole;
    void keyPressed(int key);
  ofParameterGroup group;
  ofParameterGroup groupWebcam;
    
  void setupGUI();
  cv::Mat   getROIImage();
  ofxOscSender sender;
      ofxOscSender sender2;
  ofxOscBundle bundle;
    //ofParamete<float>
  
  
  ofParameter<int> ROIx;
  ofParameter<int> ROIy;
  ofParameter<int> ROIwidth;
  ofParameter<int> ROIheight;
  ofParameter<bool> drawWebCam;
  cv::Mat                   cam_mat;
  cv::Mat                   crop;
  ofColor getBlobColor(int x, int y);
};
