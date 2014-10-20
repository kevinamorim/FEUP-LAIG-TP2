#include "Primitives.h"

// =======================
//    Primitive
// =======================
Primitive::Primitive()
{
	this->tex = Point2d(1.0, 1.0);
}

void Primitive::setTexture(Texture* tex)
{
	this->tex.setPoint2d(tex->getTexLengthS(), tex->getTexLengthT());
}



// =======================
//    Triangle
// =======================
Triangle::Triangle(Point3d* point1, Point3d* point2, Point3d* point3)
{
	// Vertex init
	this->verts = vector<Point3d*>();
	this->verts.push_back(point1);
	this->verts.push_back(point2);
	this->verts.push_back(point3);

	calculateNormal();
	calculateTextureCoords();
}

float Triangle::getSideLength(int side)
{
	int j = (side + 1) % verts.size();

	return sqrt(
			pow(verts[j]->X() - verts[side]->X(), 2) +
			pow(verts[j]->Y() - verts[side]->Y(), 2) +
			pow(verts[j]->Z() - verts[side]->Z(), 2));
}

void Triangle::draw()
{
	glBegin(GL_POLYGON);
		glNormal3f(normal.X(), normal.Y(), normal.Z());
		for(unsigned int i = 0; i < verts.size(); i++)
		{
			glTexCoord2f(texCoords[i]->X() / this->tex.X(), texCoords[i]->Y() / this->tex.Y());
			glVertex3d(verts[i]->X(), verts[i]->Y(), verts[i]->Z());
		}
	glEnd();
}

string Triangle::Type()
{
	return "Triangle";
}

void Triangle::calculateNormal() {

	// Normals init
	this->normal = Point3d(0, 0, 0);

	// Caculates normals through newell's method  
	for(unsigned int i = 0; i < verts.size(); i++)
	{
		int j = (i + 1) % verts.size();

		this->normal.setPoint3d(
			this->normal.X() + (verts[i]->X() - verts[j]->Y()) * (verts[i]->Z() + verts[j]->Z()),
			this->normal.Y() + (verts[i]->Z() - verts[j]->Z()) * (verts[i]->X() + verts[j]->X()),
			this->normal.Z() + (verts[i]->X() - verts[j]->X()) * (verts[i]->Z() + verts[j]->Z())
		);

	}

}

void Triangle::calculateTextureCoords() {

	this->texCoords = vector<Point2d*>();

	float b = getSideLength(0);
	float c = getSideLength(1);
	float a = getSideLength(2);

	float cosB = (pow(a,2) - pow(b,2) + pow(c,2)) / (2 * a * c);
	float sinB = 1 - pow(cosB, 2);

	this->texCoords.push_back(new Point2d((c - a * cosB), (a * sinB)));
	this->texCoords.push_back(new Point2d(0, 0));
	this->texCoords.push_back(new Point2d(c, 0));

}



// =======================
//    Rectangle
// =======================
Rectangle::Rectangle(Point2d* p1, Point2d* p2)
{
	this->width = abs(p2->X() - p1->X());
	this->height = abs(p2->Y() - p1->Y());

	calculateVertex(p1, p2);
}

void Rectangle::draw()
{
	float sMax = width / this->tex.X();
	float tMax = height / this->tex.Y();

	float sStep = sMax / (QUAD_RES - 1);
	float tStep = tMax / (QUAD_RES - 1);

	glNormal3f(0, 0, 1); // Perpendicular normal

	for(unsigned int i = 0; i < (verts.size() - 1) ; i++)
	{
		int ii = (i + 1) % verts.size();

		for(unsigned int j = 0; j < (verts[0].size() - 1); j++)
		{
			int jj = (j + 1) % verts[0].size();

			glBegin(GL_POLYGON);
				glTexCoord2f(j * sStep, i * tStep);
				glVertex3d(verts[i][j]->X(), verts[i][j]->Y(), 0);

				glTexCoord2f(jj * sStep, i * tStep);
				glVertex3d(verts[i][jj]->X(), verts[i][jj]->Y(), 0);

				glTexCoord2f(jj * sStep, ii * tStep);
				glVertex3d(verts[ii][jj]->X(), verts[ii][jj]->Y(), 0);

				glTexCoord2f(j * sStep, ii * tStep);
				glVertex3d(verts[ii][j]->X(), verts[ii][j]->Y(), 0);
			glEnd();
		}
	}
}

string Rectangle::Type()
{
	return "Rectangle";
}

void Rectangle::calculateVertex(Point2d* p1, Point2d* p2) {

	this->verts = vector<vector<Point2d*> >();

	vector<Point2d*> temp = vector<Point2d*>();

	// Divides the triangle in multile vertex.
	float xStep = width / (QUAD_RES - 1);
	float yStep = height / (QUAD_RES - 1);

	for(int i = 0; i < QUAD_RES; i++)
	{
		for(int j = 0; j < QUAD_RES; j++)
		{
			temp.push_back(new Point2d(
				(p1->X() + (j * xStep)),
				(p1->Y() + (i * yStep))));
		}

		verts.push_back(temp);

		temp.clear();
	}
}



// =======================
//    Cylinder
// =======================
Cylinder::Cylinder(float base, float top, float height, int slices, int stks)
{
	this->base = base;
	this->top = top; 
	this->height = height;
	this->slices = slices;
	this->stacks = stks + 2;

	this->alpha = 2 * PI / slices;

	// Vertex init
	this->vertsX = vector<vector<float> >();
	this->vertsY = vector<vector<float> >();
	this->vertsZ = vector<vector<float> >();

	vector<float> tempX = vector<float>();
	vector<float> tempY = vector<float>();
	vector<float> tempZ = vector<float>();

	float zStep = height / (stacks - 1);
	float radiusStep = (top - base) / (stacks - 1);
	float radius = base;

	for(int i = 0; i < stacks; i++)
	{
		float zCoord = i * zStep;

		for(int j = 0; j < slices; j++)
		{
			tempX.push_back(radius * cos(j * alpha));
			tempY.push_back(radius * sin(j * alpha));
			tempZ.push_back(zCoord);
		}

		vertsX.push_back(tempX);
		vertsY.push_back(tempY);
		vertsZ.push_back(tempZ);

		tempX.clear();
		tempY.clear();
		tempZ.clear();

		radius += radiusStep;
	}

	// Normals init
	this->normsX = vector<vector<float> >();
	this->normsX = vector<vector<float> >();
	this->normsX = vector<vector<float> >();

	vector<float> tempNormsX = vector<float>();
	vector<float> tempNormsY = vector<float>();
	vector<float> tempNormsZ = vector<float>();

	float zNorm = (base - top);

	for(int i = 0; i < stacks; i++)
	{
		for(int j = 0; j < slices; j++)
		{
			tempNormsX.push_back(vertsX[i][j]);
			tempNormsY.push_back(vertsY[i][j]);
			tempNormsZ.push_back(zNorm);
		}

		normsX.push_back(tempNormsX);
		normsY.push_back(tempNormsY);
		normsZ.push_back(tempNormsZ);

		tempNormsX.clear();
		tempNormsY.clear();
		tempNormsZ.clear();
	}

	// Texture coords init
	this->texCoordS = vector<vector<float> >();
	this->texCoordT = vector<vector<float> >();

	vector<float> tempTexS = vector<float>();
	vector<float> tempTexT = vector<float>();

	float sInc = 2 * PI / slices; // diameter / slices
	float tInc = 1 / (stacks - 1);

	float s = 0;
	float t = 0;

	for(int i = 0; i < stacks; i++)
	{
		for(int j = 0; j < slices; j++)
		{
			tempTexS.push_back(s);
			tempTexT.push_back(t);

			s += sInc;
		}

		texCoordS.push_back(tempTexS);
		texCoordT.push_back(tempTexT);

		tempTexS.clear();
		tempTexT.clear();

		t += tInc;
	}
}

void Cylinder::draw()
{
	for(int i = 0; i < stacks; i++)
	{
		int ii = (i+1) % stacks;

		for(int j = 0; j < slices; j++)
		{
			int jj = (j+1) % slices;

			glBegin(GL_POLYGON);
			glTexCoord2f(texCoordS[i][j] / this->tex.X(), texCoordT[i][j] / this->tex.Y());
				glNormal3f(normsX[i][j], normsY[i][j], normsZ[i][j]);
				glVertex3f(vertsX[i][j], vertsY[i][j], vertsZ[i][j]);

				glTexCoord2f(texCoordS[i][jj] / this->tex.X(), texCoordT[i][jj] / this->tex.Y());
				glNormal3f(normsX[i][jj], normsY[i][jj], normsZ[i][jj]);
				glVertex3f(vertsX[i][jj], vertsY[i][jj], vertsZ[i][jj]);

				glTexCoord2f(texCoordS[ii][jj] / this->tex.X(), texCoordT[ii][jj] / this->tex.Y());
				glNormal3f(normsX[ii][jj], normsY[ii][jj], normsZ[ii][jj]);
				glVertex3f(vertsX[ii][jj], vertsY[ii][jj], vertsZ[ii][jj]);

				glTexCoord2f(texCoordS[ii][j] / this->tex.X(), texCoordT[ii][j] / this->tex.Y());
				glNormal3f(normsX[ii][j], normsY[ii][j], normsZ[ii][j]);
				glVertex3f(vertsX[ii][j], vertsY[ii][j], vertsZ[ii][j]);
			glEnd();
		}
	}

	// Top lid
	int zTop = stacks - 1;

	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		for(int j = 0; j < slices ; j++)
		{
			float s = (vertsX[zTop][j] + top) / (top * this->tex.X());
			float t = (vertsY[zTop][j] + top) / (top * this->tex.Y());

			glTexCoord2f(s, t);
			glVertex3f(vertsX[zTop][j], vertsY[zTop][j], vertsZ[zTop][j]);
		}
	glEnd();

	// Base lid
	int zBase = 0;

	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		for(int j = (slices - 1); j >= 0 ; j--)
		{
			float s = (vertsX[zBase][j] + base) / (base * this->tex.X());
			float t = (vertsY[zBase][j] + base) / (base * this->tex.Y());

			glTexCoord2f(s, t);
			glVertex3f(vertsX[zBase][j], vertsY[zBase][j], vertsZ[zBase][j]);
		}
	glEnd();

}

string Cylinder::Type()
{
	return "Cylinder";
}

// =======================
//    Sphere
// =======================
Sphere::Sphere(float radius, int slices, int stacks)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
	this->obj = gluNewQuadric();

	gluQuadricTexture(this->obj, true);
}

void Sphere::draw()
{
	gluSphere(obj, radius, slices, stacks);
}

string Sphere::Type()
{
	return "Sphere";
}

// =======================
//    Torus
// =======================
Torus::Torus(float inner, float outer, int slices, int loops)
{
	this->inner = inner;
	this->outer = outer;
	this->slices = slices;
	this->loops = loops;
	this->radiusTube = 0.5 * (outer - inner);

	// Vertex and Normals init
	this->vertsX = vector<vector<float> >();
	this->vertsY = vector<vector<float> >();
	this->vertsZ = vector<vector<float> >();

	this->normsX = vector<vector<float> >();
	this->normsY = vector<vector<float> >();
	this->normsZ = vector<vector<float> >();

	/*
		A torus can be seen and drawn as successive circunferences,
		each one varying in their height and radius according to a circunference
		centered in the "center" of the torus' tube and the number of slices:
			- alpha = 2 * PI / slices;
			- z coordinate (or 'height' of each circunference) = sin(sliceIndex * angleOfIncrement);
			- radius = cos(sliceIndex * angleOfIncrement);
	*/

	float alpha = 2 * PI / slices;
	float beta = 2 * PI / loops; // angle of increment for each of the circunferences;

	vector<float> tempX, tempY, tempZ;
	vector<float> tempNormX, tempNormY, tempNormZ;

	tempX = vector<float>();
	tempY = vector<float>();
	tempZ = vector<float>();

	tempNormX = vector<float>();
	tempNormY = vector<float>();
	tempNormZ = vector<float>();

	for(int i = 0; i < slices; i++)
	{
		float zCoord = radiusTube * sin(i * alpha);
		float radius = inner + radiusTube * (1 + cos(i * alpha));

		for(int j = 0; j < loops; j++)
		{
			float x = radius * cos(j * beta);
			float y = radius * sin(j * beta);
			float z = zCoord;

			float xCenter = (inner + radiusTube) * cos(j * beta);
			float yCenter = (inner + radiusTube) * sin(j * beta);

			tempX.push_back(x);
			tempY.push_back(y);
			tempZ.push_back(z);

			x -= xCenter;
			y -= yCenter;

			tempNormX.push_back(x);
			tempNormY.push_back(y);
			tempNormZ.push_back(z);
			
		}

		vertsX.push_back(tempX);
		vertsY.push_back(tempY);
		vertsZ.push_back(tempZ);

		normsX.push_back(tempNormX);
		normsY.push_back(tempNormY);
		normsZ.push_back(tempNormZ);

		tempNormX.clear();
		tempNormY.clear();
		tempNormZ.clear();

		tempX.clear();
		tempY.clear();
		tempZ.clear();
	}
}

void Torus::draw()
{
	float sInc = (outer * 2 * PI) / (loops * this->tex.X());
	float tInc = (radiusTube * 2 * PI) / (slices * this->tex.Y());

	for(int i = 0; i < slices; i++)
	{
		float t = i * tInc;
		float tt = (i + 1) * tInc;

		int ii = (i + 1) % slices;

		for(int j = 0; j < loops; j++)
		{
			float s = j * sInc;
			float ss = (j + 1) * sInc;

			int jj = (j + 1) % loops;

			glBegin(GL_POLYGON);
				glTexCoord2f(s, t);
				glNormal3f(normsX[i][j], normsY[i][j], normsZ[i][j]);
				glVertex3f(vertsX[i][j], vertsY[i][j], vertsZ[i][j]);

				glTexCoord2f(ss, t);
				glNormal3f(normsX[i][jj], normsY[i][jj], normsZ[i][jj]);
				glVertex3f(vertsX[i][jj], vertsY[i][jj], vertsZ[i][jj]);

				glTexCoord2f(ss, tt);
				glNormal3f(normsX[ii][jj], normsY[ii][jj], normsZ[ii][jj]);
				glVertex3f(vertsX[ii][jj], vertsY[ii][jj], vertsZ[ii][jj]);

				glTexCoord2f(s, tt);
				glNormal3f(normsX[ii][j], normsY[ii][j], normsZ[ii][j]);
				glVertex3f(vertsX[ii][j], vertsY[ii][j], vertsZ[ii][j]);
			glEnd();
		}
	}
}

float getNorm(float x, float y, float z)
{
	return sqrt(x*x + y*y + z*z);
}

string Torus::Type()
{
	return "Torus";
}