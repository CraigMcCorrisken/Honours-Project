#include "cone.h"

void Cone::init()
{
	mesh->loadMesh("../Resources/Models/cone.obj");
	makeHullContainer(*mesh->getVerts());
}

void Cone::VelocityVerletSolver(float dt)
{
	privMove(dt * velocity + 0.5f * (dt * dt) * acceleration);
	glm::vec3 velInBetween = velocity + 0.5f * dt * acceleration;
	velocity = velInBetween + 0.5f * acceleration;
}

void Cone::privMove(glm::vec3 translation)
{
	transform->Translate(translation);
	setHull(hull);
}

void Cone::changeTexture(GLuint newTexture)
{
	texture = newTexture;
}

void Cone::move(glm::vec3 dir)
{
	updateVelocity(dir);
	VelocityVerletSolver(speed);
}

void Cone::updateVelocity(glm::vec3 newVelocity)
{
	velocity = newVelocity;
}

glm::vec3 Cone::getVelocity()
{
	return velocity;
}

void Cone::setPosition(glm::vec3 newPosition)
{
	transform->Move(newPosition);
	setHull(hull);
}

void Cone::makeHullContainer(vector<float> points)
{	
	hull.resize(points.size() / 3);
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

void Cone::setHull(vector<glm::vec3> points)
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

glm::vec3 Cone::getPosition()
{
	return transform->getPosition();
}

vector<glm::vec3> Cone::getHull()
{
	return collidable->getConvexHull();
}

void Cone::update()
{

}

void Cone::draw(GLuint shader, std::stack<glm::mat4>* _mvStack, glm::mat4 projection)
{
	mesh->drawMesh(shader, _mvStack, projection, texture,
		transform);//->getPosition(), transform->getScale());
}