#ifndef _FLAG_H_
#define _FLAG_H_

#include "Patch.h"

#include "CGFshader.h"
#include "CGFapplication.h"

class FlagShader : public CGFshader {
public:
	FlagShader(Texture* tex);

	virtual void bind();
	virtual void unbind();

protected:
	Texture* baseTexture;
	int baseTextureLoc;
};

class Flag : public Plane {
public:
	Flag(Texture* tex);
	
	string Type();
	void draw();

protected:
	FlagShader* shader;
};

#endif