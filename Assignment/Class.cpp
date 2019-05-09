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
			int	iv = face[f].vert[v].vertIndex;
			int	ic = face[f].vert[v].colorIndex;

			
			GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
			GLfloat diffuse[] = {1.0, 1, 1.0, 1.0};
			GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
			GLfloat shine = 128;
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ColorArr[ic]);
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

			
			GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
			GLfloat diffuse[] = {1.0, 1, 1.0, 1.0};
			GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
			GLfloat shine = 128;
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ColorArr[ic]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
			glMaterialf(GL_FRONT, GL_SHININESS, shine);

			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::Draw(boolean hasFrame, boolean hasColor){
	if (hasFrame) this->DrawWireframe();
	if (hasColor) this->DrawColor();
}

void Mesh::CreateCylinder(int f, float h[], float r[], int color){
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

void Mesh::CreateRectangular(float w, float l, float h, int color){
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
	AddRectangular(0,countFace, face, color);
	CalculateFacesNorm();
}

void Mesh::CreateCircle( float r1, float r2, float h, int color){
	if (r1 < r2) 
		return;
	numFaces = 6 * n /2;
	int m = 4;
	face = new Face[numFaces];
	numVerts = 4*(n/2 + 1);
	pt = new Point3[numVerts];

	int countP = -1;
	int countFace = -1;
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
		/*}
		else{
		pt[++countP].set(x2,h,z2);
		pt[++countP].set(x1,h,z1);
		pt[++countP].set(x1,0,z1);
		pt[++countP].set(x2,0,z2);
		}*/

		if (i > 0) 
			AddRectangular(countP-7,countFace,face,color);
		/*pt[m*i].set(x1,h,z1);
		pt[m*i +1].set(x1,0,z1);
		pt[m*i + 2].set(x2,0,z2);
		pt[m*i + 3].set(x2,h,z2);

		int k = 3*i ;
		face[k].set(4);
		for (int j = 0; j < 4; j++){
			face[k].vert[j].set(m*i + j, color);
		}

		if (i > 0){
			k = 3*i - 2;
			int j = 3*i - 1;
			face[k].set(4);
			face[j].set(4);

			face[k].vert[0].set(m*i +2 - 4 , color) ;
			face[k].vert[1].set(m*i +3  - 4, color) ;
			face[k].vert[2].set(m*i +3 , color) ;
			face[k].vert[3].set(m*i +2 , color) ;

			face[j].vert[0].set(m*i - 4 , color) ;
			face[j].vert[1].set(m*i +1  - 4, color) ;
			face[j].vert[2].set(m*i +1 , color) ;
			face[j].vert[3].set(m*i , color) ;
		}
		*/
	}
	CalculateFacesNorm();
}

void Mesh::CreateCircle2(float r1, float r2, float l, float h, float hBase, int color){
	if (r1 < r2) 
		return;
	numFaces = 2*n + 6*3;
	int m = 4;
	face = new Face[numFaces];
	numVerts = 4*n + 4*2 + 8;
	pt = new Point3[numVerts];
	int countFace = -1;
	int countVert = -1;
	for (int i = int(n/2); i >=0 ; i--){
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
			face[countFace].vert[j].set(m*i + j, color);
		}

		if (i > 0){
			int k = ++countFace;
			int j = ++countFace;
			face[k].set(4);
			face[j].set(4);

			face[k].vert[0].set(m*i +2 - 4 , color) ;
			face[k].vert[1].set(m*i +3  - 4, color) ;
			face[k].vert[2].set(m*i +3 , color) ;
			face[k].vert[3].set(m*i +2 , color) ;

			face[j].vert[0].set(m*i - 4 , color) ;
			face[j].vert[1].set(m*i +1  - 4, color) ;
			face[j].vert[2].set(m*i +1 , color) ;
			face[j].vert[3].set(m*i , color) ;
		}
		if (i == 0 || i == n/2){
			pt[++countVert].set(x1,h,-r2 -l);
			pt[++countVert].set(x1,0,-r2 -l);
			pt[++countVert].set(x2,0,-r2 -l);
			pt[++countVert].set(x2,h,-r2 -l);
			AddRectangular(countVert - 7, countFace, face, color);
		}
	}

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
			face[countFace].vert[j].set(countVert - 4 + j, color);
	}

	pt[++countVert].set(-2*r1,h,-r2 -l);
	pt[++countVert].set(-2*r1,0,-r2 -l);
	pt[++countVert].set(2*r1,0,-r2 -l);
	pt[++countVert].set(2*r1,h,-r2 -l);
	pt[++countVert].set(-2*r1, h,-r2 -l - hBase);
	pt[++countVert].set(-2*r1, 0,-r2 -l - hBase);
	pt[++countVert].set(2*r1, 0,-r2 -l - hBase);
	pt[++countVert].set(2*r1, h,-r2 -l - hBase);
	AddRectangular(countVert - 7, countFace, face, color);

	CalculateFacesNorm();
}

void Mesh::AddRectangular(int countPoint, int& countFace, Face* face, int color ){
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
	m.x = -m.x;m.y = -m.y;m.z = -m.z;
	face[facei].facenorm = m;
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