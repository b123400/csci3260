// Ass2.cpp : Defines the entry point for the console application.
// Student ID: 
// Student Name: 

#include "stdafx.h"
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bitmap.h"
#include <chrono>
#include <iostream>


using namespace std::chrono;
using namespace std;

float winwidth, winheight;


double wheelRotation = 0;
double carVelocity = 0;
double carRotation = 0;
double carAcceleation = 0;
double carX = 50;
double carY = 50;
double wheelTextureRotate = 0;


/* Add code here to define variable  
*/

//Load the BMP file
GLubyte* TextureLoadBitmap(char *filename, int *w, int *h)		/* I - Bitmap file to load */
{
	BITMAPINFO	*info;				/* Bitmap information */
	void		*bits;				/* Bitmap pixel bits */
	GLubyte	*rgb;				/* Bitmap RGB pixels */
	GLubyte   err = '0';

	/*
	* Try loading the bitmap and converting it to RGB...
	*/

	bits = LoadDIBitmap(filename, &info);
	if(bits==NULL) 
		return(NULL);
	rgb = ConvertRGB(info, bits);
	if (rgb == NULL)
	{
		free(info);
		free(bits);
	};

	printf("%s: %d %d\n", filename, info->bmiHeader.biWidth, info->bmiHeader.biHeight);
	printf("read %s successfully\n", filename);
	*w = info->bmiHeader.biWidth;
	*h = info->bmiHeader.biHeight;

	/*
	* Free the bitmap and RGB images, then return 0 (no errors).
	*/

	free(info);
	free(bits);
	return (rgb);

}

GLuint texture;
GLubyte* image;

GLuint wheelTexture;
GLubyte* wheelImage;

GLuint carTexture;
GLubyte* carImage;

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wheelTexture);
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	for (int i = 0; i < n; i++) {
		glTexCoord2f(0.5f * (float)ncs[i] + 0.5f, 0.5f * (float)nsn[i] + 0.5f);
		glVertex3d(cs[i], h / 2, sn[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, wheelTexture);
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	for (int i = 0; i < n; i++) {
		glTexCoord2f(0.5 * (float)ncs[i] + 0.5, 0.5 * (float)nsn[i] + 0.5); 
		glVertex3d(cs[i], -h / 2, sn[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	free(sn);
	free(cs);
	free(nsn);
	free(ncs);
}

void drawPlane(int width, int height)
{
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-width / 2, -height / 2, 0.0);
	glTexCoord2f(1, 0);
	glVertex3f(width / 2, -height / 2, 0.0);
	glTexCoord2f(1, 1);
	glVertex3f(width / 2, height / 2, 0.0);
	glTexCoord2f(0, 1);
	glVertex3f(-width / 2, height / 2, 0.0);
	glEnd();
}

void drawBox(int width, int height, int depth)
{

	glPushMatrix();
	glTranslated(0, 0, -depth / 2);
	drawPlane(width, height);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-width / 2, 0, 0);
	glRotated(-90, 0, 1, 0);
	drawPlane(depth, height);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, depth / 2);
	drawPlane(width, height);
	glPopMatrix();

	glPushMatrix();
	glTranslated(width / 2, 0, 0);
	glRotated(90, 0, 1, 0);
	drawPlane(depth, height);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -height / 2, 0);
	glRotated(90, 1, 0, 0);
	drawPlane(width, depth);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, height / 2, 0);
	glRotated(-90, 1, 0, 0);
	drawPlane(width, depth);
	glPopMatrix();
}

int windowWidth = 800;
int windowHeight = 600;
float aspectRatio = windowWidth / (windowHeight / 1.0);

float moveSpeed = 2.0;
float eyeX = 100, eyeY = 40, eyeZ = 100;
float viewCenterX = 50, viewCenterY = 50, viewCenterZ = 0;
float viewAngleX = 0, viewAngleY = 1, viewAngleZ = 0;

void init()
{
	// Clear Screen color
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0,0,0,0);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Texture mapping setting for Microsoft's OpenGL implementation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	// Texture mapping parameters for filter and repeatance
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/* Add code here to initialize lighting, read bitmap images, initialize different GLU geometry
	* use glLight, TextureLoadBitmap, gluNewQuadric...etc
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 100.0, 60.0, 60.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	int image_w;
	int image_h;
	//read image
	image = TextureLoadBitmap("ground.bmp", &image_w, &image_h);
	// allocate a texture name
	glGenTextures(1, &texture);
	// select our current texture
	glBindTexture(GL_TEXTURE_2D, texture);
	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// when texture area is small, bilinear filter the original
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// when texture area is large, bilinear filter the original
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// the texture wraps over at the edges (repeat)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// assign image to texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//read image
	wheelImage = TextureLoadBitmap("wheel.bmp", &image_w, &image_h);
	// allocate a texture name
	glGenTextures(1, &wheelTexture);
	// select our current texture
	glBindTexture(GL_TEXTURE_2D, wheelTexture);
	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// when texture area is small, bilinear filter the original
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// when texture area is large, bilinear filter the original
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// the texture wraps over at the edges (repeat)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// assign image to texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, wheelImage);

	//read image
	carImage = TextureLoadBitmap("car.bmp", &image_w, &image_h);
	// allocate a texture name
	glGenTextures(1, &carTexture);
	// select our current texture
	glBindTexture(GL_TEXTURE_2D, carTexture);
	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// when texture area is small, bilinear filter the original
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// when texture area is large, bilinear filter the original
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// the texture wraps over at the edges (repeat)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// assign image to texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGB, GL_UNSIGNED_BYTE, carImage);

}


void drawFloor() {
	// floor
	// enable texture
	glEnable(GL_TEXTURE_2D);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, texture);
	// draw a square and assign texture coordinates to each vertex
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 0.0, 0.0);
	glTexCoord2f(1.0, 2.0); glVertex3f(100.0, 100.0, 0.0);
	glTexCoord2f(0.0, 2.0); glVertex3f(0.0, 100.0, 0.0);

	glEnd();
	// disable texture
	glDisable(GL_TEXTURE_2D);
}

float wheelRadius = 2;

void drawCar() {
	float totalWidth = 10;
	float totalHeight = 10;
	float bodyWidth = 6;
	float wheelWidth = (totalWidth - bodyWidth) / 2;
	float frontWheelY = (totalHeight / 2) - wheelRadius;
	float rearWheelY = -frontWheelY;
	float leftWheelX = -totalWidth / 2 + wheelWidth / 2;
	float rightWheelX = -leftWheelX;

	glPushMatrix();
	glRotated(carRotation, 0, 0, 1.0f);

	glPushMatrix();
	glTranslated(leftWheelX, frontWheelY, wheelRadius);
	glRotated(90, 0, 0, 1.0f);
	glRotated(wheelRotation, 0, 0, 1.0);
	glRotated(wheelTextureRotate, 0, 1.0, 0);
	cylindre(wheelWidth, wheelRadius, 50);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(rightWheelX, frontWheelY, wheelRadius);
	glRotated(90, 0, 0, 1.0f);
	glRotated(wheelRotation, 0, 0, 1.0f);
	glRotated(wheelTextureRotate, 0, 1.0, 0);
	cylindre(wheelWidth, wheelRadius, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(leftWheelX, rearWheelY, wheelRadius);
	glRotated(90, 0, 0, 1.0f);
	glRotated(wheelTextureRotate, 0, 1.0, 0);
	cylindre(wheelWidth, wheelRadius, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslated(rightWheelX, rearWheelY, wheelRadius);
	glRotated(90, 0, 0, 1.0f);
	glRotated(wheelTextureRotate, 0, 1.0, 0);
	cylindre(wheelWidth, wheelRadius, 50);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	// bind texture
	glBindTexture(GL_TEXTURE_2D, carTexture);
	glPushMatrix();
	glTranslated(0, 0, wheelRadius*2);
	drawBox(bodyWidth, totalHeight, wheelRadius*2);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void display(void)
{

	/* clear the window color before drawing is performed */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	/* Add code here to transform the car and the ground & draw them
	* use glTranslate, glRotate, glLoadIdentity, glPushMatrix, glPopMatrix, glMaterial, 
	* glBegin, gluSphere...etc
	*
	* Add code for Texture Mapping for the car and the ground
	* use glTexImage2D..
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 10.0, 900.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(carX, carY - 50, 50, // eye
		carX, carY, 0, // center
		viewAngleX, viewAngleY, viewAngleZ);
	
	drawFloor();

	glPushMatrix();
	glTranslated(carX, carY, 0);
	drawCar();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();

}


void reshape(GLsizei w,GLsizei h)
{
	winwidth  = w;
	winheight = h;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(GLfloat)w/(GLfloat)h,1.0,40.0); 
}

/* keyboard up CALLBACK for handling special keyboard up event */
void keyspecialup(int key, int x, int y)
{

	/* Add code here to control the animation interactively */

	switch (key)
	{
		// car control
		case GLUT_KEY_UP: // move front
		case GLUT_KEY_DOWN: // move back
			carAcceleation = 0;
			break;
		case GLUT_KEY_LEFT: // move left
		case GLUT_KEY_RIGHT: // move right
			wheelRotation = 0;
			break;
	}
}

/* keyboard CALLBACK for handling special keyboard event */
void keyspecial(int key, int x, int y)
{

	/* Add code here to control the animation interactively */

	switch (key)
	{
		// car control
		case GLUT_KEY_UP: // move front
			carAcceleation = 10;
			break;
		case GLUT_KEY_DOWN: // move back
			carAcceleation = -10;
			break;
		case GLUT_KEY_LEFT: // move left
			wheelRotation = 30;
			break;
		case GLUT_KEY_RIGHT: // move right
			wheelRotation = -30;
			break;
	}
}

/* keyboard CALLBACK for handling keyboard event */
void keyboard(unsigned char key, int x, int y)
{
	/* Add code here to control the animation interactively */

	switch (key)
	{
		case 'r': // Reset all parameters
			carAcceleation = 0;
			carRotation = 0;
			wheelRotation = 0;
			wheelTextureRotate = 0;
			carX = 50;
			carY = 50;
			carVelocity = 0;

			break;

	}

}

void mouse(int button, int state, int ix, int iy)
{
	
}




void drag(int ix, int iy)
{

}

bool msInitialized = false;
milliseconds lastMs;

void timer(int t)
{
	/* Add code here to update the velocity, acceleration, position and rotation of car and wheels */
	auto shit = system_clock::now().time_since_epoch();
	milliseconds ms = duration_cast< milliseconds >(shit);
	if (msInitialized) {
		double timePassed = duration_cast<milliseconds>(ms - lastMs).count()/1000.0;
		carVelocity += timePassed * carAcceleation;
		carVelocity = max(-20, min(20, carVelocity));
		if (carVelocity != 0) {
			// friction
			if (carVelocity > 0) {
				carVelocity -= timePassed * 3;
			}
			else {
				carVelocity += timePassed * 3;
			}
		}
		if (carVelocity > 1){
			if (wheelRotation < 0){
				carRotation -= 5;
			}
			else if (wheelRotation > 0) {
				carRotation += 5;
			}
		}
		else if (carVelocity < -1) {
			if (wheelRotation < 0){
				carRotation += 5;
			}
			else if (wheelRotation > 0) {
				carRotation -= 5;
			}
		}
		double distance = timePassed * carVelocity;
		carX += distance * sin(-carRotation / 180 * 3.14159);
		carY += distance * cos(-carRotation / 180 * 3.14159);
		carX = min(100, max(0, carX));
		carY = min(100, max(0, carY));

		wheelTextureRotate += distance / 3.14159 * 180;

		cout << "velocity: " << carVelocity;
		cout << "X:" << carX << " Y:" << carY << "\n";
	}
	msInitialized = true;
	lastMs = ms;

	// display after update and reset timer
	glutPostRedisplay();
    glutTimerFunc(10, timer, 1);
}

void main(int argc, char** argv)
{

	/*Initialization of GLUT Library */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 100);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);

	/*Create a window with title specified */
	glutCreateWindow("Assignment 2");

	/*Register different CALLBACK function for GLUT to response 
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyspecial);
	glutSpecialUpFunc(keyspecialup);
	// call timer function every 10 ms
	glutTimerFunc(10, timer, 1);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	init();	/*not GLUT call, initialize several parameters */

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	
}


