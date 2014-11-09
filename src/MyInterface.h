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
	void createCamerasMenu();
	void createDLMenu();

private:

	GLUI_Panel * panelDrawing;
	GLUI_RadioGroup * radioDrawing;

	GLUI_Panel * panelLights;

	GLUI_Panel * panelCameras;
	GLUI_RadioGroup * radioCameras;

	GLUI_Panel * panelDL;
	GLUI_Checkbox * checkDL;

	int currentID;

	int drawingMode;
	
	int* lights;
	int lightOffset;
	int lightNum;

	int camNum;
	int cam;

	int useDL;
};

#endif