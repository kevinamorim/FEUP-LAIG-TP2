#include "Patch.h"

Patch::Patch(int ord, int partsU, int partsV, vector<Point3d *> controlPts, string drawingMode)
{
	int vSize = controlPts.size();

	// Texture Coordinates
	this->texCoords = new float[vSize * 2];
	float step = 1.0 / ord;
	for(int i = 0; i < vSize; i++)
	{
		int offset = i * 2;
		int sOffset = i % (ord + 1);
		int tOffset = i / (ord + 1);
		float s = sOffset * step;
		float t = tOffset * step;
		texCoords[offset] = s;
		texCoords[offset+1] = t;
	}

	// Control points
	this->ctrlPoints = new float[vSize * 3];
	for(int i = 0; i < vSize; i++)
	{
		int offset = i * 3;
		for(int j = 0; j < 3; j++)
		{
			ctrlPoints[offset + j] = controlPts[i]->getFloatv()[j];
		}
	}

	this->order = ord + 1;
	this->partsU = partsU;
	this->partsV = partsV;
	this->drawingMode = drawingMode;
}

Patch::Patch(int order, int partsU, int partsV, string drawingMode)
{
	this->order = order + 1;
	this->partsU = partsU;
	this->partsV = partsV;
	this->drawingMode = drawingMode;
}

void Patch::setControlPoints(float* v)
{
	this->ctrlPoints = v;
}

void Patch::setTexCoords(float* v)
{
	this->texCoords = v;
}

void Patch::draw()
{
	//glColor3f(1.0,1.0,1.0);
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order,  0.0, 1.0, 3 * order, order, ctrlPoints);
	//glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2                     ,  0.0, 1.0, 6, 2,  &nrmlcompon[0][0]);
	//glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, order,  0.0, 1.0, 2 * order, order, texCoords);

	glFrontFace(GL_CW);
	
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_MAP2_NORMAL);
	//glEnable(GL_MAP2_COLOR_4);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0); 

	// SEGUE-SE EXEMPLO DE UTILIZACAO DE "EVALUATORS"
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	if(hasTexture)
	{
		glEnable(GL_TEXTURE_2D);
		//texture->apply();
	}

	glEvalMesh2(drawingMode == "fill" ? GL_FILL : drawingMode == "line" ? GL_LINE : GL_POINT, 0, partsU, 0, partsV);

	glDisable(GL_MAP2_VERTEX_3);
	if(hasTexture) glDisable(GL_TEXTURE_2D);
	glEnable(GL_AUTO_NORMAL);

	glFrontFace(GL_CCW);
	//glDisable(GL_MAP2_NORMAL);
	//glDisable(GL_COLOR_MATERIAL);
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
	v[3] = 0.5; v[4] = 0; v[5] = 0.5;
	v[6] = -0.5; v[7] = 0; v[8] = -0.5;
	v[9] = 0.5; v[10] = 0; v[11] = -0.5;

	this->setControlPoints(v);

	float* tex = new float[8];

	tex[0] = 0; tex[1] = 0;
	tex[2] = 1; tex[3] = 0;
	tex[4] = 0; tex[5] = 1;
	tex[6] = 1; tex[7] = 1;

	this->setTexCoords(tex);
}

void Plane::draw()
{
	Patch::draw();
}

string Plane::Type()
{
	return "Plane";
}