#ifndef _APPEARANCE_H_
#define _APPEARANCE_H_

#include "Point.h"

#include <iostream>
#include <string>

using namespace std;

/*
	Texture
*/
class Texture {
public:
	Texture(string id, string file, float texlength_s, float texlength_t);
	~Texture();

	string getID();
	string getFile();

	float getTexLengthS();
	float getTexLengthT();

protected:
	string id;
	string file;

	float texlength_s;
	float texlength_t;
};

/*
	Appearance
*/
class Appearance {
public:
	Appearance(string id, float shininess, Texture* texture, Point4d* ambient, Point4d* diffuse, Point4d* specular);
	Appearance(string id, float shininess, Point4d* ambient, Point4d* diffuse, Point4d* specular);
	~Appearance();

	string getID();

	Texture* getTexture();

	float getShininess();

	Point4d* getAmbient();
	Point4d* getDiffuse();
	Point4d* getSpecular();

	bool hasTexture();

protected:
	string id;

	bool tex;
	Texture* texture;

	float shininess;

	Point4d* ambient;
	Point4d* diffuse;
	Point4d* specular;
};

#endif