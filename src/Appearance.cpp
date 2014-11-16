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