#include "Animation.h"

// =======================
//	Animation
// =======================
Animation::Animation(string id, float span)
{
	this->id = id;
	this->spanTime = span;
}

string Animation::getID()
{
	return this->id;
}

// =======================
//	Linear Animation
// =======================
LinearAnimation::LinearAnimation(string id, float span, vector<Point3d *> controlPts) : Animation(id, span)
{
	this->controlPoints =  vector<Point3d *>();
	this->direction = vector<Point3d *>();
	this->distance = vector<float>();

	int size = controlPts.size();

	for(int i = 0; i < size; i++)
	{
		// This is done so that new points are used
		//	(instead of the passed points, so that we don't access memory areas we're not supposed to be accessing)
		Point3d * p = new Point3d();
		p->setPoint3d(controlPts[i]);
		
		controlPoints.push_back(p);
	}
		

	float totalDistance = 0;

	// Let's calculate the direction vector and the distance between the control points
	//	(first will be ignored, for it is the starting point)
	this->direction.push_back(new Point3d(0,0,0));
	this->distance.push_back(0);

	for(int i = 1; i < size; i++)
	{
		float dist = Point3d::distance(controlPoints[i], controlPoints[i-1]);
		Point3d * p = Point3d::subtract(controlPoints[i], controlPoints[i-1]);

		// normalize
		p->x /= dist;
		p->y /= dist;
		p->z /= dist;

		this->direction.push_back(p);
		this->distance.push_back(dist);

		//cout << "Direction [" << i-1 << "->" << i << "] : " << this->direction.at(i) << endl;
		//cout << " Distance [" << i-1 << "->" << i << "] : " << this->distance.at(i) << endl;

		totalDistance += distance[i];
	}
	
	// constant object speed (u/s)
	this->speed = totalDistance / spanTime;

	this->currentPos = new Point3d();
	this->currentPos->setPoint3d(controlPoints.at(0));
	this->currentControl = 1;
	this->currentAngle = getAngle();

	this->reset();
}

void LinearAnimation::init(unsigned long t)
{
	this->currentPos->setPoint3d(this->controlPoints.at(0));
	this->currentControl = 1;
	this->moved = 0;

	this->currentAngle = getAngle();

	this->oldTime = t;

	this->restart = false;
}

void LinearAnimation::draw()
{
	glTranslatef(currentPos->x, currentPos->y, currentPos->z);

	glRotatef(currentAngle->x, 0, 1, 0);
	glRotatef(currentAngle->y, 1, 0, 0);
}

void LinearAnimation::reset()
{
	this->restart = true;
}

void LinearAnimation::update(unsigned long t)
{
	if(restart)
	{
		init(t);
	}
	else
	{
		float deltaTime = (t - oldTime) / 1000.0;
		this->oldTime = t;

		float deltaMovement = this->speed * deltaTime;

		move(deltaMovement);

		moved += deltaMovement;

		if(moved >= distance[currentControl])
		{
			moved -= distance[currentControl];

			currentPos->setPoint3d(controlPoints[currentControl]);
			currentControl = (currentControl + 1) % controlPoints.size();

			if(currentControl == 0)
			{
				init(t);
			}
			else 
			{
				currentAngle = getAngle();
				move(moved);
			}
		}
	}
}

Point3d * LinearAnimation::getAngle()
{
	float angleX, angleY, angleZ;

	Point2d vecZ = Point2d(0, 1);
	Point2d vecY = Point2d(direction[currentControl]->y, 1);
	Point2d vecD = Point2d(direction[currentControl]->x, direction[currentControl]->z);

	float cosX = Point2d::dotProduct(&vecZ, &vecD) / (vecZ.size() * vecD.size());
	float cosY = Point2d::dotProduct(&vecY, &vecZ) / (vecY.size() * vecZ.size());

	angleX = 360 * acos(cosX) / (2 * PI);
	angleX *= (direction[currentControl]->z >= 0 && direction[currentControl]->x >= 0) ? 1 : -1;
	//angleX = 0;

	//angleY = 360 * acos(cosY) / (2 * PI);
	//angleY *= (direction[currentControl]->y >= 0) ? -1 : 1;	// the rotation in x will be anti-clockwise, therefore the angle must me inverted
	angleY = 0;

	angleZ = 0;

	return new Point3d(angleX, angleY, angleZ);
}

void LinearAnimation::move(float distance)
{
	this->currentPos->x += distance * direction[currentControl]->x;
	this->currentPos->y += distance * direction[currentControl]->y;
	this->currentPos->z += distance * direction[currentControl]->z;
}

// =======================
//	Circular Animation
// =======================
CircularAnimation::CircularAnimation(string id, float span, Point3d * center, float radius, float startAng, float rotAng) : Animation(id, span)
{
	this->center = center;
	this->radius = radius;
	this->startAngle = startAng;
	this->rotateAngle = rotAng;

	this->angularSpeed = (rotateAngle / spanTime); // angle/s

	//float dist = 2 * PI * rotateAngle / 360.0;
	//this->linearSpeed = dist / spanTime;

	this->reset();
}

void CircularAnimation::init(unsigned long t)
{
	this->currentRotate = startAngle ;
	this->currentOffset = radius;

	this->restart = false;

	this->oldTime = t;
}

void CircularAnimation::draw()
{
	glRotatef(currentRotate, 0, 1, 0);
	glTranslatef(radius, 0, 0);
}

void CircularAnimation::reset()
{
	this->restart = true;
}

void CircularAnimation::update(unsigned long t)
{
	if(restart)
	{
		init(t);
	}
	else
	{
		float deltaTime = (t - oldTime) / 1000.0;
		this->oldTime = t;

		currentRotate += angularSpeed * deltaTime;

		if(currentRotate >= (startAngle + rotateAngle))
		{
			init(t);
		}
	}
}
