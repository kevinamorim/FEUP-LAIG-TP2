#ifndef _APPEARANCE_H_
#define _APPEARANCE_H_

#include "CGFappearance.h"
#include "Texture.h"

#include <string>

using namespace std;

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