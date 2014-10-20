#include "SceneData.h"

const int ERROR_WIDTH = 45;

// =======================
//  SCENE DATA
// =======================
SceneData::SceneData()
{
	// Graph
	this->sceneGraph = new SceneGraph();

	// Cameras
	this->cameras = vector<Camera* >();
	this->initialCameraSet = false;

	// Lights
	this->lights = vector<Light* >();
}

/*
	Graph
*/
SceneGraph* SceneData::getSceneGraph()
{
	return sceneGraph;
}

/*
	Globals
*/
void SceneData::setDrawing(string mode, string shading, Point4d* background) {

	this->drawing_mode = mode;

	this->drawing_shading = shading;

	this->drawing_background = background;
}

void SceneData::setCulling(string face, string order)
{
	this->culling_face = face;
	this->culling_order = order;
}

void SceneData::setLighting(bool doublesided, bool local, bool enabled, Point4d* ambient)
{
	this->lighting_doublesided = doublesided;
	this->lighting_local = local;
	this->lighting_enabled = enabled;
	this->lighting_ambient = ambient;
}

/*
	Cameras
*/
void SceneData::addCamera(Camera* cam)
{
	this->cameras.push_back(cam);
}

void SceneData::addCameraInitial(Camera* cam)
{
	this->cameras.insert(cameras.begin(), cam);
	this->initialCameraSet = true;
}

Camera* SceneData::getInitialCamera()
{
	if(!cameras.empty())
	{
		return cameras.at(0);
	}

	return NULL;
}

Camera* SceneData::getCamera(unsigned int index)
{
	if(cameras.size() > index)
	{
		return cameras.at(index);
	}

	return NULL;
}

vector<Camera* > SceneData::getCameras()
{
	return cameras;
}

bool SceneData::hasInitialCamera()
{
	return initialCameraSet;
}

bool SceneData::hasCameras()
{
	return cameras.size() > 0;
}

/*
	Lights
*/
void SceneData::addLight(Light* light)
{
	this->lights.push_back(light);
}

Light* SceneData::getLight(unsigned int index)
{
	if(lights.size() > index)
	{
		return lights.at(index);
	}

	return NULL;
}

vector<Light* > SceneData::getLights()
{
	return lights;
}

bool SceneData::hasLights()
{
	return !(lights.empty());
}

void printSpaces(int level);

// ***********************************************************
// ***********************************************************
// ========================
//  SCENE GRAPH
// ========================
SceneGraph::SceneGraph()
{
	this->nodes = std::vector<SceneNode* >();
}

SceneNode* SceneGraph::getNode(string id)
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes.at(i)->getID() == id) // Already contains this node
		{
			return nodes.at(i);
		}
	}

	return NULL;
}

SceneNode* SceneGraph::getRoot()
{
	// If root node is null, returns the first node.
	return (root == NULL ? nodes.at(0) : root);
}

vector<SceneNode*> SceneGraph::getNodes()
{
	return nodes;
}

int SceneGraph::Verify(ostream & out)
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		SceneNode* node = nodes.at(i);

		node->visited = false;
	}

	if(!(root->Verify(out)))
	{
		return 1;
	}

	return 0;
}

int SceneGraph::addNode(SceneNode* node)
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes.at(i)->getID() == node->getID()) // Already contains this node
		{
			return -1;
		}
	}

	this->nodes.push_back(node);

	return 0;
}

void SceneGraph::setRoot(SceneNode* node)
{
	root = node;
}

void SceneGraph::Process()
{
	// The precessing starts at the root node, progressing
	//  recursively throughout the tree.
	root->Process();
}

void SceneGraph::print()
{
	printf("\nPRINTING GRAPH...\n\n");
	root->print(0);
	printf("\nDONE PRINTING GRAPH...\n\n");
}

// =======================
//  SCENE NODE
// =======================
SceneNode::SceneNode(string nodeID) : id(nodeID)
{
	this->descendants = std::vector<SceneNode*>();
	this->primitives = std::vector<Primitive*>();

	glLoadIdentity();

	this->transform = new float[16];

	this->parent = NULL;
	this->hasParent = false;

	this->appearance = NULL;
	this->texture = NULL;

	this->hasAppearance = false;
	this->hasTexture = false;
	this->inherits = false;

	glGetFloatv(GL_MODELVIEW_MATRIX, transform);
}

void SceneNode::addDescendant(SceneNode* node)
{
	node->setParent(this);

	this->descendants.push_back(node);
}

void SceneNode::addPrimitive(Primitive* primitive)
{
	this->primitives.push_back(primitive);
}

void SceneNode::setParent(SceneNode* node)
{
	this->hasParent = true;
	this->parent = node;
}

void SceneNode::setAppearance(CGFappearance* app)
{
	this->hasAppearance = true;
	this->appearance = app;
}

void SceneNode::setTexture(Texture* tex)
{
	this->hasTexture = true;
	this->texture = tex;
}

void SceneNode::print(int level)
{
	std::cout << endl;
	for(int x = 0; x < level; x++)
	{
		std::cout << "  ";
	}
	std::cout << id << endl;

	// PRINT PRIMITIVES
	for(unsigned int i = 0; i < primitives.size(); i++)
	{
		printSpaces(level);
		std::cout << " " << primitives.at(i)->Type();
	}

	for(unsigned int i = 0; i < descendants.size(); i++)
	{
		for(int x = 0; x < level; x++)
		{
			printf(" ");
		}

		descendants.at(i)->print(level + 1);
	}
}

void SceneNode::setTransformMatrix()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, transform);
}

float* SceneNode::getTransformMatrix()
{
	return this->transform;
}

SceneNode* SceneNode::getParent()
{
	return this->parent;
}

Texture* SceneNode::getTexture()
{
	if(inherits && hasParent)
	{
		return parent->getTexture();
	}
	else
	{
		if(hasTexture)
		{
			return texture;
		}
		else 
		{
			return NULL;
		}
	}
}

string SceneNode::getID()
{
	return this->id;
}

std::vector<Primitive*> SceneNode::getPrimitives() {
	return this->primitives;
}

std::vector<SceneNode*> SceneNode::getDescendants() {
	return this->descendants;
}

CGFappearance* SceneNode::getAppearance()
{
	if(inherits && hasParent)
	{
		return parent->getAppearance();
	}
	else
	{
		if(hasAppearance)
		{
			return appearance;
		}
		else 
		{
			return NULL;
		}
	}
}

bool SceneNode::Verify(ostream & out)
{
	out << "            > checking... " << id << endl;
	visited = true;

	for(unsigned int i = 0; i < this->descendants.size(); i++)
	{
		if(descendants.at(i)->visited)
		{
			out << std::internal << setw(ERROR_WIDTH) << "[ERROR]";
			out << " : cycle found - '" << id << "'->'" << descendants.at(i)->getID() << "' is the end of the cycle" << endl;

			visited = false;
			return false;
		}
		else 
		{
			if(!descendants.at(i)->Verify(out))
			{
				visited = false;
				return false;
			}
		}
	}

	visited = false;

	return true;
}

void SceneNode::Process()
{
	glPushMatrix();

	glMultMatrixf(getTransformMatrix());

	CGFappearance* app = getAppearance();
	if(app != NULL)
	{
		app->apply();
	}

	for(unsigned int i = 0; i < primitives.size(); i++) 
	{
		Texture* tex = getTexture();

		if(tex != NULL)
		{
			this->primitives.at(i)->setTexture(tex);
		}
		this->primitives.at(i)->draw();
	}
	
	for(unsigned int i = 0; i < descendants.size(); i++) 
	{
		descendants.at(i)->setParent(this); // is necessary because the same child can have different fathers depending on the node being processed
		descendants.at(i)->Process();
	}

	glPopMatrix();
}

void printSpaces(int level) 
{
	for(int x = 0; x < level; x++) std::cout << "  ";
}
