#include "Collidable.h"

//update this to take the points passed in
//and find the hull of those points
//instead of just storing the points as is
void Collidable::setConvexHull(std::vector<glm::vec3> points)
{
	convexHull = points;
}

std::vector<glm::vec3> Collidable::getConvexHull()
{
	return convexHull;
}