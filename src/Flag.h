#ifndef _FLAG_H_
#define _FLAG_H_

#include "Patch.h"

#include "CGFshader.h"
#include "CGFapplication.h"

class FlagShader : public CGFshader {
	friend class Flag;
public:
	FlagShader(Texture* tex);

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

class Flag : public Plane {
public:
	Flag(Texture* tex);
	
	string Type();
	void draw();
	void update(unsigned long t);

	void setWind(int wind);

protected:
	FlagShader* shader;
};

#endif