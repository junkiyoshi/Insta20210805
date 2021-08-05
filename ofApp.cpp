#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(0.75);
	ofEnableDepthTest();

	this->ico_sphere = ofIcoSpherePrimitive(200, 2);
	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();

	int radius_start = 0;
	int radius_end = 250;
	for (auto& triangle : this->ico_sphere.getMesh().getUniqueFaces()) {

		auto noise_seed_x = ofRandom(1000);
		auto noise_seed_y = ofRandom(1000);
		auto noise_seed_z = ofRandom(1000);

		auto noise_value = ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005);
		this->frame.addVertex(glm::vec3());
		for (int radius = radius_start; radius <= radius_end; radius += 1) {

			auto mesh_index = this->mesh.getNumVertices();
			auto frame_index = this->frame.getNumVertices();

			auto param = ofMap(radius, radius_start, radius_end, 0, PI * 0.5);

			auto angle_x = ofMap(ofNoise(noise_seed_x, radius * 0.001 + ofGetFrameNum() * 0.0005), 0, 1, -param, param);
			auto rotation_x = glm::rotate(glm::mat4(), angle_x, glm::vec3(1, 0, 0));
			auto angle_y = ofMap(ofNoise(noise_seed_y, radius * 0.001 + ofGetFrameNum() * 0.0005), 0, 1, -param, param);
			auto rotation_y = glm::rotate(glm::mat4(), angle_y, glm::vec3(0, 1, 0));
			auto angle_z = ofMap(ofNoise(noise_seed_z, radius * 0.001 + ofGetFrameNum() * 0.0005), 0, 1, -param, param);
			auto rotation_z = glm::rotate(glm::mat4(), angle_z, glm::vec3(0, 0, 1));

			glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
			glm::vec3 loc = glm::normalize(avg) * radius;

			if (radius == radius_end) {

				this->mesh.addVertex(glm::vec4(loc + (triangle.getVertex(0) - avg) * 0.2, 0) * rotation_z * rotation_y * rotation_x);
				this->mesh.addVertex(glm::vec4(loc + (triangle.getVertex(1) - avg) * 0.2, 0) * rotation_z * rotation_y * rotation_x);
				this->mesh.addVertex(glm::vec4(loc + (triangle.getVertex(2) - avg) * 0.2, 0) * rotation_z * rotation_y * rotation_x);

				this->mesh.addIndex(mesh_index + 0); this->mesh.addIndex(mesh_index + 1); this->mesh.addIndex(mesh_index + 2);
			}

			this->frame.addVertex(glm::vec4(loc, 0) * rotation_z * rotation_y * rotation_x);
			this->frame.addIndex(frame_index + 0); this->frame.addIndex(frame_index - 1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.4);
	ofRotateX(ofGetFrameNum() * 0.25);

	ofSetColor(0);
	this->mesh.drawWireframe();

	ofSetColor(255);
	this->mesh.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}