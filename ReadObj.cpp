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
		string input;
		while (!source.eof()) {
			getline(source, input);
			char * buffer = new char[input.length()];
			//actually copy input to buffer:
			strcpy(buffer, input.c_str());
			//Loading vertex data
			if (input.substr(0, 2) == "v ") {
				vector<double> coord;
				double x, y, z;
				sscanf(buffer, "v %lf %lf %lf", &x, &y, &z);
				coord.push_back(x);
				coord.push_back(y);
				coord.push_back(z);
				modelVertices.push_back(coord);
			}
			else if (input.substr(0, 2) == "vn") {
				vector<double> norm;
				double x, y, z;
				sscanf(buffer, "vn %lf %lf %lf", &x, &y, &z);
				norm.push_back(x);
				norm.push_back(y);
				norm.push_back(z);
				normVectors.push_back(norm);
			}
			else if (input.substr(0, 2) == "vt") {
				vector<double> coord;
				double x, y;
				sscanf(buffer, "vt %lf %lf", &x, &y);
				coord.push_back(x);
				coord.push_back(y);
			}
			else if (input.substr(0, 2) == "f ") {
				//here we go with the face stuff ohhh boy
				Face newFace;
				istringstream stream(input);
				string faceData;
				//eat the 'f' and then get our first set of data:
				stream >> faceData >> faceData;
				do {
					//check if there's no texture data, we'll want to handle
					//it differently if we find a "//"
					if (faceData.find("//") == string::npos) {
						char * dataBuffer = new char[faceData.length()];
						strcpy(dataBuffer, faceData.c_str());
						int vertexNum = 0, texNum = 0, normNum = 0;
						sscanf(dataBuffer, "%d/%d/%d", &vertexNum, &texNum, &normNum);
						newFace.faceVertexNums.push_back(vertexNum - 1);
						newFace.faceTexNums.push_back(texNum - 1);
						newFace.faceNormNums.push_back(normNum - 1);
					}
					else {
						char * dataBuffer = new char[faceData.length()];
						strcpy(dataBuffer, faceData.c_str());
						int vertexNum = 0, normNum = 0;
						sscanf(dataBuffer, "%d//%d", &vertexNum, &normNum);
						newFace.faceVertexNums.push_back(vertexNum - 1);
						newFace.faceNormNums.push_back(normNum - 1);
					}
				} while (stream >> faceData);
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
