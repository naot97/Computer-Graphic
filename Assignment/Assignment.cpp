
#include "stdafx.h";
#include<iostream>
#include "math.h";
#include "glut.h";
#include "Class.h";

using namespace std;

// thong tin man hinh
float widthWin = 600;
float heightWin = 600;

//thong tin camera
Camera cam(7,1,9);

// thong tin doi tuong
Mesh	base, table, xylanh;
float value = 0.2;
float hBase = 6;
float rBase1 = 4;
float rBase2 = 2;

float hXylanh = 4;
float rXylanh = rBase2 * 0.6;
float posXylanh = hBase * 0.8;

float wTable = ;//x 
float lTable = ;//z
float hTable = ;//y

// thong tin cach ve
boolean drawFrame = 1;
boolean drawColor = 0;


void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(4, 0, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 4, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 4);
	glEnd();
}

void drawBase(){
	glPushMatrix();
	base.Draw(drawFrame , drawColor);
	glPopMatrix(); 
}

void drawXylanh(){
	glPushMatrix();
	glTranslatef(0,posXylanh,0);
	xylanh.Draw(drawFrame , drawColor);
	glPopMatrix(); 
}

void drawTable(){
	glPushMatrix();
	glTranslatef(0,posXylanh + hXylanh,0);
	table.Draw(drawFrame , drawColor);
	glPopMatrix();
}

void myDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam.x, cam.y , cam.z, 0, 0, 0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glColor3f(0, 0, 1);
	glViewport(0,0,widthWin,heightWin);
	drawAxis();
	drawBase();
	drawXylanh();
	drawTable();

	 


	glFlush();
    glutSwapBuffers();
}


void myInit()
{
	float	fHalfSize = 20;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho ( -fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -1000,1000) ;
}

void changeView(unsigned char key, int x, int y){
	switch(key){
	case '-':
		cam.changeDis(-value);
		break;
	case '+':
		cam.changeDis(+value);
		break;
	}
	glutPostRedisplay();
}
void changeView2(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		cam.changeHeight(value);
		break;	
	case GLUT_KEY_DOWN:
		cam.changeHeight(-value);
		break;
	case GLUT_KEY_LEFT:
		cam.changeAngle(5*value);
		break;
	case GLUT_KEY_RIGHT:
		cam.changeAngle(-5*value);
		break;
	}
	glutPostRedisplay();
}
int _tmain(int argc, _TCHAR* argv[])
{

	glutInit(&argc, (char**)argv); //initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB | GLUT_DEPTH);//set the display mode
	glutInitWindowSize(widthWin,heightWin);
	
	glutInitWindowPosition(100, 100); // set window position on screen
	glutCreateWindow("Lab 2"); // open the screen window

	float hBaseArr[] = {hBase/3 , 0, 2*hBase/3};
	float rBaseArr[] = {rBase1,rBase1,rBase2,rBase2};
	float hXylanhArr[] = {hXylanh};
	float rXylanhArr[] = {rXylanh, rXylanh};

	base.CreateCylinder2(4, hBaseArr, rBaseArr);
	table.CreateRectangular(10,1,20);
	xylanh.CreateCylinder2(2, hXylanhArr, rXylanhArr);

	myInit();
	glutKeyboardFunc(changeView);
    glutDisplayFunc(myDisplay);
	glutSpecialFunc(changeView2);

	glutMainLoop();
	return 0;
}


//
