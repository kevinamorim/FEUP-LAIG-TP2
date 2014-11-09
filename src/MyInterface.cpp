#include "MyInterface.h"
#include "MainScene.h"
#include "CGFapplication.h"

#include <sstream>

using namespace std;

bool between(int value, int min, int max) { return value >= min && value <= max; }

void MyInterface::initGUI()
{
	// init values
	currentID = 0;

	lightNum = ((MainScene *) scene)->getNumberOfLights();
	camNum = ((MainScene *) scene)->getNumberOfCameras();

	// create GUI
	panelDrawing = addPanel("Drawing");
	createDrawingMenu();
	addColumn();

	panelLights = addPanel("Lights");
	createLightsMenu();
	addColumn();

	panelCameras = addPanel("Points of View");
	createCamerasMenu();
	addColumn();

	panelDL = addPanel("Display Lists");
	createDLMenu();
	addColumn();
}

void MyInterface::processGUI(GLUI_Control *ctrl)
{
	int id = ctrl->user_id;

	if(id > -1)
	{
		if(between(id, lightOffset, lightOffset + lightNum - 1))
		{
			int index = id - lightOffset;

			((MainScene *) scene)->toggleLight(index, lights[index]);
		}
		else if(id == radioDrawing->get_id())
		{
			((MainScene *) scene)->toggleDrawingMode(drawingMode);
		}
		else if(id == radioCameras->get_id())
		{
 			((MainScene *) scene)->toggleCamera(cam);
		}
		else if(id == checkDL->get_id())
		{
			((MainScene *) scene)->toggleUseDL(useDL);
		}
	}

	syncVars();
}

void MyInterface::createLightsMenu()
{
	lights = new int[lightNum];

	lightOffset = currentID;

	for(int i = 0; i < lightNum; i++)
	{
		Light* sceneLight = ((MainScene *) scene)->getLight(i);

		lights[i] = sceneLight->Enabled();

		addCheckboxToPanel(panelLights, (char*)sceneLight->getName().c_str(), &(lights[i]), currentID);

		currentID++;
	}
}

void MyInterface::createDrawingMenu() {

	radioDrawing = addRadioGroupToPanel(panelDrawing, &drawingMode, currentID);
	currentID++;

	radioDrawing->set_int_val(0);

	addRadioButtonToGroup(radioDrawing, "Fill");
	addRadioButtonToGroup(radioDrawing, "Wireframe");
	addRadioButtonToGroup(radioDrawing, "Point");
}

void MyInterface::createCamerasMenu()
{
	radioCameras = addRadioGroupToPanel(panelCameras, &cam, currentID);
	currentID++;

	radioCameras->set_int_val(0);

	// add one radio button for each existent camera
	for(int i = 0; i < camNum; i++)
	{
		Camera * sceneCamera = ((MainScene*) scene)->getCamera(i);
		
		addRadioButtonToGroup(radioCameras, (char*) sceneCamera->getID().c_str());
	}
}

void MyInterface::createDLMenu()
{
	useDL = 0;

	checkDL = addCheckboxToPanel(panelDL, "Use", &useDL, currentID);	currentID++;
}

void updateVars()
{

}