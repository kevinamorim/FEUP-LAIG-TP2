#include <GL/glew.h>

#include <cstdlib>

#include "Water.h"

// =======================
//    Flag
// =======================
Water::Water(Texture* tex) : Plane(50)
{
	this->setTexture(tex);

	this->shader = new WaterShader(texture);
}

string Water::Type()
{
	return "Water";
}

void Water::draw()
{
	this->shader->bind();

	//cout << this->shader->deltaTime << endl;

	Plane::draw();

	this->shader->unbind();
}

void Water::update(unsigned long t)
{
	float time_s = t * 0.001; // <-- seconds

	this->shader->setTime(time_s);
}

void Water::setWind(int wind)
{
	this->shader->wind = wind;
}

// =======================
//    Water Shader
// =======================
WaterShader::WaterShader(Texture* tex)
{
	this->baseTexture = tex;

	init("data/waterShader.vert", "data/waterShader.frag");

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

void WaterShader::bind()
{
	CGFshader::bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(windLoc, wind);
	glUniform1f(deltaTimeLoc, deltaTime);

	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE0);

	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	baseTexture->apply();
}

void WaterShader::unbind()
{
	glDisable(GL_BLEND);
	CGFshader::unbind();
}

void WaterShader::setTime(float time)
{
	if(this->startTime <= 0.0)
	{
		this->startTime = time;
	}
	else
	{
		this->deltaTime = time - startTime;
	}
}