#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class Coordinate {
	//Stores three floats for the x, y, and z coordinates of a point
public:
	Coordinate(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
	void setX(float);
	void setY(float);
	void setZ(float);
private:
	float x, y, z;
};

class Face {
	//Stores a vector of integers corresponding
	//to coordinate elements in a vector of coordinates
public:
	Face(vector<int>);
	vector<int> vertices;
};

//Functions for reading the data in "stairs.txt" and storing
//the data as faces and coordinates
vector<Coordinate> readVertices(string fileName);
vector<Face> readFaces(string fileName);
vector<Face> readWrongFaces(string fileName);