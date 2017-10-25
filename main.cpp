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

#define PI 3.14159 

Model test("low_poly_skull.obj");

//Global vars to store the angle of rotation of the model
static GLfloat angle = 0;
static GLfloat angle2 = 0;

static int moving = 0, startx = 0, starty = 0;

//Keeps track of which render scene to draw, and the total number of scenes that can be drawn
int scene = 0, maxScene = 2;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, -8.0, 8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Setup initial OpenGL rendering state. */
	glEnable(GL_DEPTH_TEST);
	/*
	Yeah don't forget to try and improve framerate by culling faces
	*/
	glEnable(GL_CULL_FACE);
}


void drawSkull() {
	int colorArray[] = {
		255, 255, 255
	};
	test.drawNonTextured(colorArray);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	/* Perform scene rotations based on user mouse input. */

	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	drawSkull();

	glPopMatrix();
	glutSwapBuffers();
}

void clear()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, -8.0, 8.0);
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
//	'r':  reset to starting perspective
//	't' : cycle through different stair renders
//	'w', 'a', 's', 'd' : rotate model and print the new angle values
void keyboard(unsigned char key, int x, int y) {
#pragma unused(x, y)
	switch (key) {
	case 'q':
		exit(0);
		break;
	case 'r':
		angle = -26;
		angle2 = 42;
		display();
		break;
	case 'a':
		angle += 1;
		cout << angle << " " << angle2 << endl;
		display();
		break;
	case 'd':
		angle -= 1;
		cout << angle << " " << angle2 << endl;
		display();
		break;
	case 'w':
		angle2 -= 1;
		cout << angle << " " << angle2 << endl;
		display();
		break;
	case 's':
		angle2 += 1;
		cout << angle << " " << angle2 << endl;
		display();
		break;
	case 't':
		if (scene == maxScene) {
			scene = 0;
		}
		else scene++;
		display();
		break;
	default:
		break;
	}
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
		glOrtho(-8.0, 8.0, -8.0 * (GLfloat)h / (GLfloat)w,
			8.0 * (GLfloat)h / (GLfloat)w, -8.0, 8.0);
	else
		glOrtho(-8.0 * (GLfloat)w / (GLfloat)h,
			8.0 * (GLfloat)w / (GLfloat)h, -8.0, 8.0, -8.0, 8.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Penrose Stairs");

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