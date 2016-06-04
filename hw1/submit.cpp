/*****************************************************************************
FILE : submit.cpp (csci3260 2015-2016 Assignment 1)
NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
Student Information
Student ID: 
Student Name: 
*****************************************************************************/


#include "stdafx.h"
#include <iostream>
#include <gl\glut.h>

using namespace std;

int windowWidth = 600;
int windowHeight = 600;
float aspectRatio = windowWidth / (windowHeight / 1.0);

float moveSpeed = 2.0;
float eyeX = 50, eyeY = 50, eyeZ = 150;
float viewCenterX = 50, viewCenterY = 50, viewCenterZ = 50;
float viewAngleX = 0, viewAngleY = 1, viewAngleZ = 0;


void drawPlane(int width, int height)
{
	glNormal3f(0, 0, 1);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-width / 2, -height / 2, 0.0);
	glVertex3f(width / 2, -height / 2, 0.0);
	glVertex3f(-width / 2, height / 2, 0.0);
	glVertex3f(width / 2, height / 2, 0.0);
	glEnd();
}

void drawBox(int size = 20)
{

	glPushMatrix();
	glTranslated(0, 0, -size / 2);
	drawPlane(size, size);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-size / 2, 0, 0);
	glRotated(-90, 0, 1, 0);
	drawPlane(size, size);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -size / 2);
	drawPlane(size, size);
	glPopMatrix();

	glPushMatrix();
	glTranslated(size / 2, 0, 0);
	glRotated(-90, 0, 1, 0);
	drawPlane(size, size);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -size / 2, 0);
	glRotated(90, 1, 0, 0);
	drawPlane(size, size);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, size / 2, 0);
	glRotated(90, 1, 0, 0);
	drawPlane(size, size);
	glPopMatrix();
}

void cylindre(double h, double r, int n) {
	double *cs = (double *)calloc(n + 1, sizeof(double));
	double *sn = (double *)calloc(n + 1, sizeof(double));
	double *ncs = (double *)calloc(n + 1, sizeof(double));
	double *nsn = (double *)calloc(n + 1, sizeof(double));
	cs[0] = cs[n] = r;
	sn[0] = sn[n] = 0.0;
	ncs[0] = ncs[n] = 1.0;
	nsn[0] = nsn[n] = 0.0;
	for (int i = 1; i < n; i++) {
		double a = 3.14159 / n * 2 * i;
		ncs[i] = cos(a);
		nsn[i] = sin(a);
		cs[i] = r*ncs[i];
		sn[i] = r*nsn[i];
	}

	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	for (int i = 0; i < n; i++) {
		glVertex3d(cs[i], -h / 2, sn[i]);
	}
	glEnd();

	glBegin(GL_QUADS);
	for (int i = 0; i < n; i++) {
		glNormal3d(ncs[i], 0.0, nsn[i]);
		glVertex3d(cs[i], h / 2, sn[i]);
		glNormal3d(ncs[i + 1], 0.0, nsn[i + 1]);
		glVertex3d(cs[i + 1], h / 2, sn[i + 1]);
		glVertex3d(cs[i + 1], -h / 2, sn[i + 1]);
		glNormal3d(ncs[i], 0.0, nsn[i]);
		glVertex3d(cs[i], -h / 2, sn[i]);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	for (int i = 0; i < n; i++) {
		glVertex3d(cs[i], h / 2, sn[i]);
	}
	glEnd();

	free(sn);
	free(cs);
	free(nsn);
	free(ncs);
}

void drawTree(float radius, float height, float topRatio = 0.4)
{
	// lower
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.4, 0.4, 0.0, 1.0 };
	GLfloat mat_specular[] = { 0, 0, 0, 0 };
	GLfloat high_shininess[] = { 0.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);

	glPushMatrix();
	glTranslated(0, 0, height * (1 - topRatio) / 2);
	glRotated(90, 1, 0, 0);
	cylindre(height * (1 - topRatio), radius, 50);
	glPopMatrix();

	// upper
	GLfloat no_mat1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse1[] = { 0.0, 0.6, 0.1, 1.0 };
	GLfloat mat_specular1[] = { 0, 0, 0, 0 };
	GLfloat high_shininess1[] = { 0.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat1);

	glPushMatrix();
	glTranslated(0, 0, height * (1 - topRatio));
	glutSolidCone(radius*1.2, height*topRatio, 50, 50);
	glPopMatrix();
}

void drawTrees()
{

	glPushMatrix();
	glTranslated(10, 10, 0);
	drawTree(2, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslated(30, 10, 0);
	drawTree(5, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(10, 25, 0);
	drawTree(3, 25);
	glPopMatrix();
}

void drawHome()
{
	glPushMatrix();

	// material
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.6, 0.2, 0.2, 1.0 };
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat high_shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);

	glTranslated(80, 80, 6);
	drawBox(12);

	GLfloat no_mat1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse1[] = { 0.7, 0.7, 0.1, 1.0 };
	GLfloat mat_specular1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat high_shininess1[] = { 50.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat1);

	glPushMatrix();
	glTranslated(-6, 0, 6);
	glRotated(-60, 0, 1, 0);
	glTranslated(6, 0, 0);
	drawPlane(12, 12);
	glPopMatrix();

	glPushMatrix();
	glTranslated(6, 0, 6);
	glRotated(60, 0, 1, 0);
	glTranslated(-6, 0, 0);
	drawPlane(12, 12);
	glPopMatrix();

	glBegin(GL_TRIANGLES);
	glNormal3f(0, -1, 0);
	glVertex3f(-6, 6, 6);
	glVertex3f(0, 6, sin(60 * 3.14159 / 180) * 12 + 6);
	glVertex3f(6, 6, 6);
	glEnd();

	glBegin(GL_TRIANGLES);
	glNormal3f(0, -1, 0);
	glVertex3f(-6, -6, 6);
	glVertex3f(0, -6, sin(60 * 3.14159 / 180) * 12 + 6);
	glVertex3f(6, -6, 6);
	glEnd();

	glPushMatrix();
	glTranslated(0, -6.01, -2);
	glRotated(90, 1, 0, 0);
	drawPlane(3, 8);
	glPopMatrix();

	glPopMatrix();
}

void drawPool()
{

	// material
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.2, 0.2, 0.6, 1.0 };
	GLfloat mat_specular[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat high_shininess[] = { 128.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);

	glPushMatrix();
	glTranslated(50, 50, 0.5);
	glRotated(90, 1, 0, 0);
	cylindre(1, 20, 50);
	glPopMatrix();
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 10.0, 900.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, // eye
		viewCenterX, viewCenterY, viewCenterZ, // center
		viewAngleX, viewAngleY, viewAngleZ);

	// lighting
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 120.0, 0.0, 60.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse1[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_specular1[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_position1[] = { -20.0, -20.0, 60.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	GLfloat light_ambient2[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse2[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position2[] = { 50.0, 70.0, 30.0, 1.0 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	// floor
	glBegin(GL_TRIANGLE_STRIP);

	// material
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.6, 0.9, 0.6, 1.0 };
	GLfloat mat_specular[] = { 0, 0, 0, 1.0 };
	GLfloat high_shininess[] = { 2.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);
	glEnable(GL_COLOR_MATERIAL);

	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glVertex3f(100.0, 100.0, 0.0);
	glEnd();

	drawPool();
	drawTrees();
	drawHome();

	glutSwapBuffers();
	glFlush();
}

void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	cout << "key:";
	cout << key;
	cout << "\n";

	float moveSpeed = 2;
	if (key == 'a') {
		viewCenterX -= moveSpeed;
	}
	else if (key == 'd') {
		viewCenterX += moveSpeed;
	}
	else if (key == 'w') {
		viewCenterY += moveSpeed;
	}
	else if (key == 's') {
		viewCenterY -= moveSpeed;
	}
	else if (key == 'g') {
		eyeZ -= moveSpeed;
	}
	else if (key == 'h') {
		eyeZ += moveSpeed;
	}

	glutPostRedisplay();
}

void arrowKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		eyeX -= moveSpeed;
		viewCenterX -= moveSpeed;
		break;

	case GLUT_KEY_RIGHT:
		eyeX += moveSpeed;
		viewCenterX += moveSpeed;
		break;

	case GLUT_KEY_UP:
		eyeY += moveSpeed;
		viewCenterY += moveSpeed;
		break;

	case GLUT_KEY_DOWN:
		eyeY -= moveSpeed;
		viewCenterY -= moveSpeed;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	int mouseEdgeWidth = 50;
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	if (x < mouseEdgeWidth) {
		eyeX -= moveSpeed;
		viewCenterX -= moveSpeed;
	}
	if (width - x < mouseEdgeWidth) {
		eyeX += moveSpeed;
		viewCenterX += moveSpeed;
	}
	if (y < mouseEdgeWidth) {
		eyeY += moveSpeed;
		viewCenterY += moveSpeed;
	}
	if (height - y < mouseEdgeWidth) {
		eyeY -= moveSpeed;
		viewCenterY -= moveSpeed;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
	aspectRatio = windowWidth / (windowHeight / 1.0);
	glViewport(0, 0, w, h);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Assignment 1");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardCallback);
	glutPassiveMotionFunc(mouseMove);
	glutSpecialFunc(arrowKey);

	glutMainLoop();//enters the GLUT event processing loop.

	return 0;
}