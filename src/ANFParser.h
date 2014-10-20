#ifndef _ANFPARSER_H_
#define _ANFPARSER_H_

#include "tinyxml.h"

#include "ParserContainer.h"
#include "SceneData.h"
#include "Point.h"
#include "Camera.h"
#include "Appearance.h"
#include "Primitives.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

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
	TiXmlElement* graphElement;

	ParserContainer* parserContainer;

	SceneData* sceneData;

private:
	int errors, warnings;

	// Parser state: 0 for SUCCESS, != 0 for ERRORS
	int state; 

	// Loads .anf file
	int loadFile(const char* filename);

	// Check File type (opening tag: ex. <anf></anf>)
	int checkFileType(const char * fileType);

	// Checks if all elements exists
	int checkAllElements();


	int getElement(TiXmlElement** element, const char * elementName);

	// Load Elements
	int loadGlobals();
	int loadCameras();
	int loadLights();
	int loadTextures();
	int loadAppearances();
	int loadGraph();

	// Read transformations
	int readTransforms(TiXmlElement* nodeElement, ParserNode* currentNode);
	int readRotate(TiXmlElement* transformElement, ParserNode* currentNode);
	int readScale(TiXmlElement* transformElement, ParserNode* currentNode);
	int readTranslate(TiXmlElement* transformElement, ParserNode* currentNode);

	int readAppearance(TiXmlElement* transformElement, ParserNode* currentNode);

	int readPrimitives(TiXmlElement* nodeElement, ParserNode* currentNode);
	int readPrimitiveRectangle(TiXmlElement* primitiveElement, ParserNode* currentNode);
	int readPrimitiveTriangle(TiXmlElement* primitiveElement, ParserNode* currentNode);
	int readPrimitiveCylinder(TiXmlElement* primitiveElement, ParserNode* currentNode);
	int readPrimitiveSphere(TiXmlElement* primitiveElement, ParserNode* currentNode);
	int readPrimitiveTorus(TiXmlElement* primitiveElement, ParserNode* currentNode);

	int readDescendants(TiXmlElement* nodeElement, ParserNode* currentNode);

	// Read cameras
	int readPerspectiveCameras(string strInitialCamera);
	int readOrthoCameras(string strInitialCamera);

	// Read lights
	int readOmniLight(TiXmlElement** lightElement);
	int readSpotLight(TiXmlElement** lightElement);

	// Lights and appearances
	int readComponents(TiXmlElement** light, Point4d** ambient, Point4d** diffuse, Point4d** specular);

	// ===========================================
	// CREATE SCENE GRAPH
	// ===========================================
	int createSceneGraph();
	int addTransforms(ParserNode* parserNode);

	// ===========================================
	// VERIFICATIONS
	// ===========================================
	int verifyGraphCoherence();
	int addNodesToSceneGraph();
	void buildNodesOfSceneGraph();
	int verifyCicles();

	// ===========================================
	// NODES BUILDING
	// ===========================================
	void addDescendantsToNode(ParserNode* parserNode, SceneNode* sceneNode);
	void addAppearancesToNode(ParserNode* parserNode, SceneNode* sceneNode);
	void addPrimitivesToNode(ParserNode* parserNode, SceneNode* sceneNode);
	void addTransformsToNode(ParserNode* parserNode, SceneNode* sceneNode);

	// ===========================================
	// OTHERS
	// ===========================================
	Texture* getTexture(string id);
	int readString(TiXmlElement** camera, string* str, string descr, const int msgTypeInFailure);
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