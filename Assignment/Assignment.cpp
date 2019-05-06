
#include "stdafx.h";
#include<iostream>
#include "math.h";
#include "glut.h";
#include "Class.h";

using namespace std;
#pragma region var
// thong tin man hinh
float widthWin = 600;
float heightWin = 600;

//thong tin camera
Camera cam(2,0,2);

// thong tin doi tuong
Mesh	base, table, xylanh, rotor;
Mesh	circle, bar;
Mesh	slider;
Mesh latch;
Mesh shelf, shelfCircle, sBar;
float value = 0.2;
float hBase = 6;
float rBase1 = 4;
float rBase2 = 2;

float hXylanh = 4;
float rXylanh = rBase2 * 0.6;
float posXylanh = hBase * 0.8;

float wTable = 10;//x 
float lTable = 20;//z
float hTable = 1;//y

float rRotor = 3.5;
float hRotor = 1;


float rCircle1 = 1;
float rCircle2 = 0.5;
float hCircle = 1;
float lBar = rCircle1 - rCircle2;//z
float wBar = 3.5;//x
float hBar = hCircle;//y


// slider

float rSlider1 = 0.5 * hBar, rSlider2 = 0.5 * rSlider1;
float hSlider1 = 1, hSlider2 = 1,hSlider3 = 5;

//latch

float rLatch1 = rCircle2, rLatch2 = 0.5 * rLatch1;
float hLatch1 = 1.2 *  hBar , hLatch2  = 0.2 * hLatch1;
float rTurn = 0.5 * rRotor;

//shelf1 and shelf2

float wShelf = 1;//x
float lShelf = 4;//z
float hShelf = 0.1*(hRotor + hBar);//y

float rShelfCircle1 = lShelf / 4;
float rShelfCircle2 = 0.5 * rShelfCircle1;
float lShelfCircle = 0.9*(hRotor + hBar) - rShelfCircle1;
float hShelfCircle = 1;

float lShelfBar = hShelfCircle;
float wShelfBar = rShelfCircle1 - rShelfCircle2;
float hShelfBar = lShelfCircle + rShelfCircle2;

// thong tin cach ve
boolean drawFrame = 1;
boolean drawColor = 1;
#pragma endregion 

void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(40, 0, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 40, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 40);
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
	glTranslatef(0,posXylanh + hXylanh + hCircle / 2,0);
	table.Draw(drawFrame , drawColor);
	glPopMatrix();
}

void drawRotor(){
	glPushMatrix();
	glTranslatef(0,posXylanh + hXylanh + hTable,0);
	rotor.Draw(drawFrame , drawColor);
	glPopMatrix();
}

void drawOval(){
	glPushMatrix();

	glTranslatef(0,posXylanh + hXylanh + hTable + hRotor,-rTurn);

	glPushMatrix();
	glTranslatef(wBar / 2,0, 0);
	glRotatef(90,0,1,0);
	circle.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-wBar / 2,0, 0);
	glRotatef(-90,0,1,0);
	circle.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,hBar/2, -lBar/2 -rCircle2);
	bar.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,hBar/2, lBar/2 + rCircle2);
	bar.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPopMatrix();
}

void drawSliders(){
	glPushMatrix();
	glTranslatef(0,posXylanh + hXylanh + hTable + hRotor, -rTurn);
	glTranslatef(0,hBar*0.5 ,0);
	
	glPushMatrix();
	glTranslatef(0, 0 , rCircle2 + lBar);
	glRotatef(90,1,0,0);
	slider.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0 , - rCircle2 - lBar);
	glRotatef(-90,1,0,0);
	slider.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPopMatrix();
}

void drawLatch(){
	glPushMatrix();
	glTranslatef(0,posXylanh + hXylanh + hTable + hRotor, -rTurn);
	latch.Draw(drawFrame , drawColor);
	glPopMatrix();
}

void drawSubShelf(){
	glPushMatrix();
	glPushMatrix();
	glTranslatef(+rShelfCircle2 + wShelfBar/2 ,-hShelfBar/2,-lShelfBar/2);
	sBar.Draw(drawFrame , drawColor);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-rShelfCircle2 - wShelfBar/2,-hShelfBar/2,-lShelfBar/2);
	sBar.Draw(drawFrame , drawColor);
	glPopMatrix();
	
	glPushMatrix();
	//glTranslatef(0,0 , - rShelfCircle2 - lShelfCircle);
	//shelf.Draw(drawFrame , drawColor);
	glRotatef(-90,1,0,0);
	shelfCircle.Draw(drawFrame , drawColor);
	glPopMatrix();
	glPopMatrix();
	
}

void drawShelf(){
	glPushMatrix();
	drawSubShelf();
	glPopMatrix();

	glPushMatrix();
	drawSubShelf();
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
	/*drawBase();
	drawXylanh();
	drawTable();
	drawRotor();*/
	//drawOval();
	//drawSliders();
	//drawLatch();
	drawShelf(); 


	glFlush();
    glutSwapBuffers();
}


void myInit()
{
	float	fHalfSize = 10;

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
	glutCreateWindow("Assignment"); // open the screen window

	float hBaseArr[] = {hBase/3 , 0, 2*hBase/3};
	float rBaseArr[] = {rBase1,rBase1,rBase2,rBase2};
	float hXylanhArr[] = {hXylanh};
	float rXylanhArr[] = {rXylanh, rXylanh};
	float hRotorArr[] = {hRotor};
	float rRotorArr[] = {rRotor, rRotor};
	float hSliderArr[] = {hSlider1, hSlider2, hSlider3};
	float rSliderArr[] = {rSlider1, rSlider1, rSlider2, rSlider2};
	float hLatchArr[] = {hLatch1, hLatch2};
	float rLatchArr[] = {rLatch1, rLatch1, rLatch2};

	base.CreateCylinder2(4, hBaseArr, rBaseArr);
	table.CreateRectangular(wTable,hTable,lTable);
	xylanh.CreateCylinder2(2, hXylanhArr, rXylanhArr);
	rotor.CreateCylinder2(2,hRotorArr, rRotorArr);
	circle.CreateCircle(rCircle1, rCircle2, hCircle);
	//circle2.CreateCircle(rCircle1, rCircle2, hCircle);
	bar.CreateRectangular(wBar, hBar, lBar);
	//bar2.CreateRectangular(wBar, hBar, lBar);
	slider.CreateCylinder2(4, hSliderArr, rSliderArr);
	latch.CreateCylinder2(3, hLatchArr, rLatchArr);
	shelfCircle.CreateCircle2(rShelfCircle1, rShelfCircle2, lShelfCircle, hShelfCircle);
	shelf.CreateRectangular(wShelf, hShelf, lShelf);
	sBar.CreateRectangular(wShelfBar, hShelfBar, lShelfBar);

	myInit();
	glutKeyboardFunc(changeView);
    glutDisplayFunc(myDisplay);
	glutSpecialFunc(changeView2);
	
	glutMainLoop();
	return 0;
}


//
