#include "ReadObj.h"

//Constructor
Coordinate::Coordinate(float valuex, float valuey, float valuez) {
    x = valuex;
    y = valuey;
    z = valuez;
}

float Coordinate::getX() {
    return x;
}

float Coordinate::getY() {
    return y;
}

float Coordinate::getZ() {
    return z;
}

void Coordinate::setX(float value) {
    x = value;
}

void Coordinate::setY(float value) {
    y = value;
}

void Coordinate::setZ(float value) {
    z = value;
}

//Constructor
Face::Face(vector<int> verts) {
    for (int i = 0; i < verts.size(); i++) {
	vertices.push_back(verts[i]);
    }
}

//Returns a vector of Coordinates made
//by reading data in the file
vector<Coordinate> readVertices(string fileName) {
    vector<Coordinate> coordVector;
    ifstream file;
    //Open the file for reading
    file.open(fileName);
    while (!file.eof()) {
	//as long as we haven't reached the end of the file
	//get a new line of input
	string input;
	getline(file, input);
	//the line in the obj file starts with 'v' to indicate a vertex
	//which we will convert to a coordinate and then
	//push back into the vector "coordVector"
	if (input[0] == 'v') {
	    float x, y, z;
	    istringstream iss(input);

	    //This just eats the 'v' that starts the line
	    string garbage;
	    iss >> garbage >> x >> y >> z;
	    Coordinate coord(x, y - 2.5, z);
	    coordVector.push_back(coord);
	}
    }
    file.close();
    return coordVector;
}

//Returns a vector of Faces made from
//the data read in the file
vector<Face> readFaces(string fileName) {
    vector<Face> faces;
    ifstream file;
    //Open the file for reading
    file.open(fileName);
    while (!file.eof()) {
	string input;
	getline(file, input);
	//the line starts with 'f' to signify that it's face data
	if (input[0] == 'f') {
	    vector<int> vertices;
	    istringstream iss(input);
	    string garbage;
	    //eat the 'f', then this eats every other integer
	    //because every other integer is a value for a normal
	    //which we aren't dealing with in this project
	    while (iss >> garbage) {
		int x = -1;
		iss >> x;
		//as long as x has been set to a value it means we are reading data
		//Because we put the vertices in a vector whose indices start at 0,
		//we want to subtract one from x so that we are using the correct vertex
		if(x != -1) vertices.push_back(x - 1);
	    }
	    Face polygon(vertices);
	    faces.push_back(polygon);
	}
    }
    return faces;
}

//Same as the readFaces function, but because the faces in the obj file
//use vertex elements starting at 1, this causes the faces to load wrong
//It's an interesting visual effect so we kept it
vector<Face> readWrongFaces(string fileName) {
    vector<Face> faces;
    ifstream file;
    file.open(fileName);
    while (!file.eof()) {
	string input;
	getline(file, input);
	if (input[0] == 'f') {
	    vector<int> vertices;
	    istringstream iss(input);
	    string garbage;
	    while (iss >> garbage) {
		int x = -1;
		iss >> x;
		if (x != -1) vertices.push_back(x);
	    }
	    Face polygon(vertices);
	    faces.push_back(polygon);
	}
    }
    return faces;
}
