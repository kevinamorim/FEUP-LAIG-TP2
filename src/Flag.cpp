#include <GL/glew.h>

#include <cstdlib>

#include "Flag.h"

// =======================
//    Flag
// =======================
Flag::Flag(Texture* tex) : Plane(60)
{
	this->setTexture(tex);

	this->shader = new FlagShader(texture);
}

string Flag::Type()
{
	return "Flag";
}

void Flag::draw()
{
	this->shader->bind();

	Plane::draw();

	this->shader->unbind();
}

void Flag::update(unsigned long t)
{
	float time_s = t * 0.001; // <-- seconds

	this->shader->setTime(time_s);
}

void Flag::setWind(int wind)
{
	this->shader->wind = wind;
}

// =======================
//    Flag Shader
// =======================
FlagShader::FlagShader(Texture* tex)
{
	this->baseTexture = tex;

	init("data/flagShader.vert", "data/flagShader.frag");

	CGFshader::bind();

	// get the uniform location for the sampler
	baseTextureLoc = glGetUniformLocation(id(), "baseTexture");

	// set the texture id for that sampler to match the GL_TEXTUREn that you 
	// will use later e.g. if using GL_TEXTURE0, set the uniform to 0
	glUniform1i(baseTextureLoc, 0);

	// initialize in memory
	this->startTime = 0.0;
	this->deltaTime = 0.0;
	this->wind = 0;

	windLoc = glGetUniformLocation(id(), "wind");
	glUniform1f(windLoc, wind);

	deltaTimeLoc = glGetUniformLocation(id(), "deltaTime");
	glUniform1f(deltaTimeLoc, deltaTime);

	CGFshader::unbind();
}

void FlagShader::bind()
{
	CGFshader::bind();

	glUniform1f(windLoc, wind);
	glUniform1f(deltaTimeLoc, deltaTime);

	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE0);

	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	baseTexture->apply();
}

void FlagShader::unbind()
{
	CGFshader::unbind();
}

void FlagShader::setTime(float time)
{
	if(this->startTime == 0)
	{
		this->startTime = time;
	}
	else
	{
		this->deltaTime = time - startTime;
	}
}