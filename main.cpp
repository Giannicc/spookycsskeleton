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

int colorArray[] = {
	255, 255, 255
};

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
		glTranslatef(0.73281, -5.61824, 0.06215);	//move to origin
		glRotatef(armRUrotX, 1.0, 0.0, 0.0);
		glRotatef(armRUrotY, 0.0, 1.0, 0.0);
		glRotatef(armRUrotZ, 0.0, 0.0, 1.0);
		glTranslatef(-0.73281, 5.61824, -0.06215);	//move back to point
		glTranslatef(armRUX, armRUY, armRUZ);
		arm_right_upper.drawNonTextured(colorArray);
		//Draw right arm lower
		glPushMatrix();
		glTranslatef(armRLX, armRLY, armRLZ);
		glRotatef(armRLrotX, 1.0, 0.0, 0.0);
		glRotatef(armRLrotY, 0.0, 1.0, 0.0);
		glRotatef(armRLrotZ, 0.0, 0.0, 1.0);
		arm_right_lower.drawNonTextured(colorArray);
		//Draw right hand
		glPushMatrix();
		glTranslatef(handRX, handRY, handRZ);
		glRotatef(handRrotX, 1.0, 0.0, 0.0);
		glRotatef(handRrotY, 0.0, 1.0, 0.0);
		glRotatef(handRrotZ, 0.0, 0.0, 1.0);
		hand_right.drawNonTextured(colorArray);
		glPopMatrix();	//right hand pop
		glPopMatrix();	//right arm lower pop
		glPopMatrix();	//right arm pop

		/*
		Draw left arm (upper, lower, hand)
		*/
		glPushMatrix();
		glTranslatef(armLUX, armLUY, armLUZ);
		glRotatef(armLUrotX, 1.0, 0.0, 0.0);
		glRotatef(armLUrotY, 0.0, 1.0, 0.0);
		glRotatef(armLUrotZ, 0.0, 0.0, 1.0);
		arm_left_upper.drawNonTextured(colorArray);
		//Draw left arm lower
		glPushMatrix();
		glTranslatef(armLLX, armLLY, armLLZ);
		glRotatef(armLLrotX, 1.0, 0.0, 0.0);
		glRotatef(armLLrotY, 0.0, 1.0, 0.0);
		glRotatef(armLLrotZ, 0.0, 0.0, 1.0);
		arm_left_lower.drawNonTextured(colorArray);
		//Draw left hand
		glPushMatrix();
		glTranslatef(handLX, handLY, handLZ);
		glRotatef(handLrotX, 1.0, 0.0, 0.0);
		glRotatef(handLrotY, 0.0, 1.0, 0.0);
		glRotatef(handLrotZ, 0.0, 0.0, 1.0);
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
		leg_right_upper.drawNonTextured(colorArray);
		//Draw right leg lower
		glPushMatrix();
		glTranslatef(legRLX, legRLY, legRLZ);
		glRotatef(legRLrotX, 1.0, 0.0, 0.0);
		glRotatef(legRLrotY, 0.0, 1.0, 0.0);
		glRotatef(legRLrotZ, 0.0, 0.0, 1.0);
		leg_right_lower.drawNonTextured(colorArray);
		//Draw right foot
		glPushMatrix();
		glTranslatef(footRX, footRY, footRZ);
		glRotatef(footRrotX, 1.0, 0.0, 0.0);
		glRotatef(footRrotY, 0.0, 1.0, 0.0);
		glRotatef(footRrotZ, 0.0, 0.0, 1.0);
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
		leg_left_upper.drawNonTextured(colorArray);
		//Draw left leg lower
		glPushMatrix();
		glTranslatef(legLLX, legLLY, legLLZ);
		glRotatef(legLLrotX, 1.0, 0.0, 0.0);
		glRotatef(legLLrotY, 0.0, 1.0, 0.0);
		glRotatef(legLLrotZ, 0.0, 0.0, 1.0);
		leg_left_lower.drawNonTextured(colorArray);
		//Draw left foot
		glPushMatrix();
		glTranslatef(footLX, footLY, footLZ);
		glRotatef(footLrotX, 1.0, 0.0, 0.0);
		glRotatef(footLrotY, 0.0, 1.0, 0.0);
		glRotatef(footLrotZ, 0.0, 0.0, 1.0);
		foot_left.drawNonTextured(colorArray);
		glPopMatrix();	//left foot pop
		glPopMatrix();	//left leg lower pop
		glPopMatrix();	//left leg pop

		glPopMatrix();	//body pop
	}

	void moveBody(float x, float y, float z) {
		bodyX += x;
		bodyY += y;
		bodyZ += z;
	}
	void rotateBody(float x, float y, float z) {
		bodyRotX += x;
		bodyRotY += y;
		bodyRotZ += z;
	}
	void rotateArmRight(float x, float y, float z) {
		armRUrotX += x;
		armRUrotY += y;
		armRUrotZ += z;
	}

private:
	float bodyX, bodyY, bodyZ, bodyRotX, bodyRotY, bodyRotZ;
	float skullX, skullY, skullZ, skullRotX, skullRotY, skullRotZ;

	float armRUX, armRUY, armRUZ, armRUrotX, armRUrotY, armRUrotZ;
	float armRLX, armRLY, armRLZ, armRLrotX, armRLrotY, armRLrotZ;
	float handRX, handRY, handRZ, handRrotX, handRrotY, handRrotZ;

	float armLUX, armLUY, armLUZ, armLUrotX, armLUrotY, armLUrotZ;
	float armLLX, armLLY, armLLZ, armLLrotX, armLLrotY, armLLrotZ;
	float handLX, handLY, handLZ, handLrotX, handLrotY, handLrotZ;

	float legRUX, legRUY, legRUZ, legRUrotX, legRUrotY, legRUrotZ;
	float legRLX, legRLY, legRLZ, legRLrotX, legRLrotY, legRLrotZ;
	float footRX, footRY, footRZ, footRrotX, footRrotY, footRrotZ;

	float legLUX, legLUY, legLUZ, legLUrotX, legLUrotY, legLUrotZ;
	float legLLX, legLLY, legLLZ, legLLrotX, legLLrotY, legLLrotZ;
	float footLX, footLY, footLZ, footLrotX, footLrotY, footLrotZ;
};

Rig skeletonRig;

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
	glEnable(GL_LIGHTING);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}


void drawSkeleton() {
	skeletonRig.drawRig();
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

void display()
{
	resetMaterials();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat light_pos[] = { 1.0,1.0,1.0,0.0 }; //light position
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //white light
	GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
	GLfloat shininess[] = { 20.0 };
	//specular reflection coefficient.
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	GLfloat light_ambient[] = { 0.2,0.5,0.5,1.0 }; //ambient light intensity
	GLfloat light_diffuse[] = { 1.0,1.0,1.0,1.0 }; //diffuse light intensity
	GLfloat mat_ambient[] = { 0.2,0.2,0.2,1.0 }; //ambient color
	GLfloat mat_diffuse[] = { 0.2,0.5,0.5,1.0 }; /*diffuse color~kd, no direct correlation in openGL*/
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glEnable(GL_LIGHT0);
	glPushMatrix();
	/* Perform scene rotations based on user mouse input. */
	glLoadIdentity();
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	drawSkeleton();

	glPopMatrix();
	resetMaterials();
	glutSwapBuffers();
}

void animate() {

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
	case 'w':
		skeletonRig.moveBody(0.0, 0.1, 0.0);
		break;
	case 's':
		skeletonRig.moveBody(0.0, -0.1, 0.0);
		break;
	case 'a':
		skeletonRig.moveBody(-0.1, 0.0, 0.0);
		break;
	case 'd':
		skeletonRig.moveBody(0.1, 0.0, 0.0);
		break;
	case 'r':
		skeletonRig.rotateArmRight(0, 0, 1);
		break;
	case 'f':
		skeletonRig.rotateArmRight(0, 0, -1);
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