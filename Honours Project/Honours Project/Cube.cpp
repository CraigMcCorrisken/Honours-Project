#include "Cube.h"

void Cube::init()
{
	mesh->loadMesh("../Resources/Models/cube.obj");
	makeHullContainer(*mesh->getVerts());
}

void Cube::VelocityVerletSolver(float dt)
{
	privMove(dt * velocity + 0.5f * (dt * dt) * acceleration);
	glm::vec3 velInBetween = velocity + 0.5f * dt * acceleration;
	velocity = velInBetween + 0.5f * acceleration;
}

void Cube::privMove(glm::vec3 translation)
{
	transform->Translate(translation);
	setHull(hull);
}

void Cube::changeTexture(GLuint newTexture)
{
	texture = newTexture;
}

void Cube::move(glm::vec3 dir)
{
	updateVelocity(dir);
	VelocityVerletSolver(speed);
}

void Cube::updateVelocity(glm::vec3 newVelocity)
{
	velocity = newVelocity;
}

glm::vec3 Cube::getVelocity()
{
	return velocity;
}

void Cube::makeHullContainer(vector<float> points)
{
	hull.resize(8);
	int counter = 0;

	for (int i = 0; i < hull.size(); ++i)
	{
		hull[i].x = points[counter];
		hull[i].y = points[counter + 1];
		hull[i].z = points[counter + 2];
		counter += 3;
	}
	setHull(hull);
}

void Cube::setHull(vector<glm::vec3> points)
{
	for (int i = 0; i < points.size(); ++i)
	{
		glm::vec4 v = glm::vec4(points[i].x, points[i].y, points[i].z, 1.0);
		glm::mat4 m = *(transform->getModelMatrix());
		v = m * v;
		points[i] = glm::vec3(v.x, v.y, v.z);
	}
	collidable->setConvexHull(points);
}

glm::vec3 Cube::getPosition()
{
	return transform->getPosition();
}

void Cube::setPosition(glm::vec3 newPosition)
{
	transform->Move(newPosition);
	setHull(hull);
}

vector<glm::vec3> Cube::getHull()
{
	return collidable->getConvexHull();
}

void Cube::update()
{ 

}

void Cube::draw(GLuint shader,
	std::stack<glm::mat4>* _mvStack, glm::mat4 projection)
{
	mesh->drawMesh(shader, _mvStack, projection, texture,
		transform);
}