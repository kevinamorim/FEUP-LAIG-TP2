#include "ANFParser.h"

const int X = 0;
const int Y = 1;
const int Z = 2;

const string DRAWING_MODE[] = {"fill", "line", "point"};
const string DRAWING_SHADING[] = {"flat", "gouraud"};

const string CULLING_FACE[] = {"none", "back", "front", "both"};
const string CULLING_ORDER[] = {"ccw", "cw"};

unsigned int lightsID[] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

// ***********************************************************************************
// ********************************  PARSER  *****************************************
ANFParser::ANFParser(char* filename) : filename(filename)
{
	currentLight = 0;
	this->descendants = vector<pair<string, string> >();
}

// ***********************************************************************************
// *****************************  PARSE METHOD  **************************************
int ANFParser::Parse()
{
	out.open("debug.log");

	errors = 0;
	warnings = 0;

	state = OK;

	out << "DEBUG FOR .anf FILE - LAIG - FEUP 2014/15" << endl;

	auto tim = time(nullptr);
	struct tm timeinfo;
	localtime_s(&timeinfo, &tim);
	out << std::put_time(&timeinfo, "%d-%m-%Y %H-%M-%S") << endl;

	// ***************************************************************
	// ******************* CRITICAL SECTION **************************
	// ***************************************************************
	out << "___________________________________" << endl;
	if(loadFile(filename) != OK)
	{
		state = ERROR;
		return ERROR;
	}

	out << "___________________________________" << endl;
	if(checkFileType("anf") != OK)
	{
		state = ERROR;
		return ERROR;
	}

	out << "___________________________________" << endl;
	if(checkAllElements() != OK)
	{
		state = ERROR;
		return ERROR;
	}

	out << "___________________________________" << endl;
	// ***************************************************************
	// ***************************************************************

	sceneData = new SceneData();

	out << "___________________________________" << endl;
	out << "> Load : globals" << endl;
	if(globalsElement)
	{
		parseGlobals();
	}
	else
	{
		printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	}

	out << "___________________________________" << endl;
	out << "> Load : cameras" << endl;
	if(camerasElement)
	{
		parseCameras();
	}
	else
	{
		printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	}

	out << "___________________________________" << endl;
	out << "> Load : lights" << endl;
	if(lightsElement)
	{
		parseLights();
	}
	else
	{
		printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	}

	out << "___________________________________" << endl;
	out << "> Load : textures" << endl;
	if(texturesElement)
	{
		parseTextures();
	}
	else
	{
		printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	}

	out << "___________________________________" << endl;
	out << "> Load : appearances" << endl;
	if(texturesElement)
	{
		parseAppearances();
	}
	else
	{
		printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	}

	//out << "___________________________________" << endl;
	//out << "> Load : animations" << endl;
	//if(animationsElement)
	//{
	//	parseAnimations();
	//}
	//else
	//{
	//	printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	//}

	out << "___________________________________" << endl;
	out << "> Create : graph" << endl;
	if(graphElement)
	{
		out << "    _______________________________" << endl;
		out << "    > Create nodes..." << endl;
		if(parseGraphNodes() != OK)
			errors++;

		out << "    _______________________________" << endl;
		out << "    > Link nodes..." << endl;
		if(linkGraphNodes() != OK)
			errors++;

		out << "    _______________________________" << endl;
		out << "    > Verifying graph..." << endl;
		if(verifyGraph() != OK)
			errors++;

		out << "    _______________________________" << endl;
		out << "    > Creating graph display lists..." << endl;
		if(createGraphDisplayLists() != OK)
			errors++;
	}
	else
	{
		printMsg(WARNING, "The block is NULL (an error should exist concerning this block)");
	}

	out << "___________________________________" << endl;
	out << "___________________________________" << endl;
	out << "> Finished parsing:" << endl;
	out << std::internal << setw(35) << "[ERRORS] : "; out << errors << endl;
	out << std::internal << setw(35) << "[WARNINGS] : "; out << warnings << endl;

	out.close();

	// ***************************************************************
	// ***************************************************************

	if(errors)
	{
		cout << endl << endl;
		cout << "> Found errors : please check 'debug.log'";
		cout << endl << endl;

		state = ERROR;
		return ERROR;
	}

	return OK;
}

// ***********************************************************************************
// *********************************  FILE  ******************************************
int ANFParser::loadFile(const char* filename)
{
	doc = new TiXmlDocument(filename);

	out << "> Loading file... " << endl;
	if(!doc->LoadFile())
	{
		string msg = doc->ErrorDesc();
		printMsg(ERROR, msg);
		errors++;
		return ERROR;
	}
	printMsg(OK);

	return OK;
}

int ANFParser::checkFileType(const char* fileType)
{
	int localErrors = 0;

	anfElement = doc->FirstChildElement(fileType);

	out << "> Checking file type..." << endl;
	if (!anfElement)
	{
		printMsg(ERROR, "File does not respect .anf sintax");
		localErrors++;
	}

	if(!localErrors)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

// ***********************************************************************************
// *******************************  ELEMENTS  ****************************************
int ANFParser::checkAllElements()
{
	int localErrors = 0;

	if(checkElement(&globalsElement, "globals") != OK)
		localErrors++;

	if(checkElement(&camerasElement, "cameras") != OK)
		localErrors++;

	if(checkElement(&lightsElement, "lights") != OK)
		localErrors++;

	if(checkElement(&texturesElement, "textures") != OK)
		localErrors++;

	if(checkElement(&appearancesElement, "appearances") != OK)
		localErrors++;

	//if(checkElement(&animationsElement, "animations") != OK)
	//	localErrors++;

	if(checkElement(&graphElement, "graph") != OK)
		localErrors++;

	if(!localErrors)
	{
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::checkElement(TiXmlElement** element, string elementName)
{
	int localErrors = 0;

	*element = anfElement->FirstChildElement((const char*)elementName.c_str());

	out << "> Block : " << elementName << endl; 
	if (!(*element))
	{
		printMsg(ERROR, "Block does not exist");
		localErrors++;
	}

	if(!localErrors)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

// ===================================================================================
// =====================================  PARSE  =====================================
// ===================================================================================
int ANFParser::parseGlobals()
{
	int localErrors = 0;

	out << "    _______________________________" << endl;
	out << "    > Reading : drawing..." << endl;

	TiXmlElement* element = globalsElement->FirstChildElement("drawing");

	// --------------
	//    Drawing
	// --------------
	if(!element)
	{
		printMsg(ERROR, "Block <drawing> is not defined");
		localErrors++;
	}
	else
	{
		string strMode, strShading;
		Point4d* background;

		if(readString(&element, &strMode, "mode", ERROR) != OK)
		{
			localErrors++;
		}
		else if(!stringIn(strMode, DRAWING_MODE))
		{
			printMsg(ERROR, "Attribute 'mode' has illegal arguments - must be in {fill, line, point}");
			localErrors++;
		}

		if(readString(&element, &strShading, "shading", ERROR) != OK)
		{
			localErrors++;
		}
		else if(!stringIn(strShading, DRAWING_SHADING))
		{
			printMsg(ERROR, "Attribute 'shading' has illegal arguments - must be in {flat, gouraud}");
			localErrors++;
		}

		if(readPoint4d(&element, &background, "background", ERROR) != OK)
		{
			localErrors++;
		}

		if(!localErrors)
		{
			sceneData->setDrawing(strMode, strShading, background);
		}
	}

	if(!localErrors)
	{
		printMsg(OK);
	}
	else
	{
		errors += localErrors;
		localErrors = 0;
	}

	// --------------
	//    Culling
	// --------------
	out << "    _______________________________" << endl;
	out << "    > Reading : culling..." << endl;

	element = element->NextSiblingElement("culling");

	if(!element)
	{
		printMsg(ERROR, " : Block <culling> is not defined");
		localErrors++;
	}
	else
	{
		string strFace, strOrder;

		if(readString(&element, &strFace, "face", ERROR) != OK)
		{
			localErrors++;
		}
		else if(!stringIn(strFace, CULLING_FACE))
		{
			printMsg(ERROR, " : Attribute 'face' has illegal arguments - must be in {front, back, none, both}");
			localErrors++;
		}

		if(readString(&element, &strOrder, "order", ERROR) != OK)
		{
			localErrors++;
		}
		else if(!stringIn(strOrder, CULLING_ORDER))
		{
			printMsg(ERROR, " : Attribute 'order' has illegal arguments must be in {ccw, cw}");
			localErrors++;
		}

		if(!localErrors)
		{
			sceneData->setCulling(strFace, strOrder);
		}
	}

	if(!localErrors)
	{
		printMsg(OK);
	}
	else
	{
		errors += localErrors;
		localErrors = 0;
	}

	// --------------
	//    Lighting
	// --------------
	element = element->NextSiblingElement("lighting");

	out << "    _______________________________" << endl;
	out << "    > Reading : lighting..." << endl;
	if(!element)
	{
		printMsg(ERROR, "Block <lighting> is not defined");
		localErrors++;
	}
	else
	{
		bool doubleSided, local, enabled;
		Point4d* ambient;

		if(readBool(&element, &doubleSided,"doublesided", ERROR) != OK)
		{
			localErrors++;
		}

		if(readBool(&element, &local,"local", ERROR) != OK)
		{
			localErrors++;
		}

		if(readBool(&element, &enabled,"enabled", ERROR) != OK)
		{
			localErrors++;
		}

		if(readPoint4d(&element, &ambient,"ambient", ERROR) != OK)
		{
			localErrors++;
		}

		if(!localErrors)
		{
			sceneData->setLighting(doubleSided, local, enabled, ambient);
		}
	}

	if(!localErrors)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseCameras()
{
	out << "    _______________________________" << endl;
	out << "    > Reading : cameras..." << endl;

	int localErrors = 0;
	int localWarnings = 0;
	string strInitial;

	if(readString(&camerasElement, &strInitial, "initial", ERROR) != OK)
	{
		localErrors++;
	}
	else
	{
		parsePerspectiveCameras(strInitial);

		parseOrthoCameras(strInitial);
	}

	if(!sceneData->hasInitialCamera())
	{
		out << "    _______________________________" << endl;
		printMsg(ERROR, "No initial camera was set (verifiy that the attribute 'initial' refers to a well defined camera)");
		localErrors++;
	}

	if(!sceneData->hasCameras())
	{
		out << "    _______________________________" << endl;
		printMsg(ERROR, "No cameras were created (verifiy if the camera(s) is(are) well defined)");
		localErrors++;
	}

	if(!localErrors)
	{
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parsePerspectiveCameras(string strInitialCamera)
{
	int localErrors = 0;
	int localWarnings = 0;

	TiXmlElement* camera = camerasElement->FirstChildElement("perspective");
	if(!camera)
	{
		printMsg(WARNING, "A perspective camera is not defined (not mandatory ONLY when an orthographic camera is defined)");
		warnings++;
	}
	else
	{
		int cameraErrors = 0;

		while(camera)
		{
			out << "        ___________________________" << endl;
			string strID, strPosition, strTarget;
			float near, far, angle;
			Point3d *position, *target;

			if(readString(&camera, &strID, "id", ERROR) != OK)
			{
				cameraErrors++;
			}

			out << "        > Perspective : " << strID << endl;
			if(readFloat(&camera, &near, "near", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &far, "far", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &angle, "angle", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readPoint3d(&camera, &position, "pos", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readPoint3d(&camera, &target, "target", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(!cameraErrors)
			{
				PerspCamera* cam = new PerspCamera(strID, near, far, angle, position, target);

				if(strID == strInitialCamera)
				{
					sceneData->addCameraInitial(cam);
				}
				else
				{
					sceneData->addCamera(cam);
				}

				printMsg(OK);
			}
			else
			{
				localErrors += cameraErrors;
				cameraErrors = 0;
			}

			camera = camera->NextSiblingElement("perspective");
		}
	}

	warnings += localWarnings;

	if(!localErrors)
	{
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseOrthoCameras(string strInitialCamera) 
{
	int localErrors = 0;
	int localWarnings = 0;

	TiXmlElement* camera = camerasElement->FirstChildElement("ortho");
	if(!camera)
	{
		printMsg(WARNING, "An orthographic camera is not defined (not mandatory ONLY when a perspective camera is defined)");
		warnings++;
	}
	else
	{
		int cameraErrors = 0;

		while(camera)
		{
			out << "        ___________________________" << endl;
			string strID;
			float near, far, left, right, top, bottom;
			char direction; // axis

			if(readString(&camera, &strID, "id", ERROR) != OK)
			{
				cameraErrors++;
			}

			out << "        > Ortho : " << strID << endl;

			if(readFloat(&camera, &near, "near", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &far, "far", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readAxis(&camera, &direction, "direction", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &left, "left", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &right, "right", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &top, "top", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(readFloat(&camera, &bottom, "bottom", ERROR) != OK)
			{
				cameraErrors++;
			}

			if(!cameraErrors)
			{
				OrthoCamera* cam = new OrthoCamera(strID, direction, near, far, left, right, top, bottom);

				if(strID == strInitialCamera)
				{
					sceneData->addCameraInitial(cam);
				}
				else
				{
					sceneData->addCamera(cam);
				}
			}
			else
			{
				localErrors += cameraErrors;
				cameraErrors = 0;
			}

			camera = camera->NextSiblingElement("ortho");
		}
	}

	warnings += localWarnings;

	if(!localErrors)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseLights() 
{
	int localErrors = 0;

	out << "    _______________________________" << endl;
	out << "    > Reading : lights..." << endl;

	TiXmlElement* lightElement = lightsElement->FirstChildElement("light");

	if(!lightElement) 
	{
		printMsg(ERROR, "No lights are defined (must be defined at least one)");
		localErrors++;
	}
	else
	{
		while(lightElement)
		{
			string strType;

			if(readString(&lightElement, &strType, "type", ERROR) != OK)
			{
				localErrors++;
			}
			else
			{
				if(strType == "omni")
				{
					parseOmniLight(&lightElement);
				}
				else if(strType == "spot")
				{
					parseSpotLight(&lightElement);
				}
				else
				{
					printMsg(ERROR, "Attribute 'type' is of illegal type (must be in {omni, spot})");
					localErrors++;
				}
			}

			lightElement = lightElement->NextSiblingElement("light");
		}
	}

	if(!sceneData->hasLights())
	{
		out << "    _______________________________" << endl;
		printMsg(ERROR, "No lights were created (verifiy if the light(s) is(are) well defined)");
		localErrors++;
	}

	if(!localErrors)
	{
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseOmniLight(TiXmlElement** lightElement)
{
	out << "        ___________________________" << endl;

	int localErrors = 0;
	string strID;
	bool enabled, marker;
	Point3d *position;
	Point4d *ambient, *diffuse, *specular;

	TiXmlElement* light = *lightElement;

	// id (string)
	if(readString(&light, &strID, "id", ERROR) != OK)
	{
		localErrors++;
	}

	out << "        > Omni : " << strID << endl;

	// enabled (boolean)
	if(readBool(&light, &enabled, "enabled", ERROR) != OK)
	{
		localErrors++;
	}

	// marker (boolean)
	if(readBool(&light, &marker, "marker", ERROR) != OK)
	{
		localErrors++;
	}

	// position (Point3d)
	if(readPoint3d(&light, &position, "pos", ERROR) != OK)
	{
		localErrors++;
	}

	// ambient (Point4d)
	// diffuse (Point4d)
	// specular (Point4d)
	if(parseComponents(&light, &ambient, &diffuse, &specular) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		OmniLight* lightObj = new OmniLight(lightsID[currentLight], strID, enabled, marker, position, ambient, diffuse, specular);
		currentLight++;

		sceneData->addLight(lightObj);

		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseSpotLight(TiXmlElement** lightElement)
{
	out << "        ___________________________" << endl;

	int localErrors = 0;
	string strID;
	bool enabled, marker;
	Point3d *position, *target;
	Point4d *ambient, *diffuse, *specular;
	float angle, exponent;

	TiXmlElement* light = *lightElement;	

	// id (string)
	if(readString(&light, &strID, "id", ERROR) != OK)
	{
		localErrors++;
	}

	out << "        > Spot : " << strID << endl;

	// enabled (boolean)
	if(readBool(&light, &enabled, "enabled", ERROR) != OK)
	{
		localErrors++;
	}

	// marker (boolean)
	if(readBool(&light, &marker, "marker", ERROR) != OK)
	{
		localErrors++;
	}

	// position (Point3d)
	if(readPoint3d(&light, &position, "pos", ERROR) != OK)
	{
		localErrors++;
	}

	// target (Point3d)
	if(readPoint3d(&light, &target, "target", ERROR) != OK)
	{
		localErrors++;
	}

	// angle (float)
	if(readFloat(&light, &angle, "angle", ERROR) != OK)
	{
		localErrors++;
	}

	// exponent (float)
	if(readFloat(&light, &exponent, "exponent", ERROR) != OK)
	{
		localErrors++;
	}

	// ambient (Point4d)
	// diffuse (Point4d)
	// specular (Point4d)
	if(parseComponents(&light, &ambient, &diffuse, &specular) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		SpotLight* lightObj = new SpotLight(lightsID[currentLight], strID, enabled, marker, position, target, angle, exponent, ambient, diffuse, specular);
		currentLight++;

		sceneData->addLight(lightObj);

		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseTextures()
{
	int localErrors = 0;
	int localWarnings = 0;

	out << "    _______________________________" << endl;
	out << "    > Reading : textures..." << endl;

	TiXmlElement* texture = texturesElement->FirstChildElement("texture");

	if(!texture)
	{
		printMsg(WARNING, "There are no textures (though the block <textures> is defined)");
		localWarnings++;
	}
	else
	{
		int textureErrors = 0;

		while(texture)
		{
			string strID, strFile;
			float length_s, length_t;

			if(readString(&texture, &strID, "id", ERROR) != OK)
			{
				textureErrors++;
			}

			if(readString(&texture, &strFile, "file", ERROR) != OK)
			{
				textureErrors++;
			}

			if(readFloat(&texture, &length_s, "texlength_s", ERROR) != OK)
			{
				textureErrors++;
			}

			if(readFloat(&texture, &length_t, "texlength_t", ERROR) != OK)
			{
				textureErrors++;
			}

			if(!textureErrors)
			{
				out << "        ___________________________" << endl;
				out << "        > Tex : " << strID << " , " << strFile << " , "  << length_s << " , " << length_t << endl;

				sceneData->addTexture(new Texture(strID, strFile, length_s, length_t));
			}
			else
			{
				localErrors += textureErrors;
				textureErrors = 0;
			}

			texture = texture->NextSiblingElement("texture");
		}
	}

	if(!localErrors && !localWarnings)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		warnings += localWarnings;
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseAppearances()
{
	int localErrors = 0;
	int localWarnings = 0;

	out << "    _______________________________" << endl;
	out << "    > Reading : appearances..." << endl;

	TiXmlElement* appearance = appearancesElement->FirstChildElement("appearance");

	if(!appearance)
	{
		printMsg(WARNING, "There are no appearances (though the block <appearance> is defined)");
		localWarnings++;
	}
	else
	{
		int appearanceErrors = 0;

		while(appearance)
		{	
			string strID, strTexture;
			float shininess;
			Point4d *ambient, *diffuse, *specular;

			out << "        ___________________________" << endl;

			if(readString(&appearance, &strID, "id", ERROR) != OK)
			{
				appearanceErrors++;
			}
			else
			{
				out << "        > Reading : " << strID << endl;
			}

			if(readFloat(&appearance, &shininess, "shininess", ERROR) != OK)
			{
				appearanceErrors++;
			}

			if(readString(&appearance, &strTexture, "textureref", WARNING) != OK)
			{
				localWarnings++; // Not critical
			}

			if(parseComponents(&appearance, &ambient, &diffuse, &specular) != OK)
			{
				appearanceErrors++;
			}

			if(!appearanceErrors)
			{
				if(!(strTexture.empty()))
				{
					Texture* tex = sceneData->getTexture(strTexture);

					if(!tex)
					{
						string msg = "The referenced texture '";
						msg += strTexture;
						msg += "' does not exist";

						printMsg(ERROR,msg);
						localErrors++;
					}
					else
					{
						Appearance* app = new Appearance(strID, shininess, ambient, diffuse, specular, tex);

						sceneData->addAppearance(app);

						printMsg(OK);
					}
				}
				else
				{
					Appearance* app = new Appearance(strID, shininess, ambient, diffuse, specular);

					sceneData->addAppearance(app);

					printMsg(OK);
				}
			}
			else
			{
				localErrors += appearanceErrors;
				appearanceErrors = 0;
			}

			appearance = appearance->NextSiblingElement("appearance");
		}
	}

	if(!localErrors && !localWarnings)
	{
		return OK;
	}
	else
	{
		warnings += localWarnings;
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseAnimations()
{
	int localErrors = 0;
	int localWarnings = 0;

	out << "    _______________________________" << endl;
	out << "    > Reading : animations..." << endl;

	TiXmlElement* animation = animationsElement->FirstChildElement("animation");

	if(!animation)
	{
		printMsg(WARNING, "There are no animations");
		localWarnings++;
	}
	else
	{
		int animationErrors = 0;

		while(animation)
		{	
			string strID, strType;
			float span;

			out << "        ___________________________" << endl;

			if(readString(&animation, &strID, "id", ERROR) != OK)
			{
				animationErrors++;
			}
			else
			{
				out << "        > Reading : " << strID << endl;
			}

			if(readFloat(&animation, &span, "span", ERROR) != OK)
			{
				animationErrors++;
			}

			if(readString(&animation, &strType, "type", WARNING) != OK)
			{
				animationErrors++;
			}
			else
			{
				if(strType == "linear")
				{
					parseLinearAnimation(&animation);
				}
				else if(strType == "circular")
				{
					parseCircularAnimation(&animation);
				}
				else
				{
					printMsg(ERROR, "Attribute 'type' is of illegal type (must be in {linear, circular})");
					animationErrors++;
				}
			}

			if(animationErrors)
			{
				localErrors += animationErrors;
				animationErrors = 0;
			}

			animation = animation->NextSiblingElement("animation");
		}
	}

	if(!localErrors && !localWarnings)
	{
		return OK;
	}
	else
	{
		warnings += localWarnings;
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseLinearAnimation(TiXmlElement** animation)
{
	return OK;
}

int ANFParser::parseCircularAnimation(TiXmlElement** animation)
{
	return OK;
}

int ANFParser::parseComponents(TiXmlElement** element, Point4d** ambient, Point4d** diffuse, Point4d** specular)
{
	int localErrors = 0;

	TiXmlElement* component = (*element)->FirstChildElement("component");
	if(!component)
	{
		printMsg(ERROR, "No <component> block is not defined");
		localErrors++;
	}
	else
	{
		string strType;
		bool readAmbient = false;
		bool readDiffuse = false;
		bool readSpecular = false;

		int index = 1;

		while(component)
		{
			if(index > 3)
			{
				printMsg(WARNING, " : More than 3 <component> blocks have been found (overwriting previous values)");
				warnings++;
			}

			if(readString(&component, &strType, "type", ERROR) != OK)
			{
				localErrors++;
			}
			else
			{		
				if(strType == "ambient")
				{
					if(readPoint4d(&component, ambient, "value", ERROR) != OK)
					{
						localErrors++;
					}
					else
					{
						readAmbient = true;
					}
				}
				else if(strType == "diffuse")
				{
					if(readPoint4d(&component, diffuse, "value", ERROR) != OK)
					{
						localErrors++;
					}
					else
					{
						readDiffuse = true;
					}
				}
				else if(strType == "specular")
				{
					if(readPoint4d(&component, specular, "value", ERROR) != OK)
					{
						localErrors++;
					}
					else
					{
						readSpecular = true;
					}
				}
			}

			index++;
			component = component->NextSiblingElement("component");
		}
	}

	if(!localErrors)
	{
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}
// ===================================================================================
// ===================================================================================

// ***********************************************************************************
// **********************************  GRAPH  ****************************************
/* Main method */
int ANFParser::parseGraphNodes()
{
	int localErrors = 0;

	out << "        ___________________________" << endl;
	out << "        > Reading : root..." << endl;
	
	string strRootID;

	if(readString(&graphElement, &strRootID, "rootid", ERROR) != OK)
	{
		localErrors++;
	}
	else
	{
		out << "        > id : \"" << strRootID << "\""<< endl;
		printMsg(OK);
	}

	// ********************************************************
	// Read all the nodes that compose the graph tree.
	TiXmlElement* nodeElement = graphElement->FirstChildElement("node");

	while (nodeElement)
	{
		int nodeErrors = 0;

		out << "        ___________________________" << endl;
		out << "        > Creating : ";

		string strID;

		if(readString(&nodeElement, &strID, "id", ERROR) != OK)
		{
			localErrors++;
		}
		else
		{
			out << "\"" << strID << "\"" << endl;
			printMsg(OK);

			bool usesDL;
			if(readBool(&nodeElement, &usesDL, "displaylist", ERROR) != OK)
			{
				usesDL = false;
			}

			// Create the parserGraph node
			SceneNode* node = new SceneNode(strID, usesDL);
			
			// Add the node to the graph
			sceneData->getSceneGraph()->addNode(node);

			out << "            > Transforms..." << endl;
			if(parseNodeTransforms(nodeElement, node) != OK)
				nodeErrors++;

			out << "            > Appearances..." << endl;
			if(parseNodeAppearance(nodeElement, node) != OK)
				nodeErrors++;

			out << "            > Primitives..." << endl;
			if(parseNodePrimitives(nodeElement, node) != OK)
				nodeErrors++;

			out << "            > Descendants..." << endl;
			if(parseNodeDescendants(nodeElement, node) != OK)
				nodeErrors++;

			if(!nodeErrors)
			{
				if(node->getID() == strRootID)
				{
					sceneData->getSceneGraph()->setRoot(node);
				}
			}
			else
			{
				localErrors += nodeErrors;
				nodeErrors = 0;
			}
		}

		nodeElement = nodeElement->NextSiblingElement("node");
	}

	if(!(sceneData->getSceneGraph()->hasRoot()))
	{
		localErrors++;
	}

	if(!localErrors)
	{
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

/* Transforms */
int ANFParser::parseNodeTransforms(TiXmlElement* nodeElement, SceneNode* node)
{
	int localErrors = 0;
	int localWarnings = 0;

	TiXmlElement* transformsElement = nodeElement->FirstChildElement("transforms");

	queue<Transform*> * transforms = new queue<Transform*>();

	if (transformsElement)
	{
		TiXmlElement* transformElement = transformsElement->FirstChildElement("transform");

		string strType;

		while (transformElement)
		{
			if(readString(&transformElement, &strType, "type", ERROR) != OK)
			{
				localErrors++;
			}
			else
			{
				if ("scale" == strType)
				{
					if(parseScale(transformElement, transforms) != OK)
					{
						printMsg(ERROR, "Scale attributes are not valid");
						localErrors++;
					}
				}
				else if ("rotate" == strType)
				{
					if(parseRotate(transformElement, transforms) != OK)
					{
						printMsg(ERROR, "Rotation attributes are not valid");
						localErrors++;
					}
				}
				else if ("translate" == strType)
				{
					if(parseTranslate(transformElement, transforms) != OK)
					{
						printMsg(ERROR, "Translate attributes are not valid");
						localErrors++;
					}
				}
				else
				{
					printMsg(ERROR, "Invalid transform type - must be in {translate, scale, rotate}");
					localErrors++;
				}
			}

			transformElement = transformElement->NextSiblingElement("transform");
		}
	}
	else
	{
		printMsg(WARNING, " : No transforms, though the block <transforms> exists");
		localWarnings++;
	}

	warnings += localWarnings;

	if(!localErrors)
	{
		addTransformsToNode(node, transforms);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseRotate(TiXmlElement* transformElement, queue<Transform*> * transforms)
{
	out << "            > Rotate" << endl;

	int localErrors = 0;

	char axis;
	float angle;

	if(readAxis(&transformElement, &axis, "axis", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&transformElement, &angle, "angle", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		transforms->push(new Rotate(axis, angle));

		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseTranslate(TiXmlElement* transformElement, queue<Transform*> * transforms)
{
	int localErrors = 0;

	out << "            > Translate" << endl;

	Point3d* to;

	if(readPoint3d(&transformElement, &to, "to", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		transforms->push(new Translate(to));

		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseScale(TiXmlElement* transformElement, queue<Transform*> * transforms)
{
	int localErrors = 0;

	out << "            > Scale" << endl;

	Point3d* factor;

	if(readPoint3d(&transformElement, &factor, "factor", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		transforms->push(new Scale(factor));

		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::addTransformsToNode(SceneNode* node, queue<Transform* > * transforms)
{
	if(transforms->empty()) return OK;

	glPushMatrix();
	glLoadIdentity();

	while(!transforms->empty())
	{
		transforms->front()->apply();
		transforms->pop();
	}

	sceneData->getSceneGraph()->getNode(node->getID())->setTransformMatrix();

	glPopMatrix();

	return OK;
}

/* Appearances */
int ANFParser::parseNodeAppearance(TiXmlElement* nodeElement, SceneNode* node)
{
	int localErrors = 0;
	int localWarnings = 0;

	TiXmlElement* appearanceElement;

	appearanceElement = nodeElement->FirstChildElement("appearanceref");

	// Verifies if attribute exists and has been read
	if(!appearanceElement)
	{	
		printMsg(ERROR, "The block <appearanceref> does not exist");
		localErrors++;
	}
	else
	{
		string strAppearance;

		if(readString(&appearanceElement, &strAppearance, "id", ERROR) != OK)
		{
			localErrors++;
		}
		else
		{
			out << "            > Appearance : " << strAppearance << endl;

			if(strAppearance != "inherit")
			{
				node->setAppearance(sceneData->getAppearance(strAppearance));
			}
		}
	}

	if(!localErrors && !localWarnings)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		warnings += localWarnings;
		errors += localErrors;
		return ERROR;
	}

}

/* Primitives */
int ANFParser::parseNodePrimitives(TiXmlElement* nodeElement, SceneNode* node)
{
	int localErrors = 0;
	int localWarnings = 0;

	TiXmlElement* primitivesElement = nodeElement->FirstChildElement("primitives");

	if(primitivesElement)
	{
		TiXmlElement* primitive = primitivesElement->FirstChildElement();

		if(primitive)
		{
			while (primitive)
			{
				int primErrors = 0;

				string strType = primitive->Value();

				if (strType == "rectangle")
				{
					if(parseRectangle(primitive, node) != OK)
					{
						primErrors++;
					}
				}
				else if (strType == "triangle")
				{
					if(parseTriangle(primitive, node) != OK)
					{
						primErrors++;
					}
				}
				else if (strType == "cylinder")
				{
					if(parseCylinder(primitive, node) != OK)
					{
						primErrors++;
					}
				}
				else if (strType == "sphere")
				{
					if(parseSphere(primitive, node) != OK)
					{
						primErrors++;
					}
				}
				else if (strType == "torus")
				{
					if(parseTorus(primitive, node) != OK)
					{
						primErrors++;
					}
				}
				//TP2
				else if (strType == "plane")
				{
					if(parsePlane(primitive, node) != OK)
					{
						primErrors++;
					}
				}
				else
				{
					printMsg(ERROR, "Invalid primitive - must be in {triangle, rectangle, cylinder, torus, sphere}");
					localErrors++;
				}

				primitive = primitive->NextSiblingElement();
			}
		}
		else
		{
			printMsg(INFO, "The block <primitives> exists, but it is empty (or the content is not valid)");
		}
	}
	else
	{
		printMsg(INFO, "The block <primitives> does not exist");
	}

	warnings +=  localWarnings;

	if(!localErrors)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseRectangle(TiXmlElement* primitive, SceneNode* node)
{
	out << "            > Rectangle" << endl;

	int localErrors = 0;

	Point2d *topLeft, *bottomRight;

	if(readPoint2d(&primitive, &topLeft, "xy1", ERROR) != OK)
	{
		localErrors++;
	}

	if(readPoint2d(&primitive, &bottomRight, "xy2", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		node->addPrimitive(new Rectangle(topLeft, bottomRight));	
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseTriangle(TiXmlElement* primitive, SceneNode* node)
{
	out << "            > Triangle" << endl;

	int localErrors = 0;

	Point3d *point1, *point2, *point3;

	if(readPoint3d(&primitive, &point1, "xyz1", ERROR) != OK)
	{
		localErrors++;
	}

	if(readPoint3d(&primitive, &point2, "xyz2", ERROR) != OK)
	{
		localErrors++;
	}

	if(readPoint3d(&primitive, &point3, "xyz3", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		node->addPrimitive(new Triangle(point1, point2, point3));

		//out << " " << point1->toString() << " " << point2->toString() << " " << point3->toString() << endl;
		printMsg(OK);
		return OK;
	}
	else
	{
		printMsg(ERROR);
		out << " : one of the points is not well defined" << endl;
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseCylinder(TiXmlElement* primitive, SceneNode* node)
{
	out << "            > Cylinder" << endl;

	int localErrors = 0;

	float base, top, height, slices, stacks;

	if(readFloat(&primitive, &base, "base", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &top, "top", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &height, "height", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &slices, "slices", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &stacks, "stacks", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		node->addPrimitive(new Cylinder(base, top, height, slices, stacks));
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseSphere(TiXmlElement* primitive, SceneNode* node)
{
	out << "            > Sphere" << endl;

	int localErrors = 0;

	float radius, slices, stacks;

	if(readFloat(&primitive, &radius, "radius", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &slices, "slices", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &stacks, "stacks", ERROR) != OK)
	{
		localErrors++;
	}


	if(!localErrors)
	{
		node->addPrimitive(new Sphere(radius, slices, stacks));
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

int ANFParser::parseTorus(TiXmlElement* primitive, SceneNode* node)
{
	out << "            > Torus" << endl;

	int localErrors = 0;

	float inner, outer, slices, loops;

	if(readFloat(&primitive, &inner, "inner", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &outer, "outer", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &slices, "slices", ERROR) != OK)
	{
		localErrors++;
	}

	if(readFloat(&primitive, &loops, "loops", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		node->addPrimitive(new Torus(inner, outer, slices, loops));
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

//TP2
int ANFParser::parsePlane(TiXmlElement* primitive, SceneNode* node)
{
	out << "            > Plane" << endl;

	int localErrors = 0;

	int parts;

	if(readInt(&primitive, &parts, "parts", ERROR) != OK)
	{
		localErrors++;
	}

	if(!localErrors)
	{
		node->addPrimitive(new Plane(parts));
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

/* Descendants */
int ANFParser::parseNodeDescendants(TiXmlElement* nodeElement, SceneNode* node)
{
	int localErrors = 0;
	int localWarnings = 0;

	TiXmlElement* descendantsElement = nodeElement->FirstChildElement("descendants");

	if(!descendantsElement)
	{
		printMsg(INFO, "The block <primitives> does not exist");
	}
	else
	{
		TiXmlElement* noderefElement = descendantsElement->FirstChildElement("noderef");

		if(noderefElement)
		{
			while(noderefElement)
			{
				string strID;

				if(readString(&noderefElement, &strID, "id", ERROR) != OK)
				{
					localErrors++;
				}
				else
				{
					out << "            > id : " << strID << endl;

					this->descendants.push_back(pair<string, string>(node->getID(), strID));
				}

				noderefElement = noderefElement->NextSiblingElement("noderef");
			}
		}
		else
		{
			printMsg(INFO, "The block <descendants> exists, but it is empty (or the content does not match <noderef> as it should)");
			localWarnings++;
		}
	}

	warnings += localWarnings;

	if(!localErrors)
	{
		printMsg(OK);
		return OK;
	}
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

/* Build nodes */
int ANFParser::linkGraphNodes() {

	int localErrors = 0;

	if(!descendants.empty())
	{
		for(unsigned int i = 0; i < this->descendants.size(); i++)
		{
			SceneNode* n = sceneData->getSceneGraph()->getNode(descendants.at(i).first);
			SceneNode* d = sceneData->getSceneGraph()->getNode(descendants.at(i).second);

			if(d)
				n->addDescendant(d);
			else
				localErrors++;
		}
	}

	if(!localErrors)
		return OK;
	else
	{
		errors += localErrors;
		return ERROR;
	}
}

/* Verify graph */
int ANFParser::verifyGraph()
{
	return sceneData->getSceneGraph()->Verify(out);
}

/* Create graph display lists*/
int ANFParser::createGraphDisplayLists()
{
	this->sceneData->getSceneGraph()->createDisplayLists();

	return OK;
}

// ***********************************************************************************
// ***********************************************************************************
// ******************************* AUXILIARS *****************************************
int ANFParser::readString(TiXmlElement** element, string* str, string descr, const int type)
{
	const char* cID = (*element)->Attribute(descr.c_str());
	if(!cID)
	{
		printMsg(type);
		out << " : Attribute '" << descr << "' is not defined" << endl;
		return type;
	}
	else
	{
		*str = cID;
		return OK;
	}
}

int ANFParser::readInt(TiXmlElement** element, int* value, string descr, const int type)
{
	if((*element)->QueryIntAttribute(descr.c_str(), value) != TIXML_SUCCESS)
	{
		printMsg(type);
		out << " : Attribute '" << descr << "' is either not defined or has illegal arguments (must be an integer)" << endl;
		return type;
	}
	else 
	{
		return OK;
	}
}

int ANFParser::readFloat(TiXmlElement** element, float* value, string descr, const int type)
{
	if((*element)->QueryFloatAttribute(descr.c_str(), value) != TIXML_SUCCESS)
	{
		printMsg(type);
		out << " : Attribute '" << descr << "' is either not defined or has illegal arguments (must be a float)" << endl;
		return type;
	}
	else 
	{
		return OK;
	}
}

int ANFParser::readAxis(TiXmlElement** element, char* value, string descr, const int type)
{
	char axis = ((*element)->Attribute(descr.c_str()))[0];

	if(!axis)
	{
		printMsg(type);
		out << " : Attribute '" << descr << "' is not defined" << endl;
		return type;
	}
	else
	{
		if(axis != 'x' && axis != 'y' && axis != 'z')
		{
			printMsg(ERROR);
			out << " : Invalid '" << descr << "' (must be in {x, y, z})" << endl;
			return ERROR;
		}
		else
		{
			*value = axis;
			return OK;
		}
	}
}

int ANFParser::readBool(TiXmlElement** element, bool* value, string descr, const int type)
{
	const char* cValue = (*element)->Attribute(descr.c_str());

	if(!cValue)
	{
		printMsg(type);
		out << " : Attribute '" << descr << "' is not defined" << endl;
		return type;
	}
	else
	{
		string strValue = cValue;
		if(strValue != "true" && strValue != "false")
		{
			printMsg(ERROR);
			out << " : Attribute '" << descr << "' has illegal arguments (must be in {true, false})" << endl;
			return ERROR;
		}
		else
		{
			*value = (strValue == "true");
			return OK;
		}
	}
}

int ANFParser::readPoint2d(TiXmlElement** element, Point2d** point, string descr, const int type)
{
	string str;

	if(readString(element, &str, descr, ERROR) != OK)
	{
		return type;
	}
	else
	{
		float a, b;
		if(sscanf_s(str.c_str(), "%f %f", &a, &b) != 2)
		{
			printMsg(ERROR);
			out << " : Attribute '" << descr << "' has illegal arguments (must be in the form \"ff ff\")" << endl;
			return ERROR;
		}
		else
		{
			(*point) = new Point2d(a, b);
			return OK;
		}
	}
}

int ANFParser::readPoint3d(TiXmlElement** element, Point3d** point, string descr, const int type)
{
	string str;
	if(readString(element, &str, descr, ERROR) != OK)
	{
		return type;
	}
	else
	{
		float a, b, c;
		if(sscanf_s(str.c_str(), "%f %f %f", &a, &b, &c) != 3)
		{
			printMsg(ERROR);
			out << " : Attribute '" << descr << "' has illegal arguments (must be in the form \"ff ff ff\")" << endl;
			return ERROR;
		}
		else
		{
			(*point) = new Point3d(a, b, c);
			return OK;
		}
	}
}

int ANFParser::readPoint4d(TiXmlElement** element, Point4d** point, string descr, const int type)
{
	string str;

	if(readString(element, &str, descr, ERROR) != OK)
	{
		return type;
	}
	else
	{
		float a, b, c, d;

		if(sscanf_s(str.c_str(), "%f %f %f %f", &a, &b, &c, &d) != 4)
		{
			printMsg(ERROR);
			out << " : Attribute '" << descr << "' has illegal arguments (must be in the form \"ff ff ff ff\")" << endl;
			return ERROR;
		}
		else
		{
			*point = new Point4d(a, b, c, d);
			return OK;
		}
	}
}

// ***********************************************************************************
// ********************************  PRINTS  *****************************************
void ANFParser::printMsg(const int type)
{
	switch(type)
	{
	case OK:
		//out << std::internal << setw(OK_WIDTH) << "[OK]" << endl;
		break;
	case WARNING:
		out << std::internal << setw(WARNING_WIDTH) << "[WARNING]";
		break;
	case ERROR:
		out << std::internal << setw(ERROR_WIDTH) << "[ERROR]";
		break;
	case INFO:
		out << std::internal << setw(INFO_WIDTH) << "[INFO]";
		break;
	default:
		break;
	}
}

void ANFParser::printMsg(const int type, string descr)
{
	printMsg(type);

	out << " : " << descr << endl;
}

bool stringIn(string & str, const string colection[])
{
	for(unsigned int i = 0; i < colection->size(); i++)
	{
		if (str == colection[i])
		{
			return true;
		}
	}

	return false;
}

// ***********************************************************************************
// ******************************  GET METHODS  **************************************
int ANFParser::getState()
{
	return state;
}

SceneData* ANFParser::getSceneData()
{
	return sceneData;
}
