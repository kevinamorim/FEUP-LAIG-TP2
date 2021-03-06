#include "MainScene.h"

#include <windows.h>

MainScene::MainScene(SceneData* sceneData)
{
	this->sceneData = sceneData;

	this->numLights = 0;
}

// **************************************************************
void MainScene::init()
{
	// Enables vector normalization
	glEnable(GL_NORMALIZE);

	initGlobals();
	initCameras();
	initLights();

	unsigned long updatePeriod = 50;
	setUpdatePeriod(updatePeriod);

	display();
}

void MainScene::initGlobals()
{
	// Drawing
	toggleDrawingMode(sceneData->drawing_mode);
	toggleDrawingShading(sceneData->drawing_shading);

	Point4d* backColor = sceneData->drawing_background;
	glClearColor(backColor->x, backColor->y, backColor->z, backColor->w);

	// Culling
	toggleCulling(sceneData->culling_face, sceneData->culling_order);

	// Lighting
	toggleLighting(sceneData->lighting_doublesided, sceneData->lighting_local, sceneData->lighting_enabled, sceneData->lighting_ambient->getFloatv());

	// Defines a default normal
	glNormal3f(0,1,0);
}

void MainScene::initLights()
{
	this->numLights = sceneData->getLights().size();
}

// **************************************************************
void MainScene::display()
{
	setDefaults();

	updateDrawing();

	// update scene camera
	CGFscene::activeCamera->applyView();

	updateLights();

	long double time_0 = GetTickCount();

	sceneData->getSceneGraph()->Display();

	long double time_1 = GetTickCount();

	//cout << "Time between: " << time_1 - time_0 << endl;

	axis.draw();

	glNormal3f(0, 1, 0);

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

void MainScene::update(unsigned long t)
{
	this->sceneData->getSceneGraph()->Update(t);
}

void MainScene::setDefaults()
{
	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// **************************************************************
void MainScene::updateLights()
{
	for(int i = 0; i < numLights; i++)
	{
		sceneData->getLight(i)->updateLight();
	}
}

void MainScene::updateDrawing()
{
	if(sceneData->drawing_mode == "fill")
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if(sceneData->drawing_mode == "line")
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if(sceneData->drawing_mode == "point")
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
}

// **************************************************************
void MainScene::toggleDrawingMode(string mode)
{
	if(mode == "fill" || mode == "line" || mode == "point")
	{
		sceneData->drawing_mode = mode;
	}
}

void MainScene::toggleDrawingMode(int mode)
{
	switch(mode)
	{
	case 0:
		toggleDrawingMode("fill");
		break;
	case 1:
		toggleDrawingMode("line");
		break;
	case 2:
		toggleDrawingMode("point");
		break;
	default:
		break;
	}
}

void MainScene::toggleDrawingShading(string shading)
{
	if(shading == "flat")
	{
		glShadeModel(GL_FLAT);
	}
	else if(shading == "gouraud")
	{
		glShadeModel(GL_SMOOTH);
	}
}

void MainScene::toggleCulling(string face, string order)
{
	// face
	glEnable(GL_CULL_FACE);

	if(face == "none")
	{
		glDisable(GL_CULL_FACE);
	}
	else if(face == "front")
	{
		glCullFace(GL_FRONT);
	}
	else if(face == "back")
	{
		glCullFace(GL_BACK);
	}
	else if(face == "both")
	{
		glCullFace(GL_FRONT_AND_BACK);
	}

	// order
	if(order == "ccw")
	{
		glFrontFace(GL_CCW);
	}
	else if(order == "cw")
	{
		glFrontFace(GL_CW);
	}
}

void MainScene::toggleLighting(bool doublesided, bool local, bool enabled, float* ambient)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, doublesided ? GL_TRUE : GL_FALSE);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, local ? GL_TRUE : GL_FALSE);

	(enabled ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING));

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

void MainScene::toggleLight(int index, int value)
{
	if(numLights > 0)
	{
		if(index < numLights)
		{
			bool enabled = (value % 2 != 0);
			sceneData->getLight(index)->setEnabled(enabled);
		}
	}
}

void MainScene::toggleUseDL(int value)
{
	if(value)
	{
		SceneNode::setDL(true);
	}
	else
	{
		SceneNode::setDL(false);
	}
}

void MainScene::toggleWind(int wind)
{
	this->sceneData->getSceneGraph()->setFlagWind(wind);
}

void MainScene::toggleAnimationsReset()
{
	sceneData->getSceneGraph()->resetAnimations();
}

// **************************************************************
int MainScene::getNumberOfLights()
{
	return numLights;
}

vector<Light* > MainScene::getLights()
{
	return sceneData->getLights();
}

Light* MainScene::getLight(int index)
{
	return sceneData->getLights().at(index);
}