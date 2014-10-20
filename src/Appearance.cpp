#include "Appearance.h"

// =======================
//    Appearance
// =======================
Appearance::Appearance(string id, float shininess, Point4d* ambient, Point4d* diffuse, Point4d* specular, Texture* texture)
{
	this->id = id;

	this->hasTex = (texture != NULL);
	if(hasTex)
	{
		this->setTexture(texture);
	}
	this->texture = texture;

	this->setShininess(shininess);
	this->setAmbient(ambient->getFloatv());
	this->setDiffuse(diffuse->getFloatv());
	this->setSpecular(specular->getFloatv());
}

Appearance::~Appearance() { CGFappearance::~CGFappearance(); }

bool Appearance::hasTexture()
{
	return hasTex;
}

string Appearance::getID()
{
	return id;
}

Texture* Appearance::getTexture()
{
	return this->texture;
}

// =======================
//    TEXTURE
// =======================
Texture::Texture(string id, string file, float length_s, float length_t) : CGFtexture(file)
{
	this->id = id;
	this->file = file;
	this->textLength = new Point2d(length_s, length_t);
}

string Texture::getID()
{
	return this->id;
}

string Texture::getFile()
{
	return this->file;
}

float Texture::S()
{
	return this->textLength->X();
}

float Texture::T()
{
	return this->textLength->Y();
}