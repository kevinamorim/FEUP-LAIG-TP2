#include "Vehicle.h"

Vehicle::Vehicle()
{
	// Animation
	this->createAnimation();
	//this->hasAnimation = false;

	// Object
	this->obj = vector<Primitive *>();

	this->createTube();
	this->createFront();
	this->createBack();
	this->createTail();
	this->createBridge();

	numPatches = obj.size();
}

void Vehicle::createAnimation()
{
	// Animations
	vector<Point3d *> points = vector<Point3d *>();
	points.push_back(new Point3d(0,0,0));
	points.push_back(new Point3d(20,-1,0));
	points.push_back(new Point3d(30,-1,15));
	points.push_back(new Point3d(30,0,30));

	ComposedAnimation* comp = new ComposedAnimation("composed");

	comp->addAnimation(new LinearAnimation("anim_1", 10, points));
	comp->addAnimation(new CircularAnimation("circ", 5, new Point3d(15, 0, 15), 20, -45, -180));

	this->setAnimation(comp);
	this->hasAnimation = true;
	//

	points.clear();
}

void Vehicle::createTube()
{
	vector<Point3d *> points = vector<Point3d *>();

	points.push_back(new Point3d(0, -0.2, -8));
	points.push_back(new Point3d(0, -1, -4));
	points.push_back(new Point3d(0, -1, 0));
	points.push_back(new Point3d(0, -1, 4));

	points.push_back(new Point3d(-0.2, -0.2, -8));
	points.push_back(new Point3d(-1, -1, -4));
	points.push_back(new Point3d(-1, -1, 0));
	points.push_back(new Point3d(-1, -1, 4));

	points.push_back(new Point3d(-0.2, 0.2, -8));
	points.push_back(new Point3d(-1, 1, -4));
	points.push_back(new Point3d(-1, 1, 0));
	points.push_back(new Point3d(-1, 1, 4));

	points.push_back(new Point3d(0, 0.2, -8));
	points.push_back(new Point3d(0, 1, -4));
	points.push_back(new Point3d(0, 1, 0));
	points.push_back(new Point3d(0, 1, 4));

	this->obj.push_back(new Patch(3, 10, 10, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0, 0.2, -8));
	points.push_back(new Point3d(0, 1, -4));
	points.push_back(new Point3d(0, 1, 0));
	points.push_back(new Point3d(0, 1, 4));

	points.push_back(new Point3d(0.2, 0.2, -8));
	points.push_back(new Point3d(1, 1, -4));
	points.push_back(new Point3d(1, 1, 0));
	points.push_back(new Point3d(1, 1, 4));

	points.push_back(new Point3d(0.2, -0.2, -8));
	points.push_back(new Point3d(1, -1, -4));
	points.push_back(new Point3d(1, -1, 0));
	points.push_back(new Point3d(1, -1, 4));

	points.push_back(new Point3d(0, -0.2, -8));
	points.push_back(new Point3d(0, -1, -4));
	points.push_back(new Point3d(0, -1, 0));
	points.push_back(new Point3d(0, -1, 4));

	this->obj.push_back(new Patch(3, 10, 10, points, "fill"));

	points.clear();
}

void Vehicle::createFront()
{
	vector<Point3d *> points = vector<Point3d *>();

	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(0, -0.5, 5));
	points.push_back(new Point3d(0, -1, 4.5));
	points.push_back(new Point3d(0, -1, 4));

	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(1, -0.5, 5));
	points.push_back(new Point3d(1, -1, 4.5));
	points.push_back(new Point3d(1, -1, 4));

	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(1, 0.5, 5));
	points.push_back(new Point3d(1, 1, 4.5));
	points.push_back(new Point3d(1, 1, 4));
	
	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(0, 0.5, 5));
	points.push_back(new Point3d(0, 1, 4.5));
	points.push_back(new Point3d(0, 1, 4));

	this->obj.push_back(new Patch(3, 10, 10, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(0, 0.5, 5));
	points.push_back(new Point3d(0, 1, 4.5));
	points.push_back(new Point3d(0, 1, 4));
	
	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(-1, 0.5, 5));
	points.push_back(new Point3d(-1, 1, 4.5));
	points.push_back(new Point3d(-1, 1, 4));

	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(-1, -0.5, 5));
	points.push_back(new Point3d(-1, -1, 4.5));
	points.push_back(new Point3d(-1, -1, 4));

	points.push_back(new Point3d(0, 0, 5));
	points.push_back(new Point3d(0, -0.5, 5));
	points.push_back(new Point3d(0, -1, 4.5));
	points.push_back(new Point3d(0, -1, 4));

	this->obj.push_back(new Patch(3, 10, 10, points, "fill"));

	points.clear();
}

void Vehicle::createBack()
{
	vector<Point3d *> points = vector<Point3d *>();

	points.push_back(new Point3d(0.2, 0, -8));
	points.push_back(new Point3d(0.2, -0.2, -8));
	points.push_back(new Point3d(0, -0.3, -8));

	points.push_back(new Point3d(0.2, 0.2, -8));
	points.push_back(new Point3d(0, 0, -8.2));
	points.push_back(new Point3d(-0.2, -0.2, -8));

	points.push_back(new Point3d(0, 0.3, -8));
	points.push_back(new Point3d(-0.2, 0.2, -8));
	points.push_back(new Point3d(-0.2, 0, -8));

	this->obj.push_back(new Patch(2, 4, 4, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0, 0.2, -8));
	points.push_back(new Point3d(-0.2, 0.2, -8));
	points.push_back(new Point3d(-0.2, 0, -8));

	points.push_back(new Point3d(0.2, 0.2, -8));
	points.push_back(new Point3d(0, 0, -7.8));
	points.push_back(new Point3d(-0.2, -0.2, -8));

	points.push_back(new Point3d(0.2, 0, -8));
	points.push_back(new Point3d(0.2, -0.2, -8));
	points.push_back(new Point3d(0, -0.2, -8));

	this->obj.push_back(new Patch(2, 4, 4, points, "fill"));

	points.clear();
}

void Vehicle::createTail()
{
	vector<Point3d *> points = vector<Point3d *>();

	points.push_back(new Point3d(0.8, 0, -6.6));
	points.push_back(new Point3d(0.8, 0, -7.4));
	points.push_back(new Point3d(0.8, 0, -7.4));
	points.push_back(new Point3d(0.8, 0, -6.6));

	points.push_back(new Point3d(0.6, 0, -6.6));
	points.push_back(new Point3d(0.6, 0.3, -7.4));
	points.push_back(new Point3d(0.6, -0.3, -7.4));
	points.push_back(new Point3d(0.6, 0, -6.6));

	points.push_back(new Point3d(-0.6, 0, -6.6));
	points.push_back(new Point3d(-0.6, 0.3, -7.4));
	points.push_back(new Point3d(-0.6, -0.3, -7.4));
	points.push_back(new Point3d(-0.6, 0, -6.6));
	
	points.push_back(new Point3d(-0.8, 0, -6.6));
	points.push_back(new Point3d(-0.8, 0, -7.4));
	points.push_back(new Point3d(-0.8, 0, -7.4));
	points.push_back(new Point3d(-0.8, 0, -6.6));

	this->obj.push_back(new Patch(3, 10, 10, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0, 0, -6.5));
	points.push_back(new Point3d(0.3, 0, -6.85));
	points.push_back(new Point3d(0, 0, -7.2));

	points.push_back(new Point3d(0, 0.5, -6.5));
	points.push_back(new Point3d(0.3, 0.5, -6.85));
	points.push_back(new Point3d(0, 0.5, -7.2));

	points.push_back(new Point3d(0, 1, -6.7));
	points.push_back(new Point3d(0, 1, -6.85));
	points.push_back(new Point3d(0, 1, -7));

	this->obj.push_back(new Patch(2, 10, 10, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0, 1, -6.7));
	points.push_back(new Point3d(0, 1, -6.85));
	points.push_back(new Point3d(0, 1, -7));

	points.push_back(new Point3d(0, 0.5, -6.5));
	points.push_back(new Point3d(-0.3, 0.5, -6.85));
	points.push_back(new Point3d(0, 0.5, -7.2));

	points.push_back(new Point3d(0, 0, -6.5));
	points.push_back(new Point3d(-0.3, 0, -6.85));
	points.push_back(new Point3d(0, 0, -7.2));

	this->obj.push_back(new Patch(2, 10, 10, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0.2, 0.7, -6.6));
	points.push_back(new Point3d(0.2, 0.7, -7.1));
	points.push_back(new Point3d(0.2, 0.7, -7.1));
	points.push_back(new Point3d(0.2, 0.7, -6.6));

	points.push_back(new Point3d(0.1, 0.7, -6.6));
	points.push_back(new Point3d(0.1, 1, -7.1));
	points.push_back(new Point3d(0.1, 0.5, -7.1));
	points.push_back(new Point3d(0.1, 0.7, -6.6));

	points.push_back(new Point3d(-0.1, 0.7, -6.6));
	points.push_back(new Point3d(-0.1, 1, -7.1));
	points.push_back(new Point3d(-0.1, 0.5, -7.1));
	points.push_back(new Point3d(-0.1, 0.7, -6.6));
	
	points.push_back(new Point3d(-0.2, 0.7, -6.6));
	points.push_back(new Point3d(-0.2, 0.7, -7.1));
	points.push_back(new Point3d(-0.2, 0.7, -7.1));
	points.push_back(new Point3d(-0.2, 0.7, -6.6));

	this->obj.push_back(new Patch(3, 10, 10, points, "fill"));

	points.clear();
}

void Vehicle::createBridge()
{
	vector<Point3d *> points = vector<Point3d *>();

	points.push_back(new Point3d(-0.2, 1.1, 3.5));
	points.push_back(new Point3d(0.2, 1.1, 3.5));
	points.push_back(new Point3d(-0.2, 0.9, -1.8));
	points.push_back(new Point3d(0.2, 0.9, -1.8));

	this->obj.push_back(new Patch(1, 3, 5, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0.2, 1.1, 3.5));
	points.push_back(new Point3d(0.4, 0.7, 3.5));
	points.push_back(new Point3d(0.2, 0.9, -1.8));
	points.push_back(new Point3d(0.4, 0.5, -2));

	this->obj.push_back(new Patch(1, 2, 5, points, "fill"));

	points.clear();

	points.push_back(new Point3d(-0.2, 0.9, -1.8));
	points.push_back(new Point3d(-0.4, 0.5, -2));
	points.push_back(new Point3d(-0.2, 1.1, 3.5));
	points.push_back(new Point3d(-0.4, 0.7, 3.5));

	this->obj.push_back(new Patch(1, 2, 5, points, "fill"));

	points.clear();

	points.push_back(new Point3d(-0.4, 0.7, 3.5));
	points.push_back(new Point3d(0.4, 0.7, 3.5));
	points.push_back(new Point3d(-0.2, 1.1, 3.5));
	points.push_back(new Point3d(0.2, 1.1, 3.5));

	this->obj.push_back(new Patch(1, 3, 2, points, "fill"));

	points.clear();

	points.push_back(new Point3d(0.4, 0.5, -2));
	points.push_back(new Point3d(-0.4, 0.5, -2));
	points.push_back(new Point3d(0.2, 0.9, -1.8));
	points.push_back(new Point3d(-0.2, 0.9, -1.8));

	this->obj.push_back(new Patch(1, 3, 2, points, "fill"));

	points.clear();
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
	if(hasAnimation)
	{
		animation->draw();
	}

	for(int i = 0; i < numPatches; i++)
	{
		obj.at(i)->draw();
	}
}

void Vehicle::update(unsigned long t)
{
	if(hasAnimation)
	{
		animation->update(t);
	}
}

void Vehicle::setAnimation(Animation * animation)
{
	this->animation = animation;
}