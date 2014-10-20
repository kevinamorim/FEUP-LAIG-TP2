#include "Transform.h"

const int X = 0;
const int Y = 1;
const int Z = 2;

Transform::Transform() { }

Transform::~Transform() { }

// =======================
// TRANSLATE
// =======================
Translate::Translate(Point3d* to)
{
	this->good = false;

	this->to = to;

	this->good = true;
}

void Translate::apply()
{
	glTranslatef(to->X(), to->Y(), to->Z());
}

void Translate::print()
{
	printf("Translate: ");
	if(isGood())
	{
		printf("(%.1f, %.1f, %.1f)\n", to->X(), to->Y(), to->Z());
	}
	else
	{
		printf("[ERROR] > Translate is faulty\n");
	}
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

void Rotate::print()
{
	printf("Rotate: ");
	if(isGood())
	{
		printf("(%c, %.1f)\n", axis, angle);
	}
	else
	{
		printf("[ERROR] > Rotate is faulty\n");
	}
}

// =======================
// SCALE
// =======================
Scale::Scale(Point3d* factor) {

	this->good = false;

	this->factor = factor;

	this->good = true;
}

void Scale::apply()
{
	glScalef(factor->X(), factor->Y(), factor->Z());
}

void Scale::print()
{
	printf("Scale: ");
	if(isGood())
	{
		printf("(%.1f, %.1f, %.1f)\n", factor[X], factor[Y], factor[Z]);
	}
	else
	{
		printf("[ERROR] > Translate is faulty\n");
	}
}