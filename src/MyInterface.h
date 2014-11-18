#ifndef MYINTERFACE_H
#define MYINTERFACE_H

#include "CGFinterface.h"

#include <vector>

using namespace std;

class MyInterface : public CGFinterface {
public:
	virtual void initGUI();
	virtual void processGUI(GLUI_Control *ctrl);

	void createLightsMenu();
	void createDrawingMenu();
	void createFlagsMenu();
	void createDLMenu();
	void createAnimationsMenu();

private:

	GLUI_Panel * panelDrawing;
	GLUI_RadioGroup * radioDrawing;

	GLUI_Panel * panelLights;

	GLUI_Panel * panelFlags;
	GLUI_Spinner * windSpinner1;

	GLUI_Panel * panelDL;
	GLUI_Checkbox * checkDL;

	GLUI_Panel * panelAnimations;
	GLUI_Button * resetAnimations;

	int currentID;

	int drawingMode;
	
	int* lights;
	int lightOffset;
	int lightNum;

	int camNum;
	int cam;

	int wind;

	int useDL;
};

#endif