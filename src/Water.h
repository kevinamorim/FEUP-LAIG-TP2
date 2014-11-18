#ifndef _WATER_H_
#define _WATER_H_

#include "Patch.h"

#include "CGFshader.h"
#include "CGFapplication.h"

class WaterShader : public CGFshader {
	friend class Water;
public:
	WaterShader(Texture* tex);

	virtual void bind();
	virtual void unbind();

	void setTime(float time);

protected:
	Texture* baseTexture;
	int baseTextureLoc;

	int wind;
	int windLoc;

	float deltaTime;
	int deltaTimeLoc;

	float startTime;
};

class Water : public Plane {
public:
	Water(Texture* tex);
	
	string Type();
	void draw();
	void update(unsigned long t);

	void setWind(int wind);

protected:
	WaterShader* shader;
};

#endif