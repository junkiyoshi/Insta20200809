#include "ofApp.h"

/* Ripple */

//--------------------------------------------------------------
Ripple::Ripple() : Ripple(glm::vec3()) { }

//--------------------------------------------------------------
Ripple::Ripple(glm::vec3 location) {

	this->location = location;
	this->radius = 1;
	this->life = 255;
	this->color.setHsb(ofRandom(255), 255, 230);
}

//--------------------------------------------------------------
void Ripple::update() {

	if (this->location.z > 0) {

		this->location.z = this->location.z > 20 ? this->location.z - 20 : 0;
		return;
	}

	this->radius += 2;
	this->life -= 3;
}

//--------------------------------------------------------------
void Ripple::draw() {

	ofPushMatrix();
	ofTranslate(this->location);

	ofSetColor(this->color);
	if (this->location.z > 0) {

		ofDrawLine(glm::vec3(), glm::vec3(0, 0, 30));
	}
	else {

		if (this->life < 200) {

			ofSetColor(this->color, ofMap(this->life, 0, 200, 0, 255));
		}

		int deg_span = 60;
		for (int deg = 0; deg < 360; deg += deg_span) {

			int next_deg = deg + deg_span;
			float noise_param = ofMap(ofNoise(glm::vec3(
				this->location.x + cos(deg * DEG_TO_RAD),
				this->location.y + sin(deg * DEG_TO_RAD),
				ofGetFrameNum() * 0.008)),
				0, 1, 0, 2);
			float next_noise_param = ofMap(ofNoise(glm::vec3(
				this->location.x + cos((deg + deg_span) * DEG_TO_RAD),
				this->location.y + sin((deg + deg_span) * DEG_TO_RAD),
				ofGetFrameNum() * 0.008)),
				0, 1, 2, 0);

			vector<glm::vec2> vertices;
			vertices.push_back(glm::vec2(
				radius * cos(deg * DEG_TO_RAD),
				radius * sin(deg * DEG_TO_RAD)));
			vertices.push_back(glm::vec2(
				(radius * noise_param) * cos((deg + deg_span * 0.25) * DEG_TO_RAD),
				(radius * noise_param) * sin((deg + deg_span * 0.25) * DEG_TO_RAD)));
			vertices.push_back(glm::vec2(
				(radius * next_noise_param) * cos((next_deg - deg_span * 0.25) * DEG_TO_RAD),
				(radius * next_noise_param) * sin((next_deg - deg_span * 0.25) * DEG_TO_RAD)));
			vertices.push_back(glm::vec2(
				radius * cos(next_deg * DEG_TO_RAD),
				radius * sin(next_deg * DEG_TO_RAD)));

			ofNoFill();
			ofSetLineWidth(2);
			ofBeginShape();
			ofVertex(vertices[0]);
			ofBezierVertex(vertices[1], vertices[2], vertices[3]);
			ofEndShape();
		}
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
bool Ripple::isDead() {

	return this->life < 0;
}

/* ofApp */

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(3);

	this->fbo.allocate(ofGetWidth() + 25, ofGetHeight() + 25);
	this->fbo.readToPixels(this->pixels);
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 4 == 0) {

		glm::vec3 location(ofRandom(100, this->fbo.getWidth() - 100), ofRandom(100, this->fbo.getHeight() - 100), ofRandom(800, 1500));
		unique_ptr<Ripple> ripple(new Ripple(location));
		this->ripple_list.push_back(move(ripple));
	}

	this->fbo.begin();
	ofClear(0);

	for (int i = this->ripple_list.size() - 1; i >= 0; i--) {

		if (this->ripple_list[i]->isDead()) {

			this->ripple_list.erase(this->ripple_list.begin() + i);
		}
		else {

			this->ripple_list[i]->update();
			this->ripple_list[i]->draw();
		}
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(11, 11), 10, 10);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);

	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(-5, -5);

	this->fbo.draw(-25, -25);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}