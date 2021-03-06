 #include "GJK.h"

bool GJK::performDetection(std::vector<glm::vec3>& hull1, std::vector<glm::vec3>& hull2)
{
	//t1 = chrono::high_resolution_clock::now();
	initialise(hull1, hull2);

	steps = 0;
	while (steps < 50) {
		simplex[0] = support->support(direction, hull1, hull2);	//Adding third point to the simplex
		if (dot(simplex[0], direction) < 0) //is this new point further than the origin?
			return false;
		else 
		{
			if (ContainsOrigin(direction))
			{
				//t2 = chrono::high_resolution_clock::now();
				//auto time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
				//cout << time << ' ' << "microseconds.\n"; // << " \n"; 

				return true;
			}
		}
		steps++;
	}
	return false;
}

bool GJK::initialise(std::vector<glm::vec3>& hull1, std::vector<glm::vec3>& hull2)
{
	simplex[2] = support->support(direction, hull1, hull2);	//Adding first point to the simplex
	direction = -simplex[2];								//opposite direction
	simplex[1] = support->support(direction, hull1, hull2);	//Adding second point to the simplex

	//is this new point further than the origin?
	if (dot(simplex[1], direction) < 0)
		return false;

	//updating search direction
	glm::vec3 bc = simplex[2] - simplex[1];
	glm::vec3 ao = -simplex[1];
	direction = glm::cross(glm::cross(bc, ao), bc);

	simplexSize = 2; //begin with a line
}

bool GJK::ContainsOrigin(glm::vec3& direction)
{
	if (simplexSize == 2)
		return triangle(direction);

	else if (simplexSize == 3)
		return tetrahedron(direction);

	return false;
}

//check which edge/face of this triangle is closest to the origin
bool GJK::triangle(glm::vec3& direction)
{
	glm::vec3 ao = -simplex[0];					//Line AO
	glm::vec3 ab = simplex[1] - simplex[0];		//Line AB
	glm::vec3 ac = simplex[2] - simplex[0];		//Line AC
	glm::vec3 abc = glm::cross(ab, ac);		//Face ABC

	//origin can't be behind points B,C or line BC 

	glm::vec3 ab_abc = glm::cross(ab, abc);	//Line AB on face ABC
	
	//is the origin away from AB?
	if (dot(ab_abc, ao) > 0) {
		simplex[2] = simplex[1];
		simplex[1] = simplex[0];

		//not facing origin, find new direction
		direction = glm::cross(glm::cross(ab, ao), ab);

		//can't build tetrahedron
		return false;
	}

	glm::vec3 abc_ac = glm::cross(abc, ac);	//Line AC on face ABC

	// is the origin away from AC? 
	if (dot(abc_ac, ao) > 0) {
		simplex[1] = simplex[0];

		direction = glm::cross(glm::cross(ac, ao), ac);

		//direction change; can't build tetrahedron
		return false;
	}

	//above or below face ABC
	if (dot(abc, ao) > 0) {
		//base of tetrahedron
		simplex[3] = simplex[2];
		simplex[2] = simplex[1];
		simplex[1] = simplex[0];

		//new direction
		direction = abc;
	}
	else {
		//upside down tetrahedron
		simplex[3] = simplex[1];
		simplex[1] = simplex[0];
		direction = -abc;
	}

	simplexSize = 3;
	return false;
}

//does the current tetrahedron contain the origin
//or do we have to keep checking?
bool GJK::tetrahedron(glm::vec3& direction) {
	glm::vec3 ao = -simplex[0];					//Line AO
	glm::vec3 ab = simplex[1] - simplex[0];		//Line AB
	glm::vec3 ac = simplex[2] - simplex[0];		//Line AC
	glm::vec3 abc = glm::cross(ab, ac);		//FACE ABC

	//CASE 1
	//in front of face ABC
	if (dot(abc, ao) > 0)
		checkTetrahedron(ao, ab, ac, abc, direction);

	//CASE 2
	glm::vec3 ad = simplex[3] - simplex[0];		//Line AD
	glm::vec3 acd = glm::cross(ac, ad); 		//Face ACD
	
	//in front of face ACD
	if (dot(acd,ao) > 0) {
		simplex[1] = simplex[2];
		simplex[2] = simplex[3];
		ab = ac;
		ac = ad;
		abc = acd;

		checkTetrahedron(ao, ab, ac, abc, direction);
	}

	//CASE 3
	glm::vec3 adb = glm::cross(ad, ab); //ADB triangle

	//in front of face ADB
	if (dot(adb,ao) > 0) {
		simplex[2] = simplex[1];
		simplex[1] = simplex[3];
		ac = ab;
		ab = ad;
		abc = adb;

		checkTetrahedron(ao, ab, ac, abc, direction);
	}
	//origin within tetrahedron already built
	return true;
}

//tetrahedon doesnt contain origin
//therefor, must check which face on it is closest to the origin
//and set the search direction to this direction
bool GJK::checkTetrahedron(const glm::vec3& ao, const glm::vec3& ab,
	const glm::vec3& ac, const glm::vec3& abc, glm::vec3& direction)
{
	glm::vec3 ab_abc = glm::cross(ab, abc);		//Line AB on face ABC

	if (dot(ab_abc,ao) > 0)
	{
		simplex[2] = simplex[1];
		simplex[1] = simplex[0];

		//direction is not ab_abc because it does not point towards the origin
		direction = glm::cross(glm::cross(ab, ao), ab);

		//start again from triangle checks
		simplexSize = 2;
		return false;
	}

	glm::vec3 acp = glm::cross(abc, ac);		//Face 

	if (dot(acp,ao) > 0) {
		simplex[1] = simplex[0];

		//direction is not abc_ac because it's not point towards the origin
		direction = glm::cross(glm::cross(ac, ao), ac);

		//start again from triangle checks
		simplexSize = 2;
		return false;
	}

	//build new tetrahedron with new base
	simplex[3] = simplex[2];
	simplex[2] = simplex[1];
	simplex[1] = simplex[0];

	direction = abc;
	simplexSize = 3;
	return false;
}