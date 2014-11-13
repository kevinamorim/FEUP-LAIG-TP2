#include "Camera.h"

#include <string>

// ===================
//  CAMERA
// ===================
Camera::Camera(string id, float near, float far)
{
	this->id = id;
	this->near = near;
	this->far = far;

	this->aspect = 16.0 / 9.0; // default
}

string Camera::getID()
{
	return id;
}

void Camera::updateAspect()
{
	float* viewport = new float[4];

	// gets the viewport parameters
	// (x, y, width, height)
	glGetFloatv(GL_VIEWPORT, viewport);

	this->aspect = viewport[2] / viewport[3];
}

float* Camera::getFloat4v(float* v)
{
	float* v4 = new float[4];

	v4[0] = v[0];
	v4[1] = v[1];
	v4[2] = v[2];
	v4[3] = 1.0; // Required so that is not considered a directional light

	return v4;
}

// ===================
//  ORTHO CAMERA
// ===================
OrthoCamera::OrthoCamera(string id, char axis, float near, float far, float left, float right, float top, float bottom) : Camera(id, near, far)
{
	this->axis = axis;
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;

	this->mode = TARGET_MODE;
}

string OrthoCamera::Type()
{
	return "ortho";
}

void OrthoCamera::apply()
{
	updateAspect();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// creates an orthographic image of the scene
	glOrtho(left, right, bottom, top, near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(axis == 'x')
	{
		gluLookAt(far - 1, 0, 0, 0, 0, 0, 0, 1, 0);
	}
	else if(axis == 'y')
	{
		gluLookAt(0, far - 1, 0, 0, 0, 0, 1, 0, 0);
	}
	else if(axis == 'z')
	{
		gluLookAt(0, 0, far - 1, 0, 0, 0, 0, 1, 0);
	}
}

// ===================
//  PERSP CAMERA
// ===================
PerspCamera::PerspCamera(string id, float near, float far, float angle, Point3d* position, Point3d* target) : Camera(id, near, far)
{
	this->angle = angle;
	this->pos = position;
	this->tar = target;

	this->mode = TARGET_MODE;
}

string PerspCamera::Type()
{
	return "perspective";
}

void PerspCamera::apply()
{
	updateAspect();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// creates a perspective view of the scene
	gluPerspective(angle, aspect, near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pos->x, pos->y, pos->z, tar->x, tar->y, tar->z, 0, 1, 0);
}
