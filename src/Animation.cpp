#include "Animation.h"

float distance(Point3d * a, Point3d * b);
float distance(vector<Point3d *> pts);
float distance(int axis, Point3d * a, Point3d * b);

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
	this->controlPoints = controlPts;

	float dist = distance(controlPoints);

	this->velocity = dist / spanTime;

	cout << "Distance: " << dist << endl;
	cout << "Time Span: " << this->spanTime << endl;		
	cout << "Velocity: " << this->velocity << endl;

	this->currentControl = 0;
	this->currentPos = this->controlPoints[0];

	this->reset();
}

void LinearAnimation::init(unsigned long t)
{
	this->currentPos = this->controlPoints[0];
	//this->currentDir = Point3d::subtract(controlPoints[1], controlPoints[0]);
	this->currentControl = 1;
	this->restart = false;

	this->oldTime = t;
}

void LinearAnimation::draw()
{
	// TODO : rotate
	//cout << "currentPos: " << currentPos->x << " " << currentPos->y << " " << currentPos->z << endl;
	glTranslatef(currentPos->x, currentPos->y, currentPos->z);
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
		int length = controlPoints.size();

		if(currentControl < length)
		{
			//cout << "UPDATE" << endl;

			float delta = distance(currentPos, controlPoints[currentControl]);

			if(delta > 0)
			{
				float deltaX = distance(0, currentPos, controlPoints[currentControl]);
				float deltaY = distance(1, currentPos, controlPoints[currentControl]);

				float normX = deltaX / delta;
				float normY = deltaY / delta;

				float deltaTime = (t - oldTime) / 1000.0;

				float moveX = this->velocity * deltaTime * normX;
				float moveY = this->velocity * deltaTime * normY;

				this->oldTime = t;

				currentPos->x += moveX;
				currentPos->y += moveY;

				//cout << "delta: " << delta << endl;
				//cout << "normX: " << normX << endl;
				//cout << "normY: " << normY << endl;
				cout << "moveX: " << moveX << endl;
				cout << "moveY: " << moveY << endl;

				if(abs(moveX) >= deltaX || abs(moveY) >= deltaY)
				{
					cout << "true" << endl;
					currentPos = controlPoints[currentControl];
					currentControl++;
				}
				else
				{
					reset();
				}
			}
		}
		else
		{
			reset();
		}
	}
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

	// TODO : velocities

	this->reset();
}

void CircularAnimation::init(unsigned long t)
{
	this->currentRotate = 0 ;// TODO
	this->currentPos = 0; // TODO
	this->restart = false;
}

void CircularAnimation::draw()
{
	// TODO
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
		// TODO
	}
}

// =======================
// =======================
float distance(Point3d * a, Point3d * b)
{
	return sqrt(abs((b->x * b->x - a->x * a->x) + (b->y * b->y - a->y * a->y) + (b->z * b->z - a->z * a->z)));
}

float distance(vector<Point3d *> pts)
{
	float dist = 0.0;

	unsigned int size = pts.size();

	for(unsigned int i = 0; i < size - 1; i++)
	{
		dist += distance(pts[i], pts[i+1]);
	}

	return dist;
}

float distance(int axis, Point3d * a, Point3d * b)
{
	switch(axis)
	{
	case 0: // x
		return sqrt(b->x * b->x - a->x * a->x);

	case 1:
		return sqrt(b->y * b->y - a->y * a->y);

	case 2:
		return sqrt(b->z * b->z - a->z * a->z);

	default:
		return 0;
	}
}