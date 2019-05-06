#include "stdafx.h";
#include <iostream>
#include "math.h"
#include "Class.h";
using namespace std;

#define PI			3.1415926
#define	COLORNUM		14
float	ColorArr[COLORNUM][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0,  0.0, 1.0}, 
								{1.0, 1.0,  0.0}, { 1.0, 0.0, 1.0},{ 0.0, 1.0, 1.0}, 
								 {0.3, 0.3, 0.3}, {0.5, 0.5, 0.5}, { 0.9,  0.9, 0.9},
								{1.0, 0.5,  0.5}, { 0.5, 1.0, 0.5},{ 0.5, 0.5, 1.0},
									{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}};


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
			int		iv = face[f].vert[v].vertIndex;

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
			
			//ic = f % COLORNUM;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]); 
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::Draw(boolean hasFrame, boolean hasColor){
	if (hasFrame) this->DrawWireframe();
	if (hasColor) this->DrawColor();
}

void Mesh::CreateCylinder(float h , float r){
	this->numVerts = 102;
	this->pt = new Point3[numVerts]; 
	this->numFaces = 52;
	this->face = new Face[numFaces];
	pt[0].set(0,0,0);
	pt[1].set(0,0,h);
	face[0].set(n);
	face[1].set(n);
	//face[0].nVerts = face[1].nVerts = n;
	//face[0].vert = new VertexID[n];
	//face[1].vert = new VertexID[n];

	int countFace = 2;// face0 va face1 la 2 mat tron cua hinh tru roi
	int countP = 2; // pt0 va pt1 la 2 tam cua hinh tron roi

	for (int i = 0; i < n; i++){
		float angle = 2 * PI * i * (1.0/n);
		float x = r * cos(angle);
		float y = r * sin(angle);
		this->pt[countP++].set(x, y, 0);
		this->pt[countP++].set(x, y, h);

		/*face[0].vert[i].vertIndex = countP - 2;
		face[0].vert[i].colorIndex = 0;
		face[1].vert[i].vertIndex = countP - 1;
		face[1].vert[i].colorIndex = 0;*/
		face[0].vert[i].set(countP - 2, 0);
		face[1].vert[i].set(countP - 1, 0);


		/*face[countFace].nVerts = 4; 
		face[countFace].vert = new VertexID[4];
		face[countFace].vert[0].vertIndex = 0;
		face[countFace].vert[1].vertIndex = 1;
		face[countFace].vert[2].vertIndex = countP - 2;
		face[countFace].vert[3].vertIndex = countP - 1;
		for(int j = 0; j<face[countFace].nVerts ; j++)
			face[countFace].vert[j].colorIndex = 0;*/
		face[countFace].set(4);
		face[countFace].vert[0].set(0,0);
		face[countFace].vert[1].set(1,0);
		face[countFace].vert[2].set(countP - 2, 0);
		face[countFace].vert[3].set(countP - 1, 0);

		
		countFace ++;
	}
	
}

void Mesh::CreateCylinder2(int numf, float h[], float r[]){
	
	numFaces = n + numf;
	face = new Face[numFaces];
	numVerts = n * numf + 2;
	pt = new Point3[numVerts];

	float * sumH = new float[numf];
	
	for (int i =0; i < numf; i++){
		if (i == 0){
			sumH[i] = 0;
		}
		else{
			sumH[i] = sumH[i-1] + h[i - 1];
		}
		face[i].set(n);
		//face[i].nVerts = n;
		//face[i].vert = new VertexID[n];
	}

	pt[0].set(0,0,0);
	pt[1].set(0,sumH[numf-1],0);
	
	

	int countFace = numf;// face0 va face1 la 2 mat tron cua hinh tru roi
	int countP = 2;


	for (int i = 0; i < n; i++){
		float angle = 2 * PI * i * (1.0/n);
		for (int j = 0; j < numf  ; j++){
			float x = r[j] * cos(angle);
			float z = r[j] * sin(angle);
			this->pt[countP++].set(x, sumH[j], z);

			face[j].vert[i].set(countP - 1, 0);

			//face[j].vert[i].vertIndex = countP - 1;
			//face[j].vert[i].colorIndex = 0;
		}
		countFace = i + numf;

		face[countFace].set(numf + 2);
		face[countFace].vert[0].set(0,0);
		face[countFace].vert[numf + 1].set(1,0);

		//face[countFace].nVerts = numf + 2;
		//face[countFace].vert = new VertexID[face[countFace].nVerts];
		//face[countFace].vert[0].vertIndex = 0;
		//face[countFace].vert[0].colorIndex = 0;
		//face[countFace].vert[numf + 1].vertIndex = 1;
		//face[countFace].vert[numf + 1].colorIndex = 0;
		for (int j = 1; j < numf + 1  ; j++){
			//face[countFace].vert[j].vertIndex = countP - numf + j - 1;
			//face[countFace].vert[j].colorIndex = 0;
			face[countFace].vert[j].set(countP - numf + j - 1, 0);
		}
	}
}

void Mesh::CreateRectangular(float w, float l, float h){
	l = l/2;
	w = w/2;
	h = h/2;

	numVerts=8;
	pt = new Point3[numVerts];
	pt[0].set(-w, l, h);
	pt[1].set( w, l, h);
	pt[2].set( w, l, -h);
	pt[3].set(-w, l, -h);
	pt[4].set(-w, -l, h);
	pt[5].set( w, -l, h);
	pt[6].set( w, -l, -h);
	pt[7].set(-w, -l, -h);

	numFaces= 6;
	face = new Face[numFaces];
	//Left face
	face[0].set(4);
	face[0].vert[0].set(1, 0);
	face[0].vert[1].set(5, 0);
	face[0].vert[2].set(6, 0);
	face[0].vert[3].set(2, 0);
	
	//Right face
	face[1].set(4);
	face[1].vert[0].set(0,0);
	face[1].vert[1].set(3, 0);
	face[1].vert[2].set(7, 0);
	face[1].vert[3].set(4, 0);

	//top face
	face[2].set(4);
	face[2].vert[0].set(0,0);
	face[2].vert[1].set(1,0);
	face[2].vert[2].set(2,0);
	face[2].vert[3].set(3,0);
	//bottom face
	face[3].set(4);
	face[3].vert[0].set(7,0);
	face[3].vert[1].set(6,0);
	face[3].vert[2].set(5,0);
	face[3].vert[3].set(4,0);
	//near face
	face[4].set(4);
	face[4].vert[0].set(4,0);
	face[4].vert[1].set(5,0);
	face[4].vert[2].set(1,0);
	face[4].vert[3].set(0,0);

	//Far face
	face[5].set(4);
	face[5].vert[0].set(3,0);
	face[5].vert[1].set(2,0);
	face[5].vert[2].set(6,0);
	face[5].vert[3].set(7,0);
}

void Mesh::CreateCircle( float r1, float r2, float h){
	if (r1 < r2) 
		return;
	numFaces = 1.5*n + 1;
	int m = 4;
	face = new Face[numFaces];
	numVerts = 2*n + 4;
	pt = new Point3[numVerts];

	for (int i = 0; i <= int(n/2) ; i++){
		float angle = 2 * PI * i * (1.0/n);
		float x1 = r1 * cos(angle);
		float z1 = r1 * sin(angle);
		float x2 = r2 * cos(angle);
		float z2 = r2 * sin(angle);
		pt[m*i].set(x1,h,z1);
		pt[m*i +1].set(x1,0,z1);
		pt[m*i + 2].set(x2,0,z2);
		pt[m*i + 3].set(x2,h,z2);

		int k = 3*i ;
		face[k].nVerts = 4;
		face[k].vert = new VertexID[4];
		for (int j = 0; j < 4; j++){
			face[k].vert[j].set(m*i + j, 0);
		}

		if (i > 0){
			k = 3*i - 2;
			int j = 3*i - 1;
			face[k].nVerts = face[j].nVerts =  4;
			face[k].vert = new VertexID[face[k].nVerts];
			face[j].vert = new VertexID[face[j].nVerts];

			face[k].vert[0].set(m*i +2 - 4 , 0) ;
			face[k].vert[1].set(m*i +3  - 4, 0) ;
			face[k].vert[2].set(m*i +3 , 0) ;
			face[k].vert[3].set(m*i +2 , 0) ;

			face[j].vert[0].set(m*i - 4 , 0) ;
			face[j].vert[1].set(m*i +1  - 4, 0) ;
			face[j].vert[2].set(m*i +1 , 0) ;
			face[j].vert[3].set(m*i , 0) ;
		}

	}
}

void Mesh::CreateCircle2(float r1, float r2, float l, float h){
	if (r1 < r2) 
		return;
	numFaces = 2*n + 12;
	int m = 4;
	face = new Face[numFaces];
	numVerts = 4*n + 8;
	pt = new Point3[numVerts];
	int countFace = -1;
	int countVert = -1;
	for (int i = 0; i <= int(n/2) ; i++){
		float angle = 2 * PI * i * (1.0/n);
		float x1 = r1 * cos(angle);
		float z1 = r1 * sin(angle);
		float x2 = r2 * cos(angle);
		float z2 = r2 * sin(angle);
		pt[++countVert].set(x1,h,z1);
		pt[++countVert].set(x1,0,z1);
		pt[++countVert].set(x2,0,z2);
		pt[++countVert].set(x2,h,z2);

		face[++countFace].set(4);
		for (int j = 0; j < 4; j++){
			face[countFace].vert[j].set(m*i + j, 0);
		}

		if (i > 0){
			int k = ++countFace;
			int j = ++countFace;
			face[k].set(4);
			face[j].set(4);

			face[k].vert[0].set(m*i +2 - 4 , 0) ;
			face[k].vert[1].set(m*i +3  - 4, 0) ;
			face[k].vert[2].set(m*i +3 , 0) ;
			face[k].vert[3].set(m*i +2 , 0) ;

			face[j].vert[0].set(m*i - 4 , 0) ;
			face[j].vert[1].set(m*i +1  - 4, 0) ;
			face[j].vert[2].set(m*i +1 , 0) ;
			face[j].vert[3].set(m*i , 0) ;
		}
		if (i == 0 || i == n/2){
			pt[++countVert].set(x1,h,-r2 -l);
			pt[++countVert].set(x1,0,-r2 -l);
			pt[++countVert].set(x2,0,-r2 -l);
			pt[++countVert].set(x2,h,-r2 -l);
			AddRectangular(countVert, countFace, face);
		}
	}
	//
	//
	for (int i = int(n/2) + 1; i < n ; i++){
		
		float angle = 2 * PI * i * (1.0/n);
		float x = r2 * cos(angle);
		float z = r2 * sin(angle);

		pt[++countVert].set(x,0,z);
		pt[++countVert].set(x,h,z);
		pt[++countVert].set(x,h, -r2 -l);
		pt[++countVert].set(x,0, -r2 -l);

		face[++countFace].set(4);
		for (int j = 0; j < 4; j++)
			face[countFace].vert[j].set(countVert - 4 + j, 0);
	}

}

void Mesh::AddRectangular(int& countPoint, int& countFace, Face* face ){
	int recArr[6][4] = {{1,5,6,2},{0,3,7,4},{0,1,2,3},{7,6,5,4},{4,5,1,0},{3,2,6,7}};
	for (int j = 0 ; j < 6; j++){
		face[++countFace].set(4);
		for (int k =0; k < 4; k++ )
			face[countFace].vert[k].set(countPoint - 7 + recArr[j][k],0);
	}
}


void Camera::changeHeight(float value ){
	y += value;
}

void Camera::changeDis(float value){
	float fracZX = z / x; 
	float changeX = sqrt(value*value / ( 1 + fracZX*fracZX)) * value/ abs(value);
	float changeZ = sqrt(value*value - changeX*changeX) * value/ abs(value);

	x += changeX;
	z += changeZ;
}

void Camera::changeAngle(float value){
	float cosValue = cos(value * PI/ 180);
	float sinValue = sin(value * PI/ 180);
	x = cosValue * x + sinValue * z;
	z = -sinValue * x + cosValue * z;
}