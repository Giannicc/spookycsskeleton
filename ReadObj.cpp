#include "ReadObj.h"
/**
Model class functions
**/
Model::Model(string objSource) {
	drawNormals = false;
	ifstream source;
	source.open(objSource, ios::in);
	if (source.is_open()) {
		cout << "Reading " << objSource << endl;
		string input;	//We put lines gotten from "source" into input
		while (!source.eof()) {
			getline(source, input);
			istringstream iss(input);
			string lineHead;
			iss >> lineHead;	//We put the strings in input into a string stream
			if (lineHead == "v") {	//Read 3 doubles for the vertex coords
				vector<double> coords;
				double x, y, z;
				iss >> x >> y >> z;
				coords.push_back(x);
				coords.push_back(y);
				coords.push_back(z);
				modelVertices.push_back(coords);
			}
			else if (lineHead == "vn") {	//Reading vertex normal vector
				vector<double> norm;
				double x, y, z;
				iss >> x >> y >> z;
				norm.push_back(x);
				norm.push_back(y);
				norm.push_back(z);
				normVectors.push_back(norm);
			}
			else if (lineHead == "vt") {	//Texture coordinate!
				vector<double> coords;
				double x, y;
				iss >> x >> y;
				coords.push_back(x);
				coords.push_back(y);
				textureCoords.push_back(coords);
			}
			else if (lineHead == "f") {	//Reading face data
				Face newFace;
				string faceData;
				while (iss >> faceData) {
					int pos, index = 0;
					string vertNum, texNum, normNum;
					while (faceData[index] != '/') {	//Get the vertex number
						vertNum += faceData[index];
						index++;
					}
					if (vertNum != "") {
						newFace.faceVertexNums.push_back(stoi(vertNum) - 1);
					}
					else cout << "Error when reading face vertex data" << endl;
					index++;
					while (faceData[index] != '/') {	//Get the tex coord num
						texNum += faceData[index];
						index++;
					}
					if (texNum != "") {
						newFace.faceTexNums.push_back(stoi(texNum) - 1);
					}
					//else cout << "Error when reading face texture data" << endl;
					index++;
					while (faceData[index] != '\0') {
						normNum += faceData[index];
						index++;
					}
					if (normNum != "") {
						newFace.faceNormNums.push_back(stoi(normNum) - 1);
					}
					else cout << "Error when reading face normal data" << endl;
				}
				modelFaces.push_back(newFace);
			}
		}
	}
	else {
		cout << "Failed to open file" << endl;
		source.close();
		exit(1);
	}
	source.close();
}

void Model::drawNonTextured(int colorArray[3]) {
	for (int i = 0; i < modelFaces.size(); i++) {
		glBegin(GL_POLYGON);
		//Ensure the colors don't go out of bounds
		int r = colorArray[0] % 255,
			g = colorArray[1] % 255,
			b = colorArray[2] % 255;
		if (r == 0 && colorArray[0] != 0) r = 255;
		if (g == 0 && colorArray[0] != 0) g = 255;
		if (b == 0 && colorArray[0] != 0) b = 255;
		glColor3ub(r, g, b);

		//Normal vector vector
		vector<double> norm = normVectors[modelFaces[i].faceNormNums[0]];
		for (int j = 0; j < modelFaces[i].faceVertexNums.size(); j++) {
			glNormal3f(norm[0], norm[1], norm[2]);
			//Get the vertex from the vector of vertices stored in the Model
			//object at the corresponding index given by the indices specified
			//in the vertex vector in the Face class
			//Yeah that's not very intuitive sorry, but it's how I think
			vector<double> verts =
				modelVertices[modelFaces[i].faceVertexNums[j]];
			glVertex3f(verts[0], verts[1], verts[2]);
		}
		glEnd();
		
		//This is just stuff to draw normal vectors to test if my faces have
		//the correct normals
		if (drawNormals) {
			glBegin(GL_LINES);
			glColor3ub(255, 0, 0);
			vector<double> verts = 
				modelVertices[modelFaces[i].faceVertexNums[0]];
			glVertex3f(verts[0], verts[1], verts[2]);
			glVertex3f(
				verts[0] + norm[0], 
				verts[1] + norm[1], 
				verts[2] + norm[2]);
			glEnd();
		}
	}
}

//This just sets the bool we reference when we check if we want to draw
//normal vectors on the model
void Model::doNorms(bool veracity) {
	drawNormals = veracity;
}
