#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "Primitives.h"
#include "Animation.h"

class Vehicle : public Primitive {
public:
	Vehicle();
	Vehicle(Animation *animation);

	string Type();
	void draw();
	void update(unsigned long t);

	void setAnimation(Animation * animation);

protected:
	Animation *animation;
};

#endif