//################################################################
//
// A 3D cylinder for collision testing purposes
//
//################################################################

#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
private:
	void privMove(glm::vec3 translation);
	void VelocityVerletSolver(float dt);

	vector<glm::vec3> hull;
public:
	Cylinder(glm::vec3 cylinderScale, glm::vec3 pos, GLuint texture) {
		transform->Scale(cylinderScale);
		privMove(pos);
		this->texture = texture;
	}

	void init();
	void update();
	void draw(GLuint shader, std::stack<glm::mat4>* _mvStack, 
		glm::mat4 projection);
	glm::vec3 getPosition();
	void setPosition(glm::vec3 newPosition);
	vector<glm::vec3> getHull();
	void setHull(vector<glm::vec3>);
	
	void changeTexture(GLuint);
	void move(glm::vec3 dir);
	void updateVelocity(glm::vec3);
	glm::vec3 getVelocity();
	void makeHullContainer(vector<float> points);
};