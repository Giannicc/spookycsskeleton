#include <iostream>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "ReadObj.h"
using namespace std;

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
vector<Model> fullSkeleton = {
	arm_left_lower,
	arm_left_upper,
	arm_right_lower,
	arm_right_upper,
	skull,
	foot_left,
	foot_right,
	hand_left,
	hand_right,
	leg_left_lower,
	leg_left_upper,
	leg_right_lower,
	leg_right_upper,
	skeleton_body
};

//Global vars to store the angle of rotation of the model
static GLfloat angle = 0;
static GLfloat angle2 = 0;

static int moving = 0, startx = 0, starty = 0;

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
	/*
	Yeah don't forget to try and improve framerate by culling faces
	ok but my model now uses planes that will disappear (ribs)
	*/
	//glEnable(GL_CULL_FACE);
}


void drawSkeleton() {
	int colorArray[] = {
		255, 255, 255
	};
	for (int i = 0; i < fullSkeleton.size(); i++) {
		fullSkeleton[i].drawNonTextured(colorArray);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	/* Perform scene rotations based on user mouse input. */

	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	drawSkeleton();

	glPopMatrix();
	glutSwapBuffers();
}

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
void keyboard(unsigned char key, int x, int y) {
#pragma unused(x, y)
	switch (key) {
	case 'q':
		exit(0);
		break;
	default:
		break;
	}
	display();
}

/* ARGSUSED1 */
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
	glutReshapeFunc(reshape);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);

	/* Enter GLUT's main loop; callback dispatching begins. */
	glutMainLoop();
	return 0;
}