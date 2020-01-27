#pragma once
#include "Entity.h"

class Shape : Entity
{
protected:
	glm::vec3 position = glm::vec3(1,1,1);
	GLuint texture;
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	GLuint meshIndexCount;
	GLuint meshObjects;
	float rotator = 0.0f;

	rt3d::materialStruct material0 = {
	{0.2f, 0.4f, 0.2f, 1.0f}, // ambient
	{0.5f, 1.0f, 0.5f, 1.0f}, // diffuse
	{0.0f, 0.1f, 0.0f, 1.0f}, // specular
					   2.0f  // shininess
	};

	glm::vec3 scale = glm::vec3(1.5f, 1.5f, 1.5f);

	vector<glm::vec3> cHull;

public:
	virtual glm::vec3 getPosition() = 0;
	virtual vector<glm::vec3> getHull() = 0;
	virtual void setHull(vector<glm::vec3>) = 0;
};