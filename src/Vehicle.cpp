#include "Vehicle.h"

Vehicle::Vehicle()
{

}

Vehicle::Vehicle(Animation * animation)
{
	this->animation = animation;
}

string Vehicle::Type()
{
	return "Vehicle";
}

void Vehicle::draw()
{
	this->animation->draw();
}

void Vehicle::update(unsigned long t)
{
	this->animation->update(t);
}

void Vehicle::setAnimation(Animation * animation)
{
	this->animation = animation;
}