#ifndef _SCENE_DATA_H_
#define _SCENE_DATA_H_

#include "Primitives.h"
#include "CGFApplication.h"

#include "Point.h"
#include "Camera.h"
#include "Light.h"
#include "Appearance.h"

#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include <iomanip>

using namespace std;

/*
	SceneNode
*/
class SceneNode {
private:
	string id;

	SceneNode* parent;
	bool hasParent;

	float* transform; // Tranform matrix

	Appearance* appearance;

	bool hasAppearance;

	std::vector<Primitive*> primitives;
	std::vector<SceneNode*> descendants;

protected:
	stack<Appearance*> *appearancesStack;

	void addAppearanceToStack(Appearance* app);
	void removeAppearanceFromStack();

public:
	bool inherits;
	bool visited;

	SceneNode(string nodeID);
	~SceneNode();

	void setAppearancesStack(stack<Appearance*> * apps);

	void setParent(SceneNode* node);
	void setTransformMatrix();
	void setAppearance(Appearance* appearance);

	void addDescendant(SceneNode* descendant);
	void addPrimitive(Primitive* primitive);

	// GETTERS & SETTERS
	string getID();

	vector<Primitive*> getPrimitives();
	vector<SceneNode*> getDescendants();

	float* getTransformMatrix();

	Appearance* getAppearance();
	Texture* getTexture();
	SceneNode* getParent();

	void Process();

	bool Verify(ostream & out);
};


/*
	SceneGraph
*/
class SceneGraph {
private:
	// The root node belong to the vector nodes. However, for ease of access, it shall have its own pointer reference.
	SceneNode* root;

	vector<SceneNode*> nodes;

	stack<Appearance*> appearancesStack;

public:
	SceneGraph();
	~SceneGraph();

	SceneNode* getRoot();

	void setRoot(SceneNode* node);
	void addNode(SceneNode* node);

	bool hasRoot();

	// Accessory functions
	SceneNode* getNode(string id);
	vector<SceneNode*> getNodes();

	void Process();

	int Verify(ostream & out);

	int addAppearance(Appearance* app);
	int removeAppearance();
};


/*
	SceneData
*/
class SceneData {
public:
	SceneData();

	// Globals
	void setDrawing(string mode, string shading, Point4d* background);
	void setCulling(string face, string order);
	void setLighting(bool doublesided, bool local, bool enabled, Point4d* ambient);

	// Cameras
	void addCamera(Camera* cam);
	void addCameraInitial(Camera* cam);

	Camera* getInitialCamera(); // Returns camera at index = 0 (is the one defined in the .anf file)
	Camera* getCamera(unsigned int index);
	vector<Camera* > getCameras();

	bool hasInitialCamera();
	bool hasCameras();

	// Lights
	void addLight(Light* light);

	Light* getLight(unsigned int index);
	vector<Light* > getLights();

	bool hasLights();

	// Graph
	SceneGraph* getSceneGraph();

	// Drawing
	string drawing_mode;
	string drawing_shading;
	Point4d* drawing_background;

	// Culling
	string culling_face;
	string culling_order;
	Point4d* lighting_ambient;

	// Lighting
	bool lighting_doublesided;
	bool lighting_local;
	bool lighting_enabled;

	// Appearances
	Appearance* getAppearance(string id);
	void addAppearance(Appearance * app);
	Texture* getTexture(string id);
	void addTexture(Texture * tex);

private:
	SceneGraph* sceneGraph;

	vector<Camera* > cameras;
	bool initialCameraSet;

	vector<Light* > lights;

	vector<Appearance *> appearances;
	vector<Texture *> textures;
};

#endif