#include "Sphere.h"

void Sphere::init()
{
	//model loading
	rt3d::loadObj("../Resources/Models/sphere.obj", verts, norms, tex_coords, indices);
	meshIndexCount = indices.size();
	meshObjects = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(),
		meshIndexCount, indices.data());
	verts.clear(), norms.clear(), tex_coords.clear(), indices.clear();
}

glm::vec3 Sphere::getPosition()
{
	cout << position.x << endl;
	cout << position.y << endl;
	cout << position.z << endl;
	return position;
}

vector<glm::vec3> Sphere::getHull()
{
	return cHull;
}

void Sphere::setHull(vector<glm::vec3> hull)
{
	cHull = hull;
}

void Sphere::update()
{
	rotator += 0.01f;
}

void Sphere::draw(GLuint shader, std::stack<glm::mat4>* _mvStack, glm::mat4 projection, GLuint texture, glm::vec3 pos)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	_mvStack->push(_mvStack->top());
	_mvStack->top() = glm::translate(_mvStack->top(), pos);
	_mvStack->top() = glm::rotate(_mvStack->top(), float(rotator), glm::vec3(0.0f, 1.0f, 0.0f));
	_mvStack->top() = glm::scale(_mvStack->top(), scale);
	rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(projection));
	rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(_mvStack->top()));
	rt3d::setMaterial(shader, material0);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	_mvStack->pop();
}