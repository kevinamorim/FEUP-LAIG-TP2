#include <GL/glew.h>

#include <cstdlib>

#include "Sand.h"

// =======================
//    Flag
// =======================
Sand::Sand(Texture* tex) : Plane(60)
{
	this->setTexture(tex);

	this->shader = new SandShader(texture);
}

string Sand::Type()
{
	return "Sand";
}

void Sand::draw()
{
	this->shader->bind();

	Plane::draw();

	this->shader->unbind();
}

void Sand::update(unsigned long t)
{
	//
}

// =======================
//    Sand Shader
// =======================
SandShader::SandShader(Texture* tex)
{
	this->baseTexture = tex;
	this->heightMap = new Texture("sand_height_map","textures/sand_height_map.png", 1, 1); 

	init("data/sandShader.vert", "data/sandShader.frag");

	CGFshader::bind();

	// get the uniform location for the sampler
	baseTextureLoc = glGetUniformLocation(id(), "baseTexture");
	// set the texture id for that sampler to match the GL_TEXTUREn that you 
	// will use later e.g. if using GL_TEXTURE0, set the uniform to 0
	glUniform1i(baseTextureLoc, 0);

	heightMapLoc = glGetUniformLocation(id(), "heightMap");
	glUniform1i(heightMapLoc, 1);
}

void SandShader::bind()
{
	CGFshader::bind();

	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE1);
	heightMap->apply();

	glActiveTexture(GL_TEXTURE0);
	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	baseTexture->apply();
}

void SandShader::unbind()
{
	CGFshader::unbind();
}