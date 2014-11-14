#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "CGFapplication.h"
#include "CGFscene.h"
#include "CGFshader.h"

#include "SceneData.h"

#include <vector>

using namespace std;

/*
	CGFscene that will represent the scene read from the anf file
*/
class MainScene : public CGFscene {
public:
	MainScene(SceneData* sceneData);

	void init();
	void display();
	void update(unsigned long t);

	// Set methods used by the interface
	void toggleLight(int index, int value);
	void toggleCamera(int index);
	void toggleDrawingMode(int mode);
	void toggleUseDL(int value);

	// Get Methods
	int getNumberOfLights();
	int getNumberOfCameras();

	vector<Light* > getLights();
	vector<Camera* > getCameras();

	Light* getLight(int index);
	Camera* getCamera(int index);

protected:
	void initGlobals();		// Initializes the scene global configurations
	void initCameras();		// Initializes the scene cameras' variables (does not instantiate the cameras themselves, since those are already present in the sceneData)
	void initLights();		// Initializes the scene lights' variables (does not instantiate the cameras themselves, since those are already present in the sceneData)

	void setDefaults();		// Sets some default values on each display() calling, such as clearing depth and color arrays and calling the glLoadIdentity() method for the modelview matrix

	void updateLights();	// Updates the lights in the scene (sets them enabled or disabled, updates position, direction, etc..)
	void updateCameras();	// Updates the active camera in the scene (position, angle, target, etc...)
	void updateDrawing();	// Checks if the drawing mode has been altered and acts accordingly

	void toggleDrawingMode(string mode);												// Sets the drawing mode [fill, line, point]
	void toggleDrawingShading(string shading);											// Sets the sading method used [flat, gouraud]
	void toggleCulling(string face, string order);										// Sets the culling attributes and wether it is used or not
	void toggleLighting(bool doublesided, bool local, bool enabled, float* ambient);	// Sets the general lighting parameters for the scene

private:
	// All the sceneData received from the parser already in its data structures.
	// Contains:
	//  - Globals
	//  - Lights
	//	- Cameras
	//	- Appearances
	//	- Textures
	//	- Graph
	SceneData* sceneData;

	int activeCamera;	// active Camera's index
	
	// The following are used only for ease of access and reading (since all of this data is in the sceneData instance)
	int numCameras;
	int numLights;

	ComposedAnimation* anim;
};

#endif