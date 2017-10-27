/**
	Created by:  Gianni Ciccarelli and Phillip Wells
	CS300, Project 2:  Animated hierarchical object

	This program creates an OpenGL window that displays a spooky scary
	skeleton model which performs motions to moving lights when you press
	the 'r' key.  

	All models created by Gianni Ciccarelli using Blender

	Controls:
	Mouse:
	Click and drag to change the rotation of the skulls displayed

	Keyboard:
	'Q':	Press Q to exit the program
	'R':	Press R to start the skeleton rave
	'S':	Press S to stop the movement of the skeleton
**/
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#ifndef M_PI
#define M_PI 3.14159265
#endif
#define TAU (2 * M_PI)
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "ReadObj.h"
using namespace std;

// Material Properties below
static float matAmbient[4] = { 0.2, 0.2, 0.2, 1.0 };
static float matDiffuse[4] = { 0.8, 0.8, 0.8, 1.0 };
static float matSpecular[4] = { 0.4, 0.4, 0.4, 1.0 };
static float matNone[4] = { 0.0, 0.0, 0.0, 1.0 };
static float modelAmbient[4] = { 0.2, 0.2, 0.2, 1.0 };

//global for movement
int lightRot = 0;

//just a plain white color to draw the model with
int colorArray[] = {
	255, 255, 255
};


//light movement stuff -- adapted from the OpenGL examples page
typedef struct lightRec {
	float amb[4];
	float diff[4];
	float spec[4];
	float pos[4];
	float spotDir[3];
	float spotExp;
	float spotCutoff;
	float atten[3];

	float trans[3];
	float rot[3];
	float swing[3];
	float arc[3];
	float arcIncr[3];
} Light;

static Light spots[] =
{
	{
		{ 0.4, 0.0, 0.0, 1.0 },  /* ambient */
		{ 0.8, 0.0, 0.0, 1.0 },  /* diffuse */
		{ 0.4, 0.0, 0.0, 1.0 },  /* specular */
		{ 1.0, 1.0, 0.0, 1.0 },  /* position */
		{ 0.0, -3.0, 0.0 },   /* direction */
		20.0,
		60.0,             /* exponent, cutoff */
		{ 1.0, 0.0, 0.0 },    /* attenuation */
		{ 0.0, 1.25, 0.0 },   /* translation */
		{ 0.0, 0.0, 0.0 },    /* rotation */
		{ 20.0, 0.0, 40.0 },  /* swing */
		{ 0.0, 0.0, 0.0 },    /* arc */
		{ TAU / 70.0, 0.0, TAU / 140.0 }  /* arc increment */
	},
	{
		{ 0.0, 0.4, 0.0, 1.0 },  /* ambient */
		{ 0.0, 0.8, 0.0, 1.0 },  /* diffuse */
		{ 0.0, 0.4, 0.0, 1.0 },  /* specular */
		{ 1.0, 1.0, 0.0, 1.0 },  /* position */
		{ 0.0, -3.0, 0.0 },   /* direction */
		20.0,
		60.0,             /* exponent, cutoff */
		{ 1.0, 0.0, 0.0 },    /* attenuation */
		{ 0.0, 1.25, 0.0 },   /* translation */
		{ 0.0, 0.0, 0.0 },    /* rotation */
		{ 20.0, 0.0, 40.0 },  /* swing */
		{ 0.0, 0.0, 0.0 },    /* arc */
		{ TAU / 120.0, 0.0, TAU / 60.0 }  /* arc increment */
	},
	{
		{ 0.0, 0.0, 0.4, 1.0 },  /* ambient */
		{ 0.0, 0.0, 0.8, 1.0 },  /* diffuse */
		{ 0.0, 0.0, 0.4, 1.0 },  /* specular */
		{ 1.0, 1.0, 0.0, 1.0 },  /* position */
		{ 0.0, -3.0, 0.0 },   /* direction */
		20.0,
		60.0,             /* exponent, cutoff */
		{ 1.0, 0.0, 0.0 },    /* attenuation */
		{ 0.0, 1.25, 0.0 },   /* translation */
		{ 0.0, 0.0, 0.0 },    /* rotation */
		{ 20.0, 0.0, 40.0 },  /* swing */
		{ 0.0, 0.0, 0.0 },    /* arc */
		{ TAU / 50.0, 0.0, TAU / 100.0 }  /* arc increment */
	}
};

//actually make the lights
void createLights() {
	int i;

	for (i = 0; i < 3; ++i) {
		int lt = GL_LIGHT0 + i;
		Light *light = &spots[i];

		glEnable(lt);
		glLightfv(lt, GL_AMBIENT, light->amb);
		glLightfv(lt, GL_DIFFUSE, light->diff);

		if (1)
			glLightfv(lt, GL_SPECULAR, light->amb);
		else
			glLightfv(lt, GL_SPECULAR, light->spec);

		glLightf(lt, GL_SPOT_EXPONENT, light->spotExp);
		glLightf(lt, GL_SPOT_CUTOFF, light->spotCutoff);
		glLightf(lt, GL_CONSTANT_ATTENUATION, light->atten[0]);
		glLightf(lt, GL_LINEAR_ATTENUATION, light->atten[1]);
		glLightf(lt, GL_QUADRATIC_ATTENUATION, light->atten[2]);
	}
}

//point the lights
void aimLights() {
	int i;

	for (i = 0; i < 3; ++i) {
		Light *light = &spots[i];

		light->rot[0] = light->swing[0] * sin(light->arc[0]);
		light->arc[0] += light->arcIncr[0];
		if (light->arc[0] > TAU)
			light->arc[0] -= TAU;

		light->rot[1] = light->swing[1] * sin(light->arc[1]);
		light->arc[1] += light->arcIncr[1];
		if (light->arc[1] > TAU)
			light->arc[1] -= TAU;

		light->rot[2] = light->swing[2] * sin(light->arc[2]);
		light->arc[2] += light->arcIncr[2];
		if (light->arc[2] > TAU)
			light->arc[2] -= TAU;
	}
}

//move the lights
void positionLights() {
	int i;

	for (i = 0; i < 3; ++i) {
		int lt = GL_LIGHT0 + i;
		Light *light = &spots[i];

		glPushMatrix();
		glTranslatef(light->trans[0], light->trans[1], light->trans[2]);
		glRotatef(light->rot[0], 1, 0, 0);
		glRotatef(light->rot[1], 0, 1, 0);
		glRotatef(light->rot[2], 0, 0, 1);
		glLightfv(lt, GL_POSITION, light->pos);
		glLightfv(lt, GL_SPOT_DIRECTION, light->spotDir);
		glPopMatrix();
	}
}

/*
Initialize the separate models to combine into one great skeleton
Uses my ReadObj functions to load the data from the corresponding
obj files
*/
Model arm_left_lower("arm_left_lower.obj");
Model arm_left_upper("arm_left_upper.obj");
Model arm_right_lower("arm_right_lower.obj");
Model arm_right_upper("arm_right_upper.obj");
Model skull("final_skull.obj");
Model foot_left("foot_left.obj");
Model foot_right("foot_right.obj");
Model hand_left("hand_left.obj");
Model hand_right("hand_right.obj");
Model leg_left_lower("leg_left_lower.obj");
Model leg_left_upper("leg_left_upper.obj");
Model leg_right_lower("leg_right_lower.obj");
Model leg_right_upper("leg_right_upper.obj");
Model skeleton_body("skeleton_body.obj");

//A class to manage the many seperate limb models used in this project
class Rig {
public:
	void drawRig() {
		//Drawing the body as the main
		glPushMatrix();
		glTranslatef(bodyX, bodyY, bodyZ);
		glRotatef(bodyRotX, 1.0, 0.0, 0.0);
		glRotatef(bodyRotY, 0.0, 1.0, 0.0);
		glRotatef(bodyRotZ, 0.0, 0.0, 1.0);
		skeleton_body.drawNonTextured(colorArray);
		//Draw the skull within the body transformation matrix settings
		glPushMatrix();
		glTranslatef(skullX, skullY, skullZ);
		glRotatef(skullRotX, 1.0, 0.0, 0.0);
		glRotatef(skullRotY, 0.0, 1.0, 0.0);
		glRotatef(skullRotZ, 0.0, 0.0, 1.0);
		skull.drawNonTextured(colorArray);
		glPopMatrix();	//skull pop

		/*
		Draw right arm (upper, lower, hand)
		*/
		glPushMatrix();
		glTranslatef(armRUX, armRUY, armRUZ);
		glRotatef(armRUrotX, 1.0, 0.0, 0.0);
		glRotatef(armRUrotY, 0.0, 1.0, 0.0);
		glRotatef(armRUrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-armRUX, -armRUY, -armRUZ);
		arm_right_upper.drawNonTextured(colorArray);

		glPushMatrix();
		glTranslatef(armRLX, armRLY, armRLZ);
		glRotatef(armRLrotX, 1.0, 0.0, 0.0);
		glRotatef(armRLrotY, 0.0, 1.0, 0.0);
		glRotatef(armRLrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-armRLX, -armRLY, -armRLZ);
		arm_right_lower.drawNonTextured(colorArray);
		hand_right.drawNonTextured(colorArray);
		glPopMatrix();	//right hand pop
		glPopMatrix();	//right arm lower pop
		glPopMatrix();	//right arm upper pop

		/*
		Draw left arm (upper, lower, hand)
		*/
		glPushMatrix();
		glTranslatef(armLUX, armLUY, armLUZ);
		glRotatef(armLUrotX, 1.0, 0.0, 0.0);
		glRotatef(armLUrotY, 0.0, 1.0, 0.0);
		glRotatef(armLUrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-armLUX, -armLUY, -armLUZ);
		arm_left_upper.drawNonTextured(colorArray);
		//Draw left arm lower
		glPushMatrix();
		glTranslatef(armLLX, armLLY, armLLZ);
		glRotatef(armLLrotX, 1.0, 0.0, 0.0);
		glRotatef(armLLrotY, 0.0, 1.0, 0.0);
		glRotatef(armLLrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-armLLX, -armLLY, -armLLZ);
		arm_left_lower.drawNonTextured(colorArray);
		//Draw left hand
		glPushMatrix();
		glTranslatef(handLX, handLY, handLZ);
		glRotatef(handLrotX, 1.0, 0.0, 0.0);
		glRotatef(handLrotY, 0.0, 1.0, 0.0);
		glRotatef(handLrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-handLX, -handLY, -handLZ);
		hand_left.drawNonTextured(colorArray);
		glPopMatrix();	//left hand pop
		glPopMatrix();	//left arm lower pop
		glPopMatrix();	//left arm pop

		/*
		Draw right leg (upper, lower, foot)
		*/
		glPushMatrix();
		glTranslatef(legRUX, legRUY, legRUZ);
		glRotatef(legRUrotX, 1.0, 0.0, 0.0);
		glRotatef(legRUrotY, 0.0, 1.0, 0.0);
		glRotatef(legRUrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-legRUX, -legRUY, -legRUZ);
		leg_right_upper.drawNonTextured(colorArray);
		//Draw right leg lower
		glPushMatrix();
		glTranslatef(legRLX, legRLY, legRLZ);
		glRotatef(legRLrotX, 1.0, 0.0, 0.0);
		glRotatef(legRLrotY, 0.0, 1.0, 0.0);
		glRotatef(legRLrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-legRLX, -legRLY, -legRLZ);
		leg_right_lower.drawNonTextured(colorArray);
		//Draw right foot
		glPushMatrix();
		glTranslatef(footRX, footRY, footRZ);
		glRotatef(footRrotX, 1.0, 0.0, 0.0);
		glRotatef(footRrotY, 0.0, 1.0, 0.0);
		glRotatef(footRrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-footRX, -footRY, -footRZ);
		foot_right.drawNonTextured(colorArray);
		glPopMatrix();	//right foot pop
		glPopMatrix();	//right leg lower pop
		glPopMatrix();	//right leg pop

		/*
		Draw left leg (upper, lower, foot)
		*/
		glPushMatrix();
		glTranslatef(legLUX, legLUY, legLUZ);
		glRotatef(legLUrotX, 1.0, 0.0, 0.0);
		glRotatef(legLUrotY, 0.0, 1.0, 0.0);
		glRotatef(legLUrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-legLUX, -legLUY, -legLUZ);
		leg_left_upper.drawNonTextured(colorArray);
		//Draw left leg lower
		glPushMatrix();
		glTranslatef(legLLX, legLLY, legLLZ);
		glRotatef(legLLrotX, 1.0, 0.0, 0.0);
		glRotatef(legLLrotY, 0.0, 1.0, 0.0);
		glRotatef(legLLrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-legLLX, -legLLY, -legLLZ);
		leg_left_lower.drawNonTextured(colorArray);
		//Draw left foot
		glPushMatrix();
		glTranslatef(footLX, footLY, footLZ);
		glRotatef(footLrotX, 1.0, 0.0, 0.0);
		glRotatef(footLrotY, 0.0, 1.0, 0.0);
		glRotatef(footLrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-footLX, -footLY, -footLZ);
		foot_left.drawNonTextured(colorArray);
		glPopMatrix();	//left foot pop
		glPopMatrix();	//left leg lower pop
		glPopMatrix();	//left leg pop

		glPopMatrix();	//body pop
	}

	/*
	The following are pretty self-explanatory functions
	used for rotating the corresponding body parts
	*/
	void moveBody() {
		//unused
		bodyX += rand() % 2;
		bodyY += rand() % 2;
		bodyZ += rand() % 2;
	}
	void rotateBody(float x, float y, float z) {
		bodyRotX += x;
		bodyRotY += y;
		bodyRotZ += z;
	}
	void rotateArmRightUpper(float x, float y, float z) {
		armRUrotX += x;
		armRUrotY += y;
		armRUrotZ += z;
	}
	void rotateArmRightLower(float x, float y, float z) {
		armRLrotX += x;
		armRLrotY += y;
		armRLrotZ += z;
	}
	void rotateArmLeftUpper(float x, float y, float z) {
		armLUrotX += x;
		armLUrotY += y;
		armLUrotZ += z;
	}
	void rotateArmLeftLower(float x, float y, float z) {
		armLLrotX += x;
		armLLrotY += y;
		armLLrotZ += z;
	}
	void rotateLegRightUpper(float x, float y, float z) {
		legRUrotX += x;
		legRUrotY += y;
		legRUrotZ += z;
	}
	void rotateLegRightLower(float x, float y, float z) {
		legRLrotX += x;
		legRLrotY += y;
		legRLrotZ += z;
	}
	void rotateLegLeftUpper(float x, float y, float z) {
		legLUrotX += x;
		legLUrotY += y;
		legLUrotZ += z;
	}
	void rotateLegLeftLower(float x, float y, float z) {
		legLLrotX += x;
		legLLrotY += y;
		legLLrotZ += z;
	}
	void rotateSkull(float x, float y, float z) {
		skullRotY += y;
	}

private:
	/*
	A whole lotta floats to hold rotation data of individual body parts
	The x, y, and z components are the locations of the pivot points
	*/
	float bodyX, bodyY, bodyZ, bodyRotX, bodyRotY, bodyRotZ;
	float skullX, skullY, skullZ, skullRotX, skullRotY, skullRotZ;

	float armRUX = -0.67702, armRUY = 5.62719, armRUZ = -0.05725, 
		armRUrotX, armRUrotY, armRUrotZ;
	float armRLX = -0.82596, armRLY = 4.21011, armRLZ = -0.02102, 
		armRLrotX, armRLrotY, armRLrotZ;
	float handRX = -0.84435, handRY = 3.34702, handRZ = -0.44979, 
		handRrotX, handRrotY, handRrotZ;

	float armLUX = 0.67702, armLUY = 5.62719, armLUZ = -0.05725, 
		armLUrotX, armLUrotY, armLUrotZ;
	float armLLX = 0.82596, armLLY = 4.21011, armLLZ = -0.02102, 
		armLLrotX, armLLrotY, armLLrotZ;
	float handLX = 0.84435, handLY = 3.34702, handLZ = -0.44979, 
		handLrotX, handLrotY, handLrotZ;

	float legRUX = -0.39565, legRUY = 3.64407, legRUZ = -0.11636, 
		legRUrotX, legRUrotY, legRUrotZ;
	float legRLX = -0.33641, legRLY = 1.78488, legRLZ = -0.03953, 
		legRLrotX, legRLrotY, legRLrotZ;
	float footRX = -0.27710, footRY = 0.30598, footRZ = 0.06849, 
		footRrotX, footRrotY, footRrotZ;

	float legLUX = 0.39565, legLUY = 3.64407, legLUZ = -0.11636, 
		legLUrotX, legLUrotY, legLUrotZ;
	float legLLX = 0.33641, legLLY = 1.78488, legLLZ = -0.03953, 
		legLLrotX, legLLrotY, legLLrotZ;
	float footLX = 0.27710, footLY = 0.30598, footLZ = 0.06849, 
		footLrotX, footLrotY, footLrotZ;
};

//The crowning jewel of this project:
Rig skeletonRig;

//Global vars to store the angle of rotation of the model
static GLfloat angle = 0;
static GLfloat angle2 = 0;

//Motion variables for the mouse rotation capability
static int moving = 0, startx = 0, starty = 0;

//Init function for more openGL stuff
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
		-4.0,
		4.0,
		0.0,
		8.0,
		-8.0,
		8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Setup initial OpenGL rendering state. */
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

//Probably didn't need a separate function for this
void drawSkeleton() {
	skeletonRig.drawRig();
}


//Just draws a flat plane on the ground
void drawDanceFloor() {
	glBegin(GL_POLYGON);
	glVertex3f(-4, 0, -4);
	glVertex3f(-4, 0, 4);
	glVertex3f(4, 0, 4);	//vertex not found
	glVertex3f(4, 0, -4);
	glEnd();
}


//
void renderScene() {
	//glutSolidTeapot(1.0);	//used for testing porpoises
	glPushMatrix();
	glTranslatef(0, 0, -1);
	drawSkeleton();
	drawDanceFloor();
	glPopMatrix();
}

void resetMaterials() {
	//Reset the materials to no light			
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat no_shine[] = { 0.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, no_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shine);
}

//OpenGL display function
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glScalef(2.5, 2.5, 1);
	glTranslatef(0, 3, -1);
	aimLights();
	positionLights();
	glPopMatrix();
	glPushMatrix();
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	renderScene();
	glPopMatrix();
	glutSwapBuffers();
}

/*
The secret sauce of the exorcist-like movement of the skeleton:
	random movement.
*/
void animate() {
	//Rotate the skull around the y axis
	skeletonRig.rotateSkull(0, 5, 0);
	//Rotate everything else randomly
	//I threw in some random seed calls to shake it up a bit
	skeletonRig.rotateArmRightUpper(rand() % 2, rand() % 2, rand() % 2);
	srand(time(NULL));
	skeletonRig.rotateArmRightLower(rand() % 2, rand() % 2, rand() % 2);
	srand(time(NULL));
	skeletonRig.rotateArmLeftUpper(rand() % 2, rand() % 2, rand() % 2);
	skeletonRig.rotateArmLeftLower(rand() % 2, rand() % 2, rand() % 2);
	srand(time(NULL));
	skeletonRig.rotateLegRightUpper(rand() % 2, rand() % 2, rand() % 2);
	skeletonRig.rotateLegRightLower(rand() % 2, rand() % 2, rand() % 2);
	skeletonRig.rotateLegLeftUpper(rand() % 2, rand() % 2, rand() % 2);
	srand(time(NULL));
	skeletonRig.rotateLegLeftLower(rand() % 2, rand() % 2, rand() % 2);
	lightRot += 1.0;
	if (lightRot > 360)
		lightRot -= 360;
	glutPostRedisplay();
}

//OpenGL clear function
void clear()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
		-4.0,
		4.0,
		0.0,
		8.0,
		-8.0,
		8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Mouse control function
static void mouse(int button, int state, int x, int y)
{
	/* Rotate the scene with the left mouse button. */
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
}


//OpenGL keyboard input
//	'q':  exit
//	'r':  begin random rotation of skeleton limbs
//	's':  stop random rotation of skeleton limbs
void keyboard(unsigned char key, int x, int y) {
#pragma unused(x, y)
	switch (key) {
	case 'q':
		exit(0);
		break;
	case 'r':
		glutIdleFunc(animate);
		break;
	case 's':
		glutIdleFunc(NULL);
		break;
	default:
		break;
	}
	display();
}

static void motion(int x, int y)
{
	if (moving) {
		angle = (angle + (x - startx));
		angle2 = (angle2 + (y - starty));
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

//OpenGL reshape function
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(
			-4.0,
			4.0,
			0.0 * (GLfloat)h / (GLfloat)w,
			8.0 * (GLfloat)h / (GLfloat)w,
			-8.0,
			8.0);
	else
		glOrtho(
			-4.0 * (GLfloat)w / (GLfloat)h,
			4.0 * (GLfloat)w / (GLfloat)h,
			0.0,
			8.0,
			-8.0,
			8.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Spooky Scary Cs Project");

	/* Register assorted GLUT callback routines. */
	init();

	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(45.0, 1, 0, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, modelAmbient);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, matNone);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

	createLights();
	glutIdleFunc(NULL);
	glutReshapeFunc(reshape);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);

	glutMainLoop();
	return 0;
}