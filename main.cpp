#include <iostream>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

using namespace std;

#define PI 3.14159 

static GLfloat angle = -150;   /* in degrees */
static GLfloat angle2 = 30;   /* in degrees */

static int moving = 0, startx=0, starty=0;

//Draw the cube. 
GLfloat vertices[][3] ={{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},{1.0,1.0,-1.0},{-1.0,1.0,-1.0},
			{-1.0,-1.0,1.0},{1.0,-1.0,1.0},{1.0,1.0,1.0},{-1.0,1.0,1.0}}; 
GLfloat colors[][3] ={{0.0,0.0,0.0},{1.0,0.0,0.0},{1.0,1.0,0.0},{0.0,1.0,0},
		      {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}}; 
void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-6.0,6.0,-6.0,6.0,-6.0,6.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 	
    /* Setup initial OpenGL rendering state. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
 
void polygon(int a, int b, int c ,int d) 
/* draw a polygon via list of vertices*/ 
{
    glBegin(GL_POLYGON); 
    glColor3fv(colors[a]); 
    glVertex3fv(vertices[a]); 
    glColor3fv(colors[b]); 
    glVertex3fv(vertices[b]); 
    glColor3fv(colors[c]); 
    glVertex3fv(vertices[c]); 
    glColor3fv(colors[d]); 
    glVertex3fv(vertices[d]); 
    glEnd();
} 

void colorcube()
/* map vertices to faces*/
{
    polygon(1,0,3,2); 
    polygon(3,7,6,2); 
    polygon(7,3,0,4); 
    polygon(2,6,5,1); 
    polygon(4,5,6,7); 
    polygon(5,4,0,1); 
}
  
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    /* Perform scene rotations based on user mouse input. */
    	
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    colorcube();
    glPopMatrix();
    glutSwapBuffers();
}

//OpenGL keyboard input
//	'q':  exit
void keyboard(unsigned char key, int x, int y){
#pragma unused(x, y)
    switch (key) {
    case 'q':
	exit(0);
	break;
    }
}

static void
mouse(int button, int state, int x, int y)
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

/* ARGSUSED1 */
static void
motion(int x, int y)
{
    if (moving) {
	angle = (angle + (x - startx));
	angle2 = (angle2 + (y - starty));
	startx = x;
	starty = y;
	glutPostRedisplay();
    }
}

//OpenGL reshape function to handle window resizing
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
	glOrtho(-6.0, 6.0, -6.0 * (GLfloat)h / (GLfloat)w,
		6.0 * (GLfloat)h / (GLfloat)w, -6.0, 6.0);
    else
	glOrtho(-6.0 * (GLfloat)w / (GLfloat)h,
		6.0 * (GLfloat)w / (GLfloat)h, -6.0, 6.0, -6.0, 6.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Get spooked, son");

    /* Register assorted GLUT callback routines. */
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
  
    /* Enter GLUT's main loop; callback dispatching begins. */
    glutMainLoop();
    return 0;
}
