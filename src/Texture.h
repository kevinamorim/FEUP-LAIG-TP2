#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Point.h"

#include <CGFtexture.h>

/*
	Texture
*/
class Texture : public CGFtexture {
public:
	Texture(string id, string file, float texlength_s, float texlength_t);

	string getID();
	string getFile();

	float S();
	float T();

protected:
	string id;
	string file;

	Point2d* textLength;
};

#endif