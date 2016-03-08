#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
//  grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
  grabber.setup(640, 480);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setAutogain(false);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(false);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setBrightness(0);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setGain(0);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setExposure(58);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(20);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setRedBalance(102);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setBlueBalance(123);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setGreenBalance(0);
  ofSetFrameRate(20);
  setupContourFinder();
  setupGUI();
  sender.setup("127.0.0.1", 12345);
}

void ofApp::setupGUI()
{
  gui.setup();
  group.setName("Gruppo");
  group.add(threshold.set("Threshold", 50, 0, 255));
  group.add(minArea.set("Min area", 50, 0, 640*480));
  group.add(maxArea.set("Max area", 2000, 0, 640*480));
  group.add(bInvert.set("Invert", false));
  group.add(ROIx.set("ROI x", 0, 0, 640));
  group.add(ROIy.set("ROI y", 0, 0, 480));
  group.add(ROIwidth.set("ROI width", 100, 10, 640));
  group.add(ROIheight.set("ROI heighy", 100, 10, 480));
  group.add(drawWebCam.set("Draw webcam", true));
  gui.add(group);
  gui.loadFromFile("settings.xml");
}

void ofApp::setupContourFinder()
{
  contourFinder.setMinAreaRadius(1);
  contourFinder.setMaxAreaRadius(200);
  trackingColorMode = TRACK_COLOR_RGB;
  // TRACK_COLOR_RGB, TRACK_COLOR_HSV, TRACK_COLOR_H, TRACK_COLOR_HS
}

//--------------------------------------------------------------
void ofApp::update(){
  grabber.update();
  if(grabber.isFrameNew()) {
    
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setBrightness(0);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setGain(0);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setExposure(58);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(20);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setRedBalance(102);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setBlueBalance(123);
//  grabber.getGrabber<ofxPS3EyeGrabber>()->setGreenBalance(0);
    
    contourFinder.setMaxArea(maxArea);
    contourFinder.setMinArea(minArea);
    contourFinder.setThreshold(threshold);
    contourFinder.setInvert(bInvert);
    contourFinder.findContours(getROIImage());
    
    
    int totBlob = contourFinder.getContours().size();
    for(int a = 0; a < totBlob; a++)
    {
      float x = (contourFinder.getCentroid(a).x/ROIwidth) * 100;
      float y = (contourFinder.getCentroid(a).y/ROIheight) * 100;
      ofColor yellow = ofColor(0,0,200);
      ofColor found = getBlobColor(contourFinder.getCentroid(a).x, contourFinder.getCentroid(a).y);
      ofColor difference = found - yellow;
      cout << "difference " << difference << endl;
//      bundle.addMessage(msgOsc(x,y,a));
      ofxOscMessage m = msgOsc(x,y,a);
      sender.sendMessage(m);
    }
    
  }
}

int ofApp::getBlobColorIndex(ofColor color)
{
  return 1;
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255);
  if(drawWebCam)
    grabber.draw(0,0);
  drawHighlightString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
  drawHighlightString(ofToString((int) threshold) + " threshold", 10, 40);
  drawHighlightString(trackingColorMode == TRACK_COLOR_HSV ? "HSV tracking" : "RGB tracking", 10, 60);
  
  ofPushStyle();
  ofNoFill();
  ofPushMatrix();
  ofTranslate(ROIx, ROIy);
  ofDrawRectangle(0, 0, ROIwidth, ROIheight);
  contourFinder.draw();
  
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
  cam_mat = toCv(grabber);
  ROIx = ofClamp(ROIx, 1, 640 - 200);
  ROIy = ofClamp(ROIy, 1, 400);
  ROIwidth = ofClamp(ROIwidth, 1, 640 - 50 -ROIx-1);
  ROIheight = ofClamp(ROIheight, 1, 480 - ROIy - 1);
  cv::Rect crop_roi = cv::Rect(ROIx, ROIy, ROIwidth, ROIheight);
  crop = cam_mat(crop_roi).clone();
  return crop;
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
void ofApp::mousePressed(int x, int y, int button) {
  targetColor = grabber.getPixels().getColor(x, y);
  contourFinder.setTargetColor(targetColor, trackingColorMode);
}

void ofApp::keyPressed(int key) {
  if(key == 'h') {
    trackingColorMode = TRACK_COLOR_HSV;
  }
  if(key == 'r') {
    trackingColorMode = TRACK_COLOR_RGB;
  }
  contourFinder.setTargetColor(targetColor, trackingColorMode);
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
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

ofColor ofApp::getBlobColor(int x, int y)
{
  return grabber.getPixels().getColor(ROIx + x, ROIy + y);
}

ofxOscMessage ofApp::msgOsc(int x, int y, int index){
  ofxOscMessage m;
  m.setAddress("/blob_"+ofToString(index));
  m.addInt32Arg(x);
  m.addInt32Arg(y);
  ofLog() << "*******";
  cout << x << endl;
  cout << y << endl;
  m.addInt32Arg(index);
  return m;
}