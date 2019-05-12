
#include "stdafx.h";
#include <iostream>;
#include "math.h";
#include "glut.h";
#include <string>;
using namespace std;

#pragma region library
class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}
	void set(Point3& p)
						{ x = p.x; y = p.y; z = p.z;}
	Point3() { x = y = z = 0;}
	Point3(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}

};

class Color3
{
public:
	float r, g, b;
	void set(float red, float green, float blue)
						{ r = red; g = green; b = blue;}
	void set(Color3& c)
						{ r = c.r; g = c.g; b = c.b;}
	Color3() { r = g = b = 0;}
	Color3(float red, float green, float blue)
						{ r = red; g = green; b = blue;}

};

class Vector3
{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}
	void set(Vector3& v)
						{ x = v.x; y = v.y; z = v.z;}
	void flip()
						{ x = -x; y = -y; z = -z;}
	void normalize();
	Vector3() { x = y = z = 0;}
	Vector3(float dx, float dy, float dz)
						{ x = dx; y = dy; z = dz;}
	Vector3(Vector3& v)
						{ x = v.x; y = v.y; z = v.z;}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};

class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
	void set(int id, int color){
		vertIndex = id;
		colorIndex = color;
	}
};

class Face
{
public:
	int		nVerts;
	VertexID*	vert;
	Vector3		facenorm;
	Face()
	{
		nVerts	= 0;
		vert	= NULL;
	}
	~Face()
	{
		if(vert !=NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}

	void set(int numVert){
		nVerts = numVert;
		vert = new VertexID[numVert]; 
	}

};

class Mesh
{
public:
	//int n;
	int		numVerts;
	Point3*		pt;
	
	int		numFaces;
	Face*		face;
	int color;
public:
	Mesh()
	{
		numVerts	= 0;
		pt		= NULL;
		numFaces	= 0;
		face		= NULL;
		//n = 72;
	}
	Mesh(int c)
	{
		numVerts	= 0;
		pt		= NULL;
		numFaces	= 0;
		face		= NULL;
		//n = 72;
		color = c;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}	
		if(face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void Draw(bool hasFrame, bool hasColor);
	void DrawWireframe();
	void DrawColor();
	void DrawFloor();

	void CreateRectangular(float length, float width, float height);
	void CreateCylinder(int face, float * h, float * r , int n);
	void CreateCircle( float r1, float r2, float h, int color);
	void CreateCircle2(float r1, float r2, float w, float h, float hBase, int n);
	void CreateSquare(float x, float z, float a, int color);

	void AddRectangular(int countPoint, int& countFace);
	void AddCircle(int &countPoint, int& countFace,float r1, float r2, float h, int n);
	void CalculateFacesNorm();
	void NormNewell(int facei);
};

#define PI			3.1415926
#define	COLORNUM		14
float	ColorArr[COLORNUM][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, 1.0}, 
								{1.0, 1.0,  0.0}, { 198.0/255, 51.0/255, 51.0/255},{ 0.0, 1.0, 1.0}, 
								 {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.5, 1.0, 0.5},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};

GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};

GLfloat shine = 100;

Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
	return c;
};

float Vector3::dot(Vector3 b)
{
	return x*b.x + y*b.y + z*b.z;
};

void Vector3::normalize()
{
	float temp = sqrt(x*x + y*y + z*z);
	x = x/temp;
	y = y/temp;
	z = z/temp;
};
									
void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int	iv = face[f].vert[v].vertIndex;
			int	ic = face[f].vert[v].colorIndex;

			float diff[] = {ColorArr[ic][0],ColorArr[ic][1],ColorArr[ic][2],1};

			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, shine);
			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;
			float  diff[4] = {ColorArr[ic][0],ColorArr[ic][1],ColorArr[ic][2],1};

			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, shine);

			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawFloor(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;
			float  diff[4] = {ColorArr[ic][0],ColorArr[ic][1],ColorArr[ic][2],1};

			GLfloat ambient[] = {0, 0, 0, 1.0};
			GLfloat specular[] = {0, 0.0, 0.0, 1.0};
			GLfloat shine = 100;

			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, shine);

			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::Draw(bool hasFrame, bool hasColor){
	if (hasFrame) this->DrawWireframe();
	if (hasColor) this->DrawColor();
}

void Mesh::CreateCylinder(int f, float h[], float r[], int n){
	numFaces = 2*(n + 1) + n*(f-1);
	face = new Face[numFaces];
	numVerts = 2 + (n + 1) * f;
	pt = new Point3[numVerts];

	float * sumH = new float[f];
	
	for (int i = 0; i < f; i++){
		if (i == 0){
			sumH[i] = 0;
		}
		else{
			sumH[i] = sumH[i-1] + h[i - 1];
		}
	}
	
	pt[0].set(0,0,0);
	pt[1].set(0,sumH[f-1],0);
	int countFace = -1;
	int countP = 1;
	for (int i = 0; i <= n ; i++){
		float angle = 2 * PI * i * (1.0/n);
		
		for (int j = 0; j < f  ; j++){
			float x1 = r[j] * cos(angle);
			float z1 = r[j] * sin(angle);
	
			this->pt[++countP].set(x1, sumH[j], z1);

			if (j > 0 && i > 0){
				face[++countFace].set(4);
				face[countFace].vert[0].set(countP - f - 1, color);
				face[countFace].vert[1].set(countP - 1, color);
				face[countFace].vert[2].set(countP , color);
				face[countFace].vert[3].set(countP - f, color);
			}

			if( (j == 0 || j  == f - 1) && i > 0){
				int k = (j == 0) ? 0: 1;
				face[++countFace].set(3);
				face[countFace].vert[0].set(k,color);
				face[countFace].vert[1].set(countP - f,color);
				face[countFace].vert[2].set(countP, color);
			}
		}
	}


	CalculateFacesNorm();
}

void Mesh::CreateRectangular(float w, float l, float h){
	l = l/2;
	w = w/2;
	h = h/2;

	numVerts=8;
	pt = new Point3[numVerts];
	pt[0].set( w, l, h);
	pt[1].set( -w, l, h);
	pt[2].set( -w, l, -h);
	pt[3].set(w, l, -h);
	pt[4].set(w, -l, h);
	pt[5].set(-w, -l, h);
	pt[6].set( -w, -l, -h);
	pt[7].set(w, -l, -h);

	numFaces= 6;
	face = new Face[numFaces];
	
	int countFace = -1;
	AddRectangular(0,countFace);
	CalculateFacesNorm();
}

void Mesh::CreateCircle( float r1, float r2, float h, int n){
	if (r1 < r2) 
		return;
	numFaces = 6 * n /2;
	face = new Face[numFaces];
	numVerts = 4*(n/2 + 1);
	pt = new Point3[numVerts];

	int countP = -1;
	int countFace = -1;
	this->AddCircle(countP,countFace,r1,r2,h,n);
	CalculateFacesNorm();
}

void Mesh::CreateCircle2(float r1, float r2, float l, float h, float hBase, int n){
	if (r1 < r2) 
		return;
	numFaces = 6*(n-2) + 6*3;
	face = new Face[numFaces];
	numVerts = 4 * n + 8*3;
	pt = new Point3[numVerts];
	int countFace = -1;
	int countP = -1;
	
	this->AddCircle(countP,countFace,r1,r2,h,n);
	int a  = 0 ;
	for (int i= (n/2) +1 ; i < n; i++){
		float angle = 2 * PI * i * (1.0/n);
		float x = r2 * cos(angle);
		float z = r2 * sin(angle);

		pt[++countP].set(x,0,z);
		pt[++countP].set(x,0, -r2 -l);
		pt[++countP].set(x,h, -r2 -l);
		pt[++countP].set(x,h,z);
		if (i > (n/2) +1) 
			AddRectangular(countP-7,countFace);
		
	}
	pt[++countP].set(-2*r1,0,-r2 -l);
	pt[++countP].set(-2*r1, 0,-r2 -l - hBase);
	pt[++countP].set(-2*r1, h,-r2 -l - hBase);
	pt[++countP].set(-2*r1,h,-r2 -l);

	pt[++countP].set(2*r1,0,-r2 -l);
	pt[++countP].set(2*r1, 0,-r2 -l - hBase);
	pt[++countP].set(2*r1, h,-r2 -l - hBase);
	pt[++countP].set(2*r1,h,-r2 -l);
	
	
	AddRectangular(countP - 7, countFace);

	//
	pt[++countP].set(-r1,0,0);
	pt[++countP].set(-r1, 0,-r2 -l);
	pt[++countP].set(-r1, h,-r2 -l );
	pt[++countP].set(-r1,h,0);

	pt[++countP].set(-r2,0,0);
	pt[++countP].set(-r2, 0,-r2 -l);
	pt[++countP].set(-r2, h,-r2 -l );
	pt[++countP].set(-r2,h,0);
	AddRectangular(countP - 7, countFace);
	//
	pt[++countP].set(r2,0,0);
	pt[++countP].set(r2, 0,-r2 -l);
	pt[++countP].set(r2, h,-r2 -l );
	pt[++countP].set(r2,h,0);

	pt[++countP].set(r1,0,0);
	pt[++countP].set(r1, 0,-r2 -l);
	pt[++countP].set(r1, h,-r2 -l );
	pt[++countP].set(r1,h,0);
	AddRectangular(countP - 7, countFace);
	//
	CalculateFacesNorm();
}

void Mesh::CreateSquare(float x, float z, float a, int color){
	numFaces = 1;
	face = new Face[numFaces];
	numVerts = 4;
	pt = new Point3[numVerts];

	pt[0].set(x,0,z);
	pt[1].set(x+a,0,z);
	pt[2].set(x+a,0,z+a);
	pt[3].set(x,0,z+a);
	

	face[0].set(4);
	for (int i =0 ; i<4; i++)
		face[0].vert[i].set(i,color);

	CalculateFacesNorm();
}

void Mesh::AddCircle(int &countP, int& countFace,float r1, float r2, float h, int n){
	for (int i = 0; i <= int(n/2) ; i++){
		float angle = 2 * PI * i * (1.0/n);
		float x1 = r1 * cos(angle);
		float z1 = r1 * sin(angle);
		float x2 = r2 * cos(angle);
		float z2 = r2 * sin(angle);
		
		
		pt[++countP].set(x2,0,z2);
		pt[++countP].set(x1,0,z1);
		pt[++countP].set(x1,h,z1);
		pt[++countP].set(x2,h,z2);

		if (i > 0) 
			AddRectangular(countP-7,countFace);
	}
}

void Mesh::AddRectangular(int countPoint, int& countFace){
	int recArr[6][4] = {{1,5,6,2},{0,3,7,4},{0,1,2,3},{7,6,5,4},{4,5,1,0},{3,2,6,7}};
	for (int j = 0 ; j < 6; j++){
		face[++countFace].set(4);
		for (int k =0; k < 4; k++ )
			face[countFace].vert[k].set(countPoint + recArr[j][k], color);
	}
}

void  Mesh::CalculateFacesNorm(){
	for(int i = 0; i < numFaces; i++)
		NormNewell(i);
}
void Mesh::NormNewell(int facei){
	Vector3 m  = Vector3(0,0,0);
	Point3  pa,pb;
	for (int i = 0; i < face[facei].nVerts ; i++)
	{
		int j = (i + 1) % face[facei].nVerts;
		VertexID * vert = face[facei].vert;
		
		pa = pt[vert[i].vertIndex];
		pb = pt[vert[j].vertIndex];
		m.x += (pa.y - pb.y)*(pa.z + pb.z);
		m.y += (pa.z - pb.z)*(pa.x + pb.x);
		m.z += (pa.x - pb.x)*(pa.y + pb.y);
	}
	m.normalize();
	m.x = -m.x;m.y = -m.y;m.z = -m.z;
	face[facei].facenorm = m;
}
#pragma endregion library
#pragma region var
#define PI			3.1415926
// thong tin man hinh
float widthWin = 600;
float heightWin = 600;

//thong tin camera

float	cameraAngle = 42;
float	cameraHeight = 15;
float	cameraDis = 24;
float cameraX,cameraY,cameraZ;


// thong tin doi tuong
Mesh	base1(0),base2(0), table(1), xylanh(2), rotor(4);
Mesh	circle(3), bar(3),slider(3);
Mesh latch(4);
Mesh shelf(7);
Mesh brick(11);

// thong tin ve
float value = 0.3;
float hBase = 4;
float hBase1 = 0.2 *hBase;
float hBase2 = 0.8 * hBase;
float rBase1 = 2;
float rBase2 = 1.5;

float hXylanh = 3;
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

// dieu khien
bool automation = false;
bool isEnableLight1 = false;
// thong tin cach ve
bool drawFrame = 0;
bool drawColor = 1;
#pragma endregion 

#pragma region main
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
	glTranslatef(0, hBase1, 0);
	base2.Draw(drawFrame , drawColor);
	glPopMatrix(); 
	glPushMatrix();
	base1.Draw(drawFrame , drawColor);
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
	glTranslatef(0,posXylanh + hXylanh + hCircle / 2 - 0.1,0);
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

void drawFloor(){
	for (int i = 0; i < 20; i++){
		float x = -40 + i*8;
		float z0 = -40;
		for (int j = 0; j < 20; j++){
			float z = z0 + j*8;
			if ((i + j) % 2 == 0)
				brick.CreateSquare(x,z,8,11);
			else
				brick.CreateSquare(x,z,8,13);
			brick.DrawFloor();
		}
	}
}

void myDisplay()
{	/*cout << "Cam dis :" << cameraDis <<endl;
	cout << "Cam height :" << cameraHeight <<endl;
	cout << "Cam angle :" << cameraAngle <<endl;*/
	
	glPushMatrix();
	//glTranslatef(0,-hBase -hXylanh -hTable,0);
	
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
	glPopMatrix();
	
}

void mainDisplay(){
	cameraX = cameraDis * cos(PI * cameraAngle / 180);
	cameraY = cameraHeight;
	cameraZ = cameraDis * sin(PI *  cameraAngle / 180);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraX, cameraY , cameraZ, 0, 0, 0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glViewport(0, 0, widthWin, heightWin);
	myDisplay();
	drawFloor();
	glPushMatrix();
	glRotatef(-180,0,0,1);
	myDisplay();
	glPopMatrix();
	
	glFlush();
    glutSwapBuffers();
}

void changeLight1(){
	isEnableLight1 = !isEnableLight1;
	if (isEnableLight1)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
}

void lightInit(){
	glFrontFace(GL_CW   );
	glEnable(GL_NORMALIZE);

	GLfloat	lightDiffuse[]={1, 1, 1, 1.0};

	GLfloat	lightSpecular[]={1.0, 0.8, 1.0, 1.0};

	GLfloat	lightAmbient[]={0.2, 0.4, 0.6, 1.0};

	GLfloat 	light_position0[]={-20.0f, 20.0f, 20.0f, 1};  
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,	 lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	GLfloat 	light_position1[]= {20.0, -20.0, -20.0, 1.0};

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,	 lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	
	changeLight1();
}

void cameraInit()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 0.5, 1000);
}

void autoTurnRotor(int i){
	if (automation == true) 
	{
		angleRotor += 5 * value;
		xTranRotor= rTurn * cos(3.1415926 * angleRotor / 180);
		zTranRotor= rTurn * sin(3.1415926 * angleRotor / 180);
		glutPostRedisplay();
		glutTimerFunc(20,autoTurnRotor,0);
	}
}

void changeView(unsigned char key, int x, int y){
	key = tolower(key);
	switch(key){
	case '-':
		cameraDis -= value;
		break;
	case '+':
		cameraDis += value;
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
		if (posXylanh + tranXylanh + value + 0.2 <= hBase) tranXylanh += value;
		break;
	case '6':
		if (posXylanh + hXylanh + tranXylanh - value - 0.2 >= hBase) tranXylanh -= value;
		break;
	case '7':
		angleRotor -= 2 * 5 * value;
		break;
	case '8':
		angleRotor += 2 * 5 * value;
		break;
	case 'w':
		drawFrame = !drawFrame;
		drawColor = !drawColor;
		break;
	case 'd':
		changeLight1();
		break;
	case 'a':
		automation = !automation;
		autoTurnRotor(0);
		break;
	}

	xTranRotor= rTurn * cos(PI * angleRotor / 180);
	zTranRotor= rTurn * sin(PI * angleRotor / 180);
	glutPostRedisplay();
}

void changeView2(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		cameraHeight += value;
		break;	
	case GLUT_KEY_DOWN:
		cameraHeight -= value;
		break;
	case GLUT_KEY_LEFT:
		cameraAngle -= 5 * value;
		break;
	case GLUT_KEY_RIGHT:
		cameraAngle += 5 * value;
		break;
	}
	glutPostRedisplay();
}

void printConsole(){
	cout << "1, 2: Rotate the base \n3, 4: Rotate the cylinder \n5, 6: Cylinder up/down \n7, 8: Rotate the rotator" << endl;
	cout << "W, w: Switch between wireframe and solid mode \nA, a: Turn on/off animation \nD, d: Turn on/off the 2nd light source" <<endl;
	cout << "+   : to increase camera distance. \n-   : to decrease camera distance."<< endl;
	cout << "up arrow  : to increase camera height. \ndown arrow: to decrease camera height." << endl;
	cout << "<-        : to rotate camera clockwise. \n->        : to rotate camera counterclockwise." << endl;
	
}

int _tmain(int argc, _TCHAR* argv[])
{

	glutInit(&argc, (char**)argv); 
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(widthWin,heightWin);
	
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Assignment - Nguyen Duy Viet Toan (1513539)"); 

	float hBase1Arr[] = {hBase1 };
	float rBase1Arr[] = {rBase1,rBase1};
	float hBase2Arr[] = {hBase2};
	float rBase2Arr[] = {rBase2,rBase2};

	float hXylanhArr[] = {hXylanh};
	float rXylanhArr[] = {rXylanh, rXylanh};
	float hRotorArr[] = {hRotor};
	float rRotorArr[] = {rRotor, rRotor};
	float hSliderArr[] = {hSlider1, hSlider2, hSlider3};
	float rSliderArr[] = {rSlider1, rSlider1, rSlider2, rSlider2};
	float hLatchArr[] = {hLatch1, hLatch2};
	float rLatchArr[] = {rLatch1, rLatch1, rLatch2};

	base1.CreateCylinder(2, hBase1Arr, rBase1Arr, 72);
	base2.CreateCylinder(2, hBase2Arr, rBase2Arr, 72);
	table.CreateRectangular(wTable,hTable,lTable);
	xylanh.CreateCylinder(2, hXylanhArr, rXylanhArr, 72);
	rotor.CreateCylinder(2, hRotorArr, rRotorArr, 72);
	circle.CreateCircle(rCircle1, rCircle2, hCircle, 72);
	bar.CreateRectangular(wBar, hBar, lBar);
	slider.CreateCylinder(4, hSliderArr, rSliderArr,36);
	latch.CreateCylinder(3, hLatchArr, rLatchArr,36);
	shelf.CreateCircle2(rShelf1, rShelf2, lShelf, hShelf, 0.5, 36);

	printConsole();
	glutTimerFunc(20,autoTurnRotor,0);
	lightInit();
	cameraInit();	
	glutDisplayFunc(mainDisplay);

	glutSpecialFunc(changeView2);
	glutKeyboardFunc(changeView);
	glutMainLoop();
	return 0;
}
#pragma endregion main