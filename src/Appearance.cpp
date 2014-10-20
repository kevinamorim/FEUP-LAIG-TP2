#include "Appearance.h"

// =======================
//    Appearance
// =======================
Appearance::Appearance(string id, float shininess, Texture* texture, Point4d* ambient, Point4d* diffuse, Point4d* specular)
{
	this->id = id;
	this->shininess = shininess;
	this->texture = texture;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->tex = true;
}

Appearance::Appearance(string id, float shininess, Point4d* ambient, Point4d* diffuse, Point4d* specular)
{
	this->id = id;
	this->shininess = shininess;
	this->texture = texture;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->tex = false;
}

bool Appearance::hasTexture()
{
	return this->tex;
}

string Appearance::getID()
{
	return id;
}

float Appearance::getShininess()
{
	return shininess;
}

Texture* Appearance::getTexture()
{
	return texture;
}

Point4d* Appearance::getAmbient()
{
	return ambient;
}

Point4d* Appearance::getDiffuse()
{
	return diffuse;
}

Point4d* Appearance::getSpecular()
{
	return specular;
}

// =======================
//    TEXTURE
// =======================
Texture::Texture(string id, string file, float length_s, float length_t)
{
	this->id = id;
	this->file = file;

	this->texlength_s = length_s;
	this->texlength_t = length_t;
}

string Texture::getID()
{
	return this->id;
}

string Texture::getFile()
{
	return this->file;
}

float Texture::getTexLengthS()
{
	return this->texlength_s;
}

float Texture::getTexLengthT()
{
	return this->texlength_t;
}