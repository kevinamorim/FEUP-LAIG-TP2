#include "Primitives.h"

// =======================
//    Primitive
// =======================
Primitive::Primitive()
{
	this->texParams = Point2d(1.0, 1.0);
	hasTexture = false;
}

void Primitive::setTexture(Texture *t)
{
	this->texture = t;
	this->texParams.setPoint2d(t->S(), t->T());
	hasTexture = true;
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
			pow(verts[j]->x - verts[side]->x, 2) +
			pow(verts[j]->y - verts[side]->y, 2) +
			pow(verts[j]->z - verts[side]->z, 2));
}

void Triangle::draw()
{
	glBegin(GL_POLYGON);
		glNormal3f(normal.x, normal.y, normal.z);
		for(unsigned int i = 0; i < verts.size(); i++)
		{
			glTexCoord2f(texCoords[i]->x / this->texParams.x, texCoords[i]->y / this->texParams.y);
			glVertex3d(verts[i]->x, verts[i]->y, verts[i]->z);
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
			this->normal.x + (verts[i]->x - verts[j]->y) * (verts[i]->z + verts[j]->z),
			this->normal.y + (verts[i]->z - verts[j]->z) * (verts[i]->x + verts[j]->x),
			this->normal.z + (verts[i]->x - verts[j]->x) * (verts[i]->z + verts[j]->z)
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
	this->width = abs(p2->x - p1->x);
	this->height = abs(p2->y - p1->y);

	calculateVertex(p1, p2);
}

void Rectangle::draw()
{
	float sMax = width / this->texParams.x;
	float tMax = height / this->texParams.y;

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
				glVertex3d(verts[i][j]->x, verts[i][j]->y, 0);

				glTexCoord2f(jj * sStep, i * tStep);
				glVertex3d(verts[i][jj]->x, verts[i][jj]->y, 0);

				glTexCoord2f(jj * sStep, ii * tStep);
				glVertex3d(verts[ii][jj]->x, verts[ii][jj]->y, 0);

				glTexCoord2f(j * sStep, ii * tStep);
				glVertex3d(verts[ii][j]->x, verts[ii][j]->y, 0);
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
				(p1->x + (j * xStep)),
				(p1->y + (i * yStep))));
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

	calculateVertex();
	calculateNormals();
	calculateTextureCoords();

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
			glTexCoord2f(texCoords[i][j]->x / this->texParams.x, texCoords[i][j]->y / this->texParams.y);
				glNormal3f(norms[i][j]->x, norms[i][j]->y, norms[i][j]->z);
				glVertex3f(verts[i][j]->x, verts[i][j]->y, verts[i][j]->z);

				glTexCoord2f(texCoords[i][jj]->x / this->texParams.x, texCoords[i][jj]->y / this->texParams.y);
				glNormal3f(norms[i][jj]->x, norms[i][jj]->y, norms[i][jj]->z);
				glVertex3f(verts[i][jj]->x, verts[i][jj]->y, verts[i][jj]->z);

				glTexCoord2f(texCoords[ii][jj]->x / this->texParams.x, texCoords[ii][jj]->y / this->texParams.y);
				glNormal3f(norms[ii][jj]->x, norms[ii][jj]->y, norms[ii][jj]->z);
				glVertex3f(verts[ii][jj]->x, verts[ii][jj]->y, verts[ii][jj]->z);

				glTexCoord2f(texCoords[ii][j]->x / this->texParams.x, texCoords[ii][j]->y / this->texParams.y);
				glNormal3f(norms[ii][j]->x, norms[ii][j]->y, norms[ii][j]->z);
				glVertex3f(verts[ii][j]->x, verts[ii][j]->y, verts[ii][j]->z);
			glEnd();
		}
	}

	// Top lid
	int zTop = stacks - 1;

	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		for(int j = 0; j < slices ; j++)
		{
			float s = (verts[zTop][j]->x + top) / (top * this->texParams.x);
			float t = (verts[zTop][j]->y + top) / (top * this->texParams.y);

			glTexCoord2f(s, t);
			glVertex3f(verts[zTop][j]->x, verts[zTop][j]->y, verts[zTop][j]->z);
		}
	glEnd();

	// Base lid
	int zBase = 0;

	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		for(int j = (slices - 1); j >= 0 ; j--)
		{
			float s = (verts[zBase][j]->x + base) / (base * this->texParams.x);
			float t = (verts[zBase][j]->y + base) / (base * this->texParams.y);

			glTexCoord2f(s, t);
			glVertex3f(verts[zBase][j]->x, verts[zBase][j]->y, verts[zBase][j]->z);
		}
	glEnd();

}

string Cylinder::Type()
{
	return "Cylinder";
}

void Cylinder::calculateVertex() {
	// Vertex init
	this->verts = vector<vector<Point3d*> >();

	vector<Point3d*> temp = vector<Point3d*>();

	float zStep = height / (stacks - 1);
	float radiusStep = (top - base) / (stacks - 1);
	float radius = base;

	for(int i = 0; i < stacks; i++)
	{
		float zCoord = i * zStep;

		for(int j = 0; j < slices; j++)
		{
			temp.push_back(new Point3d(
				(radius * cos(j * alpha)),
				(radius * sin(j * alpha)),
				zCoord));
		}

		verts.push_back(temp);

		temp.clear();
		radius += radiusStep;
	}
}

void Cylinder::calculateNormals() {

	// Normals init
	this->norms = vector<vector<Point3d*> >();
	vector<Point3d*> tempNorms = vector<Point3d*>();

	float zNorm = (base - top);

	for(int i = 0; i < stacks; i++)
	{
		for(int j = 0; j < slices; j++)
		{
			tempNorms.push_back(new Point3d(
				(verts[i][j]->x),
				(verts[i][j]->y),
				zNorm
			));
		}

		norms.push_back(tempNorms);

		tempNorms.clear();
	}
}

void Cylinder::calculateTextureCoords() {

		// Texture coords init
	this->texCoords = vector<vector<Point2d*> >();

	vector<Point2d*> tempTex = vector<Point2d*>();

	float sInc = 2 * PI / slices; // diameter / slices
	float tInc = 1 / (stacks - 1);

	float s = 0;
	float t = 0;

	for(int i = 0; i < stacks; i++)
	{
		for(int j = 0; j < slices; j++)
		{
			tempTex.push_back(new Point2d(s, t));
			s += sInc;
		}

		texCoords.push_back(tempTex);

		tempTex.clear();

		t += tInc;
	}

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

	/*
		A torus can be seen and drawn as successive circunferences,
		each one varying in their height and radius according to a circunference
		centered in the "center" of the torus' tube and the number of slices:
			- alpha = 2 * PI / slices;
			- z coordinate (or 'height' of each circunference) = sin(sliceIndex * angleOfIncrement);
			- radius = cos(sliceIndex * angleOfIncrement);
	*/
	calculate();

}

void Torus::draw()
{
	float sInc = (outer * 2 * PI) / (loops * this->texParams.x);
	float tInc = (radiusTube * 2 * PI) / (slices * this->texParams.y);

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
				glNormal3f(norms[i][j]->x, norms[i][j]->y, norms[i][j]->z);
				glVertex3f(verts[i][j]->x, verts[i][j]->y, verts[i][j]->z);

				glTexCoord2f(ss, t);
				glNormal3f(norms[i][jj]->x, norms[i][jj]->y, norms[i][jj]->z);
				glVertex3f(verts[i][jj]->x, verts[i][jj]->y, verts[i][jj]->z);

				glTexCoord2f(ss, tt);
				glNormal3f(norms[ii][jj]->x, norms[ii][jj]->y, norms[ii][jj]->z);
				glVertex3f(verts[ii][jj]->x, verts[ii][jj]->y, verts[ii][jj]->z);

				glTexCoord2f(s, tt);
				glNormal3f(norms[ii][j]->x, norms[ii][j]->y, norms[ii][j]->z);
				glVertex3f(verts[ii][j]->x, verts[ii][j]->y, verts[ii][j]->z);
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

void Torus::calculate() {

	this->verts = vector<vector<Point3d*> >();
	this->norms = vector<vector<Point3d*> >();

	float alpha = 2 * PI / slices;
	float beta = 2 * PI / loops; // angle of increment for each of the circunferences;

	vector<Point3d*> temp;
	vector<Point3d*> tempNorm;

	temp = vector<Point3d*>();
	tempNorm = vector<Point3d*>();

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

			temp.push_back(new Point3d(x,y,z));

			x -= xCenter;
			y -= yCenter;

			tempNorm.push_back(new Point3d(x,y,z));
			
		}

		verts.push_back(temp);
		norms.push_back(tempNorm);

		temp.clear();
		tempNorm.clear();

	}

}