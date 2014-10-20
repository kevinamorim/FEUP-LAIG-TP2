#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "CGFapplication.h"
#include "CGFcamera.h"

#include "Point.h"

#include <iostream>
#include <string>

using namespace std;

/*
	Represents the points of view (POV) in the scene.
*/
class Camera : public CGFcamera {
public:
	Camera(string id, float near, float far);

	// Get Methods
	string getID(); 

	// Virtual Methods
	virtual string Type() = 0;			// [perspective, ortho(graphic)]
	virtual void apply() = 0;			// Updates the position, orientation, size, angle int the projection and modelview matrices

	void updateAspect();				// Gets the aspect from the openGl viewport

	// Auxiliary Methods
	float* getFloat4v(float* v);		// resultv = (v[0], v[1], v[2], 1.0)

protected:
	string id;			

	float near;			// Min distance of rendering (from the camera to this distance all will be ingored)
	float far;			// Max distance of rendering (beyond this distance all will be ingored)
	float aspect;		// Ratio aspect of the camera
};

class OrthoCamera : public Camera {
public:
	OrthoCamera(string id, char axis, float near, float far, float left, float right, float top, float bottom);
	
	string Type();

	void apply();

private:
	char axis;			// axis in which the camera is "inserted", a.k.a. the axis in which the image will be flatened (loss of distance perception)
	float left;			// ------------------------------
	float right;		// Dimensions of the camera's box
	float top;			//
	float bottom;		// ------------------------------
};

class PerspCamera : public Camera {
public:
	PerspCamera(string id, float near, float far, float angle, Point3d* position, Point3d* target);

	string Type();

	void apply();

private:
	float angle;		// angle of the camera "lens"
	Point3d* pos;		// camera position
	Point3d* tar;		// target point
};

#endif