#include "Primitives.h"

// =======================
//    Primitive
// =======================
Primitive::Primitive()
{
	this->tex = Point2d(1.0, 1.0);
}

void Primitive::setTextureParams(float s, float t)
{
	this->tex.setPoint2d(s, t);
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
			glTexCoord2f(texCoords[i][j]->X() / this->tex.X(), texCoords[i][j]->Y() / this->tex.Y());
				glNormal3f(norms[i][j]->X(), norms[i][j]->Y(), norms[i][j]->Z());
				glVertex3f(verts[i][j]->X(), verts[i][j]->Y(), verts[i][j]->Z());

				glTexCoord2f(texCoords[i][jj]->X() / this->tex.X(), texCoords[i][jj]->Y() / this->tex.Y());
				glNormal3f(norms[i][jj]->X(), norms[i][jj]->Y(), norms[i][jj]->Z());
				glVertex3f(verts[i][jj]->X(), verts[i][jj]->Y(), verts[i][jj]->Z());

				glTexCoord2f(texCoords[ii][jj]->X() / this->tex.X(), texCoords[ii][jj]->Y() / this->tex.Y());
				glNormal3f(norms[ii][jj]->X(), norms[ii][jj]->Y(), norms[ii][jj]->Z());
				glVertex3f(verts[ii][jj]->X(), verts[ii][jj]->Y(), verts[ii][jj]->Z());

				glTexCoord2f(texCoords[ii][j]->X() / this->tex.X(), texCoords[ii][j]->Y() / this->tex.Y());
				glNormal3f(norms[ii][j]->X(), norms[ii][j]->Y(), norms[ii][j]->Z());
				glVertex3f(verts[ii][j]->X(), verts[ii][j]->Y(), verts[ii][j]->Z());
			glEnd();
		}
	}

	// Top lid
	int zTop = stacks - 1;

	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);
		for(int j = 0; j < slices ; j++)
		{
			float s = (verts[zTop][j]->X() + top) / (top * this->tex.X());
			float t = (verts[zTop][j]->Y() + top) / (top * this->tex.Y());

			glTexCoord2f(s, t);
			glVertex3f(verts[zTop][j]->X(), verts[zTop][j]->Y(), verts[zTop][j]->Z());
		}
	glEnd();

	// Base lid
	int zBase = 0;

	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);
		for(int j = (slices - 1); j >= 0 ; j--)
		{
			float s = (verts[zBase][j]->X() + base) / (base * this->tex.X());
			float t = (verts[zBase][j]->Y() + base) / (base * this->tex.Y());

			glTexCoord2f(s, t);
			glVertex3f(verts[zBase][j]->X(), verts[zBase][j]->Y(), verts[zBase][j]->Z());
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
				(verts[i][j]->X()),
				(verts[i][j]->Y()),
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
				glNormal3f(norms[i][j]->X(), norms[i][j]->Y(), norms[i][j]->Z());
				glVertex3f(verts[i][j]->X(), verts[i][j]->Y(), verts[i][j]->Z());

				glTexCoord2f(ss, t);
				glNormal3f(norms[i][jj]->X(), norms[i][jj]->Y(), norms[i][jj]->Z());
				glVertex3f(verts[i][jj]->X(), verts[i][jj]->Y(), verts[i][jj]->Z());

				glTexCoord2f(ss, tt);
				glNormal3f(norms[ii][jj]->X(), norms[ii][jj]->Y(), norms[ii][jj]->Z());
				glVertex3f(verts[ii][jj]->X(), verts[ii][jj]->Y(), verts[ii][jj]->Z());

				glTexCoord2f(s, tt);
				glNormal3f(norms[ii][j]->X(), norms[ii][j]->Y(), norms[ii][j]->Z());
				glVertex3f(verts[ii][j]->X(), verts[ii][j]->Y(), verts[ii][j]->Z());
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

//TP2
// =======================
//    Patch
// =======================
Patch::Patch(int order, int partsU, int partsV, vector<Point3d *> controlPts, string drawingMode)
{
	this->ctrlPoints = new float[controlPts.size() * 3];

	for(unsigned int i = 0; i < controlPts.size(); i++)
	{
		for(int j = 0; j < 3; j++)
		{
			int index = i * 3 + j;
			ctrlPoints[index] = controlPts.at(i)->getFloatv()[j];
		}
	}

	this->order = order;
	this->partsU = partsU;
	this->partsV = partsV;
	this->drawingMode = drawingMode;
}

Patch::Patch(int order, int partsU, int partsV, string drawingMode)
{
	this->order = order;
	this->partsU = partsU;
	this->partsV = partsV;
	this->drawingMode = drawingMode;
}

void Patch::setControlPoints(float * v)
{
	this->ctrlPoints = v;
}

void Patch::draw()
{
	//glColor3f(1.0,1.0,1.0);
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlPoints[0]);
	//glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2                     ,  0.0, 1.0, 6, 2,  &nrmlcompon[0][0]);
	//glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &this->texCoords[0]);

	glEnable(GL_MAP2_VERTEX_3);
	//glEnable(GL_MAP2_NORMAL);
	//glEnable(GL_MAP2_COLOR_4);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glEnable(GL_AUTO_NORMAL);

	glMapGrid2f(this->partsU, 0.0, 1.0, this->partsV, 0.0, 1.0); 

	
	// SEGUE-SE EXEMPLO DE UTILIZACAO DE "EVALUATORS"
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	//myTexture->apply();


	glEvalMesh2(drawingMode == "fill" ? GL_FILL : drawingMode == "line" ? GL_LINE : GL_POINT, 0, this->partsU, 0, this->partsV);		// GL_POINT, GL_LINE, GL_FILL

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
}

string Patch::Type()
{
	return "Patch";
}

// =======================
//    Plane
// =======================
Plane::Plane(int parts) : Patch(1, parts, parts, "fill")
{
	float* v = new float[12];
	v[0] = -0.5; v[1] = 0; v[2] = 0.5;
	v[3] = -0.5; v[4] = 0; v[5] = -0.5;
	v[6] = 0.5; v[7] = 0; v[8] = 0.5;
	v[9] = 0.5; v[10] = 0; v[11] = -0.5;

	this->setControlPoints(v);
}

void Plane::draw()
{
	Patch::draw();
}

string Plane::Type()
{
	return "Plane";
}