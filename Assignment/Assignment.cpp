
#include "stdafx.h";
#include<iostream>
#include "math.h";
#include "glut.h";
#include "Class.h";

using namespace std;
#pragma region var
#define PI			3.1415926
// thong tin man hinh
float widthWin = 600;
float heightWin = 600;

//thong tin camera
Camera cam(5,0,5);

// thong tin doi tuong
Mesh	base, table, xylanh, rotor;
Mesh	circle, bar;
Mesh	slider;
Mesh latch;
Mesh shelf;
float value = 0.5;
float hBase = 6;
float rBase1 = 3;
float rBase2 = 1.5;

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


//shelf1 and shelf2

float rShelf2 = rSlider2;
float rShelf1 = rSlider1;
float lShelf = 0.8*hRotor;
float hShelf = 1;
float hShelfBase = 0.2*hRotor;
//di chuyen 
float angleBase = 0, angleXylanh = 0, angleRotor = -90;
float tranXylanh = 0;
float rTurn = wBar /2 ;
float xTranRotor= rTurn * cos(3.1415926 * angleRotor / 180 );
float zTranRotor= rTurn * sin(3.1415926 * angleRotor / 180);
// thong tin cach ve
boolean drawFrame = 0;
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
	glRotatef(angleBase,0,1,0);
	glPushMatrix();
	base.Draw(drawFrame , drawColor);
	glPopMatrix(); 
}

void drawXylanh(){
	glRotatef(angleXylanh,0,1,0);
	glTranslatef(0, tranXylanh, 0);
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
	glRotatef(angleRotor,0,1,0);
	glPushMatrix();
	glTranslatef(0,posXylanh + hXylanh + hTable,0);
	rotor.Draw(drawFrame , drawColor);
	glPopMatrix();
}

void drawOval(){
	glPushMatrix();

		glTranslatef(0,posXylanh + hXylanh + hTable + hRotor,0);

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
		glTranslatef(0,posXylanh + hXylanh + hTable + hRotor, 0);
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
		glTranslatef(xTranRotor, 0, 0);
		glTranslatef(0,posXylanh + hXylanh + hTable + hRotor, 0);
		latch.Draw(drawFrame , drawColor);
	glPopMatrix();
}


void drawShelfs(){
	glPushMatrix();
		glTranslatef(0,posXylanh + hXylanh + hTable + rShelf1 + hShelfBase + lShelf,0);
		glPushMatrix();
			glTranslatef(0,0,lTable * 0.35);
			glRotatef(-90,1,0,0);
			shelf.Draw(drawFrame , drawColor);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0,0, -lTable * 0.35);
			glRotatef(180,0,0,1);
			glRotatef(90,1,0,0);
			shelf.Draw(drawFrame , drawColor);
		glPopMatrix();
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
	drawShelfs(); 

	
	glPushMatrix();
		glTranslatef(0,0,zTranRotor);
		drawOval();
		drawSliders();
		drawLatch();
	glPopMatrix();

	drawRotor();
	glFlush();
    glutSwapBuffers();
}



void myInit()
{
	float	fHalfSize = 15;

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
	case '1':
		angleBase += 5 * value;
		break;
	case '2' :
		angleBase -= 5 * value;
		break;
	case '3':
		angleXylanh += 5 * value;
		break;
	case '4' :
		angleXylanh -= 5 * value;
		break;
	case '5':
		if (posXylanh + tranXylanh + value < hBase) tranXylanh += value;
		break;
	case '6':
		if (posXylanh + hXylanh + tranXylanh - value > hBase) tranXylanh -= value;
		break;
	case '7':
		angleRotor += 5 * value;
		break;
	case '8':
		angleRotor -= 5 * value;
		break;
	case 'w':
	case 'W' :
		drawFrame = !drawFrame;
		drawColor = !drawColor;
		break;

	}

	xTranRotor= rTurn * cos(3.1415926 * angleRotor / 180);
	zTranRotor= rTurn * sin(3.1415926 * angleRotor / 180);
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

	base.CreateCylinder(4, hBaseArr, rBaseArr, 0);
	table.CreateRectangular(wTable,hTable,lTable, 1);
	xylanh.CreateCylinder(2, hXylanhArr, rXylanhArr, 2);
	rotor.CreateCylinder(2, hRotorArr, rRotorArr, 3);
	circle.CreateCircle(rCircle1, rCircle2, hCircle, 4);
	bar.CreateRectangular(wBar, hBar, lBar, 4);
	slider.CreateCylinder(4, hSliderArr, rSliderArr,5);
	latch.CreateCylinder(3, hLatchArr, rLatchArr,6);
	shelf.CreateCircle2(rShelf1, rShelf2, lShelf, hShelf, 0.5, 7);

	myInit();
	glutKeyboardFunc(changeView);
    glutDisplayFunc(myDisplay);
	glutSpecialFunc(changeView2);
	
	glutMainLoop();
	return 0;
}


//
