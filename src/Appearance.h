#ifndef _APPEARANCE_H_
#define _APPEARANCE_H_

#include "CGFappearance.h"
#include "Point.h"

#include <iostream>
#include <string>

using namespace std;

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

/*
	Appearance
*/
class Appearance : public CGFappearance {
public:
	Appearance(string id, float shininess, Point4d* ambient, Point4d* diffuse, Point4d* specular, Texture* texture = NULL);
	~Appearance();

	string getID();

	Texture* getTexture();

	bool hasTexture();

protected:
	string id;

	bool hasTex;

	Texture* texture;
};

#endif