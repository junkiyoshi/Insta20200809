#pragma once
#include "ofMain.h"
#include "opencv2/opencv.hpp"

/* Ripple */

class Ripple
{
public:

	Ripple();
	Ripple(glm::vec3 location);
	void update();
	void draw();
	bool isDead();
private:

	glm::vec3 location;
	float radius;
	float life;
	ofColor color;
};

/* ofApp */

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	vector<unique_ptr<Ripple>> ripple_list;

	ofFbo fbo;
	ofPixels pixels;
	cv::Mat pixels_mat;
};