#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
  grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());

  grabber.setup(640, 480);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setAutogain(false);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(false);
        setupGrabber();
  ofSetFrameRate(20);
  setupContourFinder();
    
    
    
  setupGUI();
  sender.setup("127.0.0.1", 12345);
}

void ofApp::setupGrabber(){
    grabber.getGrabber<ofxPS3EyeGrabber>()->setExposure(exp);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(framerate);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setRedBalance(redBalance);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setBlueBalance(blueBalance);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setGreenBalance(greenBalance);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setBrightness(brightness);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setGain(gain);
}

void ofApp::setupGUI()
{
  gui.setup();
  group.setName("Capture");
    
    setupMaskGui(group);
    gui.add(group);
for(int a = 0; a < countoursFinders.size(); a++){
    ofParameterGroup group_contour;
    group_contour.setName(ofToString(a));
    group_contour.add(thresholds[a].set("threshold", 50,   0, 255));
    group_contour.add(minAreas[a].set(  "Min area", 50,   0, 255));
    group_contour.add(maxAreas[a].set(  "Max area", 50,   0, 255));
    group_contour.add(colors[a].set(    "Colors", a*10, 0, 255));
    colors[a].addListener(this,&ofApp::colorChanged);
    gui.add(group_contour);
  }
    
  setupWebcamGui(groupWebcam);
  gui.add(groupWebcam);

  gui.loadFromFile("settings.xml");
}



void ofApp::setupMaskGui(ofParameterGroup &_group){
    _group.add(ROIx.set("ROI x", 0, 0, 640));
    _group.add(ROIy.set("ROI y", 0, 0, 480));
    _group.add(ROIwidth.set("ROI width", 100, 10, 640));
    _group.add(ROIheight.set("ROI height", 100, 10, 480));
    _group.add(drawWebCam.set("Draw webcam", true));
    _group.add(bInvert.set("Invert", true));
}

void ofApp::setupWebcamGui(ofParameterGroup &_group){
    // webcam gui
    _group.setName("Webcam");
    _group.add(exp.set("exposure", 58,0,255));
    _group.add(framerate.set("framerate", 20,0,60));
    _group.add(redBalance.set("red", 102,0,255));
    _group.add(greenBalance.set("green", 0,0,255));
    _group.add(blueBalance.set("blue", 123,0,255));
    _group.add(brightness.set("bright", 0,0,255));
    _group.add(gain.set("gain", 0,0,60));
    
    exp.addListener(this,&ofApp::gainChanged);
    framerate.addListener(this,&ofApp::gainChanged);
    redBalance.addListener(this,&ofApp::gainChanged);
    greenBalance.addListener(this,&ofApp::gainChanged);
    blueBalance.addListener(this,&ofApp::gainChanged);
    brightness.addListener(this,&ofApp::gainChanged);
    gain.addListener(this,&ofApp::gainChanged);
}

void ofApp::colorChanged(int & colorNew){
    
}

void ofApp::gainChanged(int & circleResolution){
    setupGrabber();
}

void ofApp::setupContourFinder(){
    for(int a = 0 ; a <= NUM_OF_FINDERS; a++)
    {
        ofxCv::ContourFinder temp;
        countoursFinders.push_back(temp);
        thresholds.push_back(128);
        minAreas.push_back(128);
        maxAreas.push_back(128);
        colors.push_back(1);
        
    }
  contourFinder.setMinAreaRadius(1);
  contourFinder.setMaxAreaRadius(200);
  trackingColorMode = TRACK_COLOR_RGB;
  // TRACK_COLOR_RGB, TRACK_COLOR_HSV, TRACK_COLOR_H, TRACK_COLOR_HS
}

//--------------------------------------------------------------
void ofApp::update(){
  grabber.update();
    
  if(grabber.isFrameNew()) {
      for(int channel = 0; channel < countoursFinders.size(); channel++){
        setupFinder(countoursFinders[channel], channel);
        searchBlobs(countoursFinders[channel], channel);
      }
  }
}

void ofApp::setupFinder(ofxCv::ContourFinder &_finder, int a){
    _finder.setMaxArea(maxAreas[a]);
    _finder.setMinArea(minAreas[a]);
    _finder.setThreshold(thresholds[a]);
    _finder.setSimplify(true);
    _finder.setInvert(bInvert);
    _finder.findContours(getROIImage());
}

void ofApp::searchBlobs(ofxCv::ContourFinder &_finder, int _channel){
    int totBlob = _finder.getContours().size();
    for(int i = 0; i < totBlob; i++)
    {
        getCenterAndSendOsc(_finder, i, _channel);
    }
}

void ofApp::getCenterAndSendOsc(ofxCv::ContourFinder _finder, int _numCentroid, int _channel){
    float xCenter = _finder.getCentroid(_numCentroid).x/ROIwidth;
    float yCenter = _finder.getCentroid(_numCentroid).y/ROIwidth;
    float x = xCenter * 100;
    float y = yCenter * 100;
    ofColor yellow = ofColor(0,0,200);
    ofColor found = getBlobColor(xCenter, yCenter);
    ofColor difference = found - yellow;
//    cout << "difference: " << difference << endl;
    ofxOscMessage m = msgOsc(x,y,_channel,true);
    sender.sendMessage(m);
}


//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255);
  if(drawWebCam)
    grabber.draw(0,0);

  ofPushStyle();
  ofNoFill();
  ofPushMatrix();
  ofTranslate(ROIx, ROIy);
  ofDrawRectangle(0, 0, ROIwidth, ROIheight);
    for(int a = 0; a < countoursFinders.size(); a++){
        ofColor color;
        color.setHsb(colors[a], 255, 255);
        
        ofSetColor(color);
        countoursFinders[a].draw();
    }
  
  ofPopMatrix();
  ofPopStyle();
  ofFill();
  ofSetColor(0);
  ofDrawRectangle(-3, -3, 64+6, 64+6);
  ofSetColor(targetColor);
  ofDrawRectangle(0, 0, 64, 64);
  gui.draw();
}


cv::Mat ofApp::getROIImage()
{
  cam_mat   = toCv(grabber);
  ROIx      = ofClamp(ROIx, 1, 640 - 200);
  ROIy      = ofClamp(ROIy, 1, 400);
  ROIwidth  = ofClamp(ROIwidth, 1, 640 - 50 - ROIx -1);
  ROIheight = ofClamp(ROIheight, 1, 480 - ROIy - 1);
  cv::Rect crop_roi = cv::Rect(ROIx, ROIy, ROIwidth, ROIheight);
  crop      = cam_mat(crop_roi).clone();
  return crop;
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  targetColor = grabber.getPixels().getColor(x, y);
  for(int a = 0; a < countoursFinders.size(); a++){
      countoursFinders[a].setTargetColor(targetColor, trackingColorMode);
  }
}

ofColor ofApp::getBlobColor(int x, int y)
{
  return grabber.getPixels().getColor(ROIx + x, ROIy + y);
}

ofxOscMessage ofApp::msgOsc(int x, int y, int channel, bool accesoSpento){
  ofxOscMessage m;
  m.setAddress("/blob_"+ofToString(channel));
  m.addInt32Arg(x);
  m.addInt32Arg(y);
    if (accesoSpento){
        m.addInt32Arg(1);
    } else {
        m.addInt32Arg(0);
    }
  return m;
}