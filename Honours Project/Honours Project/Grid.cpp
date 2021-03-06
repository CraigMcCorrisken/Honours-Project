#include "Grid.h"
#include <iostream>

Grid::Grid(int width, int height, int depth, int cellSize) : cellSize(cellSize),
width(width), height(height), depth(depth)
{
	columns = width / cellSize;
	rows = height / cellSize;
}

Grid::~Grid()
{
	for (auto it = objBuckets.begin(); it != objBuckets.end(); it++)
	{
		it->second.clear();
	}
	objBuckets.clear();
}

Indices Grid::computeCellIndices(vec3 position)
{
	int i = position.x / columns;
	int j = position.y / rows;
	return Indices(i, j);
}

void Grid::registerObj(Shape* obj)
{
	Indices cellInd = computeCellIndices(vec3(obj->getPosition().x, 
			obj->getPosition().y, obj->getPosition().z));

	objBuckets[cellInd].push_back(obj);
}

vector<Shape*> Grid::getNeighbours(Shape* obj)
{
	vector<Shape*> neighbours;

	Indices cellInd = computeCellIndices(vec3(obj->getPosition().x,
		obj->getPosition().y, obj->getPosition().z));
	neighbours.insert(neighbours.end(), objBuckets[cellInd].begin(), objBuckets[cellInd].end());

	Indices E(cellInd.first + 1, cellInd.second);
	if (objBuckets.count(E)>0)
		neighbours.insert(neighbours.end(), objBuckets[E].begin(), objBuckets[E].end());

	Indices SE(cellInd.first + 1, cellInd.second + 1);
	if (objBuckets.count(SE)>0)
		neighbours.insert(neighbours.end(), objBuckets[SE].begin(), objBuckets[SE].end());

	Indices S(cellInd.first, cellInd.second + 1);
	if (objBuckets.count(S)>0)
		neighbours.insert(neighbours.end(), objBuckets[S].begin(), objBuckets[S].end());

	return neighbours;
}

void Grid::clearGrid()
{
	objBuckets.clear();
}