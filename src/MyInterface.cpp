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

	// create GUI
	panelDrawing = addPanel("Drawing");
	createDrawingMenu();
	addColumn();

	panelLights = addPanel("Lights");
	createLightsMenu();
	addColumn();

	panelFlags = addPanel("Flags");
	createFlagsMenu();
	addColumn();

	panelDL = addPanel("Display Lists");
	createDLMenu();
	addColumn();

	panelAnimations = addPanel("Animations");
	createAnimationsMenu();
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
		else if(id == checkDL->get_id())
		{
			((MainScene *) scene)->toggleUseDL(useDL);
		}
		else if(id == windSpinner1->get_id())
		{
			((MainScene *) scene)->toggleWind(wind);
		}
		else if(id == resetAnimations->get_id())
		{
			((MainScene *) scene)->toggleAnimationsReset();
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

void MyInterface::createFlagsMenu() {

	windSpinner1 = addSpinnerToPanel(panelFlags, "Wind", GLUI_SPINNER_INT, &wind, currentID);
	currentID++;

	windSpinner1->set_int_limits(0, 10, GLUI_LIMIT_WRAP);
	//windSpinner1->set_speed(1.0);
}

void MyInterface::createDLMenu()
{
	useDL = 0;

	checkDL = addCheckboxToPanel(panelDL, "Use", &useDL, currentID);
	currentID++;
}

void MyInterface::createAnimationsMenu()
{
	resetAnimations = addButtonToPanel(panelAnimations, "Reset", currentID);
	currentID++;
}