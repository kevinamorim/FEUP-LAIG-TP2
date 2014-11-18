#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "Primitives.h"
#include "Patch.h"
#include "Animation.h"

class Vehicle : public Primitive {
public:
	Vehicle();
	Vehicle(Animation *animation);

	string Type();
	void draw();
	void update(unsigned long t);

	void setAnimation(Animation * animation);
	void resetAnimation();

	// create animation
	void createAnimation();

	// create object
	void createTube();
	void createFront();
	void createBack();
	void createTail();
	void createBridge();
	void createBridgeTower();

protected:
	Animation *animation;
	bool hasAnimation;

	vector<Primitive *> obj;
	int numPatches;
};

#endif