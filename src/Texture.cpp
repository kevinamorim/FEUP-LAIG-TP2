#include "Texture.h"

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
	return this->textLength->x;
}

float Texture::T()
{
	return this->textLength->y;
}