#include "Transforms.h"

// =======================
// TRANSLATE
// =======================
Translate::Translate(Point3d* to)
{
	this->to = to;
}

void Translate::apply()
{
	glTranslatef(to->X(), to->Y(), to->Z());
}

// =======================
// ROTATE
// =======================
Rotate::Rotate(char axis, float angle) {

	this->good = false;

	this->angle = angle;
	this->axis = axis;

	this->good = true;
}

void Rotate::apply()
{
	switch(axis)
	{
	case 'x':
		glRotatef(angle, 1, 0, 0);
		break;
	case 'y':
		glRotatef(angle, 0, 1, 0);
		break;
	case 'z':
		glRotatef(angle, 0, 0, 1);
		break;
	}
}

// =======================
// SCALE
// =======================
Scale::Scale(Point3d* factor)
{
	this->factor = factor;
}

void Scale::apply()
{
	glScalef(factor->X(), factor->Y(), factor->Z());
}