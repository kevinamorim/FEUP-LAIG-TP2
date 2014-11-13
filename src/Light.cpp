#include "Light.h"

Light::Light(unsigned int lightID, string name, bool enabled, bool marker, Point3d* pos, Point4d* ambient, Point4d* diffuse, Point4d* specular)
	: CGFlight(lightID, getFloat4v(pos->getFloatv()))
{
	this->name = name;

	this->marker = marker;

	this->setAmbient(getFloat4v(ambient->getFloatv()));
	this->setDiffuse(getFloat4v(diffuse->getFloatv()));
	this->setSpecular(getFloat4v(specular->getFloatv()));

	this->position[0] = pos->x;
	this->position[1] = pos->y;
	this->position[2] = pos->z;
	this->position[3] = 1.0;

	this->setEnabled(enabled);
}

Light::~Light()
{
	CGFlight::~CGFlight();
}

string Light::getName()
{
	return name;
}

void Light::setEnabled(bool value)
{
	this->enabled = value;
	
	enabled ? this->enable() : this->disable();
}

void Light::setID(unsigned int id)
{
	this->id = id;
}

bool Light::Enabled()
{
	return this->enabled;
}

// ===================
//  OMNI
// ===================
OmniLight::OmniLight(unsigned int lightID, string name, bool enabled, bool marker, Point3d* pos, Point4d* ambient, Point4d* diffuse, Point4d* specular)
	: Light(lightID, name, enabled, marker, pos, ambient, diffuse, specular)
{
	// Nothing
}

string OmniLight::Type()
{
	return "omni";
}

void OmniLight::updateLight()
{
	if(marker && enabled)
	{
		CGFlight::draw();
	}

	CGFlight::update();
}

// ===================
//  SPOT
// ===================
SpotLight::SpotLight(unsigned int lightID, string name, bool enabled, bool marker, Point3d* pos, Point3d* tar, float angle, float exp, Point4d* ambient, Point4d* diffuse, Point4d* specular)
	: Light(lightID, name, enabled, marker, pos, ambient, diffuse, specular)
{
	this->target = tar;
	this->angle = angle;
	this->exponent = exp;
}

string SpotLight::Type()
{
	return "spot";
}

void SpotLight::updateLight()
{
	if(marker && enabled)
	{
		CGFlight::draw();
	}

	CGFlight::update();

	this->setAngle(this->angle);

	glLightfv(id, GL_SPOT_DIRECTION, subtractFloatv(target->getFloatv(), position));
	glLightf(id, GL_SPOT_EXPONENT, exponent);	
}

// *********************************************
// *********************************************
float* Light::getFloat4v(float* v)
{
	float* resv = new float[4];

	resv[0] = v[0];
	resv[1] = v[1];
	resv[2] = v[2];
	resv[3] = 1.0; // Required so that is not considered a directional light

	return resv;
}

float* Light::subtractFloatv(float* v1, float* v2)
{
	float* resv = new float[3];

	resv[0] = v1[0] - v2[0];
	resv[1] = v1[1] - v2[1];
	resv[2] = v1[2] - v2[2];

	return getFloat4v(resv);
}