 #include "GJK.h"

glm::vec3 GJK::tripleProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) {

    glm::vec3 r;

    float ac = a.x * c.x + a.y * c.y + a.z * c.z; // perform a.dot(c)
    float bc = b.x * c.x + b.y * c.y + b.z * c.z; // perform b.dot(c)

    // perform b * a.dot(c) - a * b.dot(c)
    r.x = b.x * ac - a.x * bc;
    r.y = b.y * ac - a.y * bc;
    r.z = b.z * ac - a.z * bc;
    return r;
}

//CODE BASED ON ->>> http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
glm::vec3 cross(const glm::vec3& v1, const glm::vec3& v2)
{
    return glm::vec3(v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z, 
        v1.x * v2.y - v1.y * v2.x);
}

glm::vec3 doubleCross(const glm::vec3& v1, const glm::vec3& v2)
{
    return cross(cross(v1, v2), v1);
}

bool GJK::performGJK(/*vector<glm::vec3> hull1, vector<glm::vec3> hull2*/)
{

    //!! FOR TEST PURPOSES !!
    // Final edition = shapes will create their own convex hulls for their data
    // Shapes hulls will be passed in from the game class

    // create a set of points in 3D space
    // pass them into the convex hull class which calculates the hull of the points
    // convex hull is passed back the game as a set of points stored in a vector
    // points are written to console window for testing purposes

    ////CONVEX HULL CALCULATOR
    //int size = points.size();
    //vector<glm::vec3> hull1;
    //vector<glm::vec3> hull2;

    //if (size >= 4) {	//make sure there are enough points for a 3D shape
    //    hull1 = cHull->createHull(points);
    //    hull2 = cHull->createHull(points2);
    //}
    //else
    //    cout << "not enought vertices for 3D shape, minimum required: 4.\nCurrent vertex count"
    //    << size << endl;

    //cout << "Points on first hull: " << endl;
    //for (int i = 0; i <= hull1.size() - 1; i++)
    //    cout << "(" << hull1[i].x << ", " << hull1[i].y << ", " << hull1[i].z << ")\n";

    //cout << "Points on second hull: " << endl;
    //for (int i = 0; i <= hull2.size() - 1; i++)
    //    cout << "(" << hull2[i].x << ", " << hull2[i].y << ", " << hull2[i].z << ")\n";







    //direction = glm::vec3(0, 1, 0);

    //unsigned int furthestIndex = support->furthestPoint(direction, hull1);
    //cout << "\n\n" << endl;
    //cout << "furthest point: " << furthestIndex << endl;

    //glm::vec3 furthestVertice = points[furthestIndex];
    //cout << "x: " << furthestVertice.x << endl;
    //cout << "y: " << furthestVertice.y << endl;
    //cout << "z: " << furthestVertice.z << endl;

    ////direction  two point clouds  
    //glm::vec3 furthestPoint = support->support(direction, hull1, hull2);
    //cout << "\n\n" << endl;
    //cout << "x: " << furthestPoint.x << endl;
    //cout << "y: " << furthestPoint.y << endl;
    //cout << "z: " << furthestPoint.z << endl;






    ////TESTING THE GJK
    //cout << "TESTING THE GJK" << endl;

    //unsigned int index = 0; // index of current vertex of the simplex
    //glm::vec3 a, b, c, ao, ab, ac, abperp, acperp, simplex[3];

    //// if initial direction is zero then set to any arbitrary axis
    //if ((direction.x == 0) && (direction.y == 0))
    //    direction.x = 1.f;

    //// set the first support as initial point of the new simplex
    //a = simplex[0] = support->support(direction, hull1, hull2);

    //if (dot(a, direction) <= 0) //no collision
    //    return false;

    //direction = -a; //check the opposite direction

	// TEST CODE ONLY!!
	glm::vec3 dir = glm::vec3(1, 1, 1);

	c = support->support(dir, points, points2);

	dir = -c;//negative direction

	b = support->support(dir, points, points2);

	if (dot(b, dir) < 0)
	{
		return false;
	}
	dir = doubleCross(c - b, -b);

	nrPointsSimplex = 2; //begin with 2 points in simplex

	int steps = 0;//avoid infinite loop
	while (steps < 50)
	{
		a = support->support(dir, points, points2);
		if (dot(a,dir) < 0)
			return false;
		else
		{
			if (ContainsOrigin(dir))
			{
				return true;
			}
		}
		steps++;
	}
	return false;
}


// EVERYTHING BELOW HERE IS FOR TESTING PURPOSES ONLY!!! 
// CODE TAKEN FROM ->>> http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
// NONE OF THE BELOW IS MY WORK AND SHOULD NOT APPEAR IN FINAL CODE WITHOUT 
// PERSONALISED CHANGES AND EDITIONS




bool GJK::ContainsOrigin(glm::vec3& dir)
{
	if (nrPointsSimplex == 1)
		return line(dir);

	if (nrPointsSimplex == 2)
		return triangle(dir);

	else if (nrPointsSimplex == 3)
		return tetrahedron(dir);

	return false;
}

bool GJK::line(glm::vec3& dir)
{
	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;//vector3::zero() - a

	//cant be behind b;

	//new direction towards a0
	dir = doubleCross(ab, ao);

	c = b;
	b = a;
	nrPointsSimplex = 2;

	return false;
}

bool GJK::triangle(glm::vec3& dir)
{
	glm::vec3 ao = glm::vec3(-a.x, -a.y, -a.z);
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 abc = cross(ab, ac);

	//point is can't be behind/in the direction of B,C or BC


	glm::vec3 ab_abc = cross(ab, abc);
	// is the origin away from ab edge? in the same plane
	//if a0 is in that direction than
	if (dot(ab_abc, ao) > 0)
	{
		//change points
		c = b;
		b = a;

		//dir is not ab_abc because it's not point towards the origin
		dir = doubleCross(ab, ao);

		//direction change; can't build tetrahedron
		return false;
	}


	glm::vec3 abc_ac = cross(abc, ac);

	// is the origin away from ac edge? or it is in abc?
	//if a0 is in that direction than
	if (dot(abc_ac, ao) > 0)
	{
		//keep c the same
		b = a;

		//dir is not abc_ac because it's not point towards the origin
		dir = doubleCross(ac, ao);

		//direction change; can't build tetrahedron
		return false;
	}

	//now can build tetrahedron; check if it's above or below
	if (dot(abc, ao) > 0)
	{
		//base of tetrahedron
		d = c;
		c = b;
		b = a;

		//new direction
		dir = abc;
	}
	else
	{
		//upside down tetrahedron
		d = b;
		b = a;
		dir = -abc;
	}

	nrPointsSimplex = 3;

	return false;
}

bool GJK::tetrahedron(glm::vec3& dir)
{
	glm::vec3 ao = -a; //0-a
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;

	//build abc triangle
	glm::vec3 abc = cross(ab, ac);

	//CASE 1
	if (dot(abc, ao) > 0)
	{
		//in front of triangle ABC
		//we don't have to change the ao,ab,ac,abc meanings
		checkTetrahedron(ao, ab, ac, abc, dir);
	}


	//CASE 2:

	glm::vec3 ad = d - a;

	//build acd triangle
	glm::vec3 acd = cross(ac, ad);

	//same direaction with ao
	if (dot(acd,ao) > 0)
	{

		//in front of triangle ACD
		b = c;
		c = d;
		ab = ac;
		ac = ad;
		abc = acd;

		checkTetrahedron(ao, ab, ac, abc, dir);
	}

	//build adb triangle
	glm::vec3 adb = cross(ad, ab);

	//same direaction with ao
	if (dot(adb,ao) > 0)
	{

		//in front of triangle ADB

		c = b;
		b = d;

		ac = ab;
		ab = ad;

		abc = adb;
		checkTetrahedron(ao, ab, ac, abc, dir);
	}


	//origin in tetrahedron
	return true;

}

bool GJK::checkTetrahedron(const glm::vec3& ao,
	const glm::vec3& ab,
	const glm::vec3& ac,
	const glm::vec3& abc,
	glm::vec3& dir)
{

	//almost the same like triangle checks
	glm::vec3 ab_abc = cross(ab, abc);

	if (dot(ab_abc,ao) > 0)
	{
		c = b;
		b = a;

		//dir is not ab_abc because it's not point towards the origin;
		//ABxA0xAB direction we are looking for
		dir = doubleCross(ab, ao);

		//build new triangle
		// d will be lost
		nrPointsSimplex = 2;

		return false;
	}

	glm::vec3 acp = cross(abc, ac);

	if (dot(acp,ao) > 0)
	{
		b = a;

		//dir is not abc_ac because it's not point towards the origin;
		//ACxA0xAC direction we are looking for
		dir = doubleCross(ac, ao);

		//build new triangle
		// d will be lost
		nrPointsSimplex = 2;

		return false;
	}

	//build new tetrahedron with new base
	d = c;
	c = b;
	b = a;

	dir = abc;

	nrPointsSimplex = 3;

	return false;
}