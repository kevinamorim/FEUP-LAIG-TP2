#ifndef _ANFPARSER_H_
#define _ANFPARSER_H_

#include "CGFApplication.h"

#include "tinyxml.h"

#include "SceneData.h"
#include "Camera.h"
#include "Primitives.h"
#include "Patch.h"
#include "Transforms.h"

#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

void printMatrix(float* m, unsigned int rows, unsigned int cols);

bool stringIn(string & str, const string colection[]);

string getArrayAsString(const string colection[]);

class ANFParser
{
public:
	ANFParser(char *filename);
	~ANFParser();
	
	// ===========================================
	// GETTERS & SETTERS
	// ===========================================
	int getState();

	SceneData* getSceneData();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent, const char * attr, const char *val);
	
	// Parse the .anf file
	// Returns SUCCESS (0) or ERROR (-1)
	int Parse();

protected:
	// Later we can create more specific codes. Like, particular error codes.
	const static int OK = 0;
	const static int ERROR = -1;
	const static int WARNING = -2;
	const static int INFO = 1;

	const static int OK_WIDTH = 35;
	const static int ERROR_WIDTH = 45;
	const static int WARNING_WIDTH = 45;
	const static int INFO_WIDTH = 35;

	ofstream out;

	const static bool infoActive = false;

	unsigned int currentLight;

	const char* filename;

	TiXmlDocument* doc;

	TiXmlElement* anfElement;

	TiXmlElement* globalsElement;
	TiXmlElement* camerasElement;
	TiXmlElement* lightsElement;
	TiXmlElement* texturesElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* animationsElement;
	TiXmlElement* graphElement;

	SceneData* sceneData;

	vector<pair<string, string> > descendants;

private:
	int errors, warnings;

	// Parser state: 0 for SUCCESS, != 0 for ERRORS
	int state; 

	// Loads .anf file
	int loadFile(const char* filename);

	// Check File type (opening tag: ex. <anf></anf>)
	int checkFileType(const char * fileType);

	// Checks if all main elements exists
	int checkAllElements();
	int checkElement(TiXmlElement** element, string elementName);

	// ======================================================
	//		PARSER
	// ======================================================
	// Globals
	int parseGlobals();
	// Cameras
	int parseCameras();
	int parsePerspectiveCameras(string strInitialCamera);
	int parseOrthoCameras(string strInitialCamera);
	// Lights
	int parseLights();
	int parseOmniLight(TiXmlElement** lightElement);
	int parseSpotLight(TiXmlElement** lightElement);
	int parseComponents(TiXmlElement** element, Point4d** ambient, Point4d** diffuse, Point4d** specular);
	// Textures
	int parseTextures();
	// Appearances
	int parseAppearances();
	// Animations
	int parseAnimations();
	int parseLinearAnimation(TiXmlElement** lightElement, string id, float span);
	int parseCircularAnimation(TiXmlElement** lightElement, string id, float span);

	// ======================================================
	//		GRAPH
	// ======================================================
	int parseGraphNodes();
	int verifyGraph();
	int linkGraphNodes();
	int createGraphDisplayLists();

	// Transforms
	int parseNodeTransforms(TiXmlElement* nodeElement, SceneNode* node);
	int parseRotate(TiXmlElement* transformElement, queue<Transform*> * transforms);
	int parseScale(TiXmlElement* transformElement, queue<Transform*> * transforms);
	int parseTranslate(TiXmlElement* transformElement, queue<Transform*> * transforms);
	int addTransformsToNode(SceneNode* node, queue<Transform* > * transforms);

	// Appearance
	int parseNodeAppearance(TiXmlElement* transformElement, SceneNode* node);

	// Primitives
	int parseNodePrimitives(TiXmlElement* nodeElement, SceneNode* node);
	int parseRectangle(TiXmlElement* primitiveElement, SceneNode* node);
	int parseTriangle(TiXmlElement* primitiveElement, SceneNode* node);
	int parseCylinder(TiXmlElement* primitiveElement, SceneNode* node);
	int parseSphere(TiXmlElement* primitiveElement, SceneNode* node);
	int parseTorus(TiXmlElement* primitiveElement, SceneNode* node);

	//TP2
	int parsePlane(TiXmlElement* primitive, SceneNode* node);
	int parsePatch(TiXmlElement* primitive, SceneNode* node);

	//Animations
	int parserNodeAnimation(TiXmlElement* nodeElement, SceneNode* node);

	// Descendants
	int parseNodeDescendants(TiXmlElement* nodeElement, SceneNode* node);

	// ===========================================
	// OTHERS
	// ===========================================
	int readString(TiXmlElement** camera, string* str, string descr, const int msgTypeInFailure);
	int readInt(TiXmlElement** element, int* value, string descr, const int type);
	int readFloat(TiXmlElement** element, float* value, string descr, const int msgTypeInFailure);
	int readAxis(TiXmlElement** element, char* axis, string descr, const int msgTypeInFailure);
	int readBool(TiXmlElement** element, bool* value, string descr, const int msgTypeInFailure);
	int readPoint2d(TiXmlElement** element, Point2d** point, string descr, const int msgTypeInFailure);
	int readPoint3d(TiXmlElement** element, Point3d** point, string descr, const int msgTypeInFailure);
	int readPoint4d(TiXmlElement** element, Point4d** point, string descr, const int msgTypeInFailure);

	void printMsg(const int type);
	void printMsg(const int type, string descr);
};

#endif