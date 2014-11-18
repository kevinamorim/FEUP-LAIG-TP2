#ifndef _SAND_H_
#define _SAND_H_

#include "Patch.h"

#include "CGFshader.h"
#include "CGFapplication.h"

class SandShader : public CGFshader {
	friend class Sand;
public:
	SandShader(Texture* tex);

	virtual void bind();
	virtual void unbind();

protected:
	Texture* baseTexture;
	int baseTextureLoc;

	Texture* heightMap;
	int heightMapLoc;
};

class Sand : public Plane {
public:
	Sand(Texture* tex);
	
	string Type();
	void draw();
	void update(unsigned long t);

protected:
	SandShader* shader;
};

#endif