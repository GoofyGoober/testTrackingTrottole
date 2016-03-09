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
        void colorChanged(int & colorNew);
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
  
  // webcam gui
  ofParameter<int> exp;
  ofParameter<int> framerate;
  ofParameter<int> redBalance;
  ofParameter<int> greenBalance;
  ofParameter<int> blueBalance;
  ofParameter<int> brightness;
  ofParameter<int> gain;


  ofParameter<bool> bInvert;

    
  vector<ofxCv::ContourFinder> countoursFinders;
  vector< ofParameter<float> > thresholds;
  vector< ofParameter<float> > minAreas;
  vector< ofParameter<float> > maxAreas;
  vector< ofParameter<int> > colors;
  ofxCv::ContourFinder contourFinder;
  ofVideoGrabber grabber;
  ofxCv::TrackingColorMode trackingColorMode;
  ofColor targetColor;
  ofxPanel gui;
  ofParameterGroup group;
  ofParameterGroup groupWebcam;
    
  void setupGUI();
  cv::Mat   getROIImage();
  ofxOscSender sender;
  ofxOscBundle bundle;
    //ofParamete<float>
  
  
  ofParameter<float> ROIx;
  ofParameter<float> ROIy;
  ofParameter<float> ROIwidth;
  ofParameter<float> ROIheight;
  ofParameter<bool> drawWebCam;
  cv::Mat                   cam_mat;
  cv::Mat                   crop;
  ofColor getBlobColor(int x, int y);
};
