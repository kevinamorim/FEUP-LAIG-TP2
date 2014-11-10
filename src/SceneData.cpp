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

	// Appearances
	this->appearances = vector<Appearance *>();

	// Textures
	this->textures = vector<Texture* >();
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


/*
Appearances
*/
void SceneData::addAppearance(Appearance * app) {
	this->appearances.push_back(app);
}

Appearance* SceneData::getAppearance(string id) {
	for(unsigned int i = 0; i < this->appearances.size(); i++) {
		if(this->appearances.at(i)->getID() == id) return this->appearances.at(i);
	}
	return NULL;
}

void SceneData::addTexture(Texture* tex)
{
	this->textures.push_back(tex);
}

Texture* SceneData::getTexture(string id) {
	for(unsigned int i = 0; i < this->textures.size(); i++) {
		if(this->textures.at(i)->getID() == id) return this->textures.at(i);
	}
	return NULL;
}

// ***********************************************************
// ***********************************************************
// ========================
//  SCENE GRAPH
// ========================
SceneGraph::SceneGraph()
{
	this->nodes = std::vector<SceneNode* >();

	this->root = NULL;

	this->appearancesStack = stack<Appearance*>();
}

SceneNode* SceneGraph::getNode(string id)
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes.at(i)->getID() == id)
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
		return -1;
	}

	return 0;
}

void SceneGraph::addNode(SceneNode* node)
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes.at(i)->getID() == node->getID()) // Already contains this node
		{
			return;
		}
	}

	this->nodes.push_back(node);

	node->setAppearancesStack(&appearancesStack);
}

void SceneGraph::setRoot(SceneNode* node)
{
	root = node;
}

void SceneGraph::Display()
{
	// The precessing starts at the root node, progressing
	//  recursively throughout the tree.
	root->Display();
}

void SceneGraph::createDisplayLists()
{
	this->root->createDisplayTree();
}

bool SceneGraph::hasRoot()
{
	return this->root != NULL;
}

// =======================
//  SCENE NODE
// =======================
bool SceneNode::usingDL = false;

SceneNode::SceneNode(string nodeID, bool usesDL) : id(nodeID)
{
	this->usesDisplayList = usesDL;
	this->displayListIndex = 0;

	this->descendants = std::vector<SceneNode*>();
	this->primitives = std::vector<Primitive*>();

	this->appearance = NULL;

	this->hasAppearance = false;
	this->inherits = true;

	glLoadIdentity();

	this->transform = new float[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, transform);
}

void SceneNode::addDescendant(SceneNode* node)
{
	this->descendants.push_back(node);
}

void SceneNode::addPrimitive(Primitive* primitive)
{
	this->primitives.push_back(primitive);
}

void SceneNode::setAppearance(Appearance* app)
{
	this->inherits = false;
	this->hasAppearance = true;
	this->appearance = app;
}

void SceneNode::setTransformMatrix()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, transform);
}

float* SceneNode::getTransformMatrix()
{
	return this->transform;
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

void SceneNode::setAppearancesStack(stack<Appearance*> *apps)
{
	appearancesStack = apps;
}

void SceneNode::addAppearanceToStack(Appearance* app)
{
	appearancesStack->push(app);
	appearancesStack->top()->apply();
}

void SceneNode::removeAppearanceFromStack()
{
	if(!appearancesStack->empty())
	{
		appearancesStack->pop();
		if(!appearancesStack->empty())
		{
			appearancesStack->top()->apply();
		}
	}
}

void SceneNode::Display()
{
	if(usingDL && this->usesDisplayList && this->hasDisplayList())
	{
		glCallList(this->displayListIndex);
	}
	else
	{
		glPushMatrix();

		glMultMatrixf(getTransformMatrix());

		if(hasAppearance)
		{
			addAppearanceToStack(this->appearance);
		}

		// call the animation transforms (if the node is animated)

		drawPrimitives();

		for(unsigned int i = 0; i < descendants.size(); i++) 
		{
			descendants.at(i)->Display();
		}

		if(hasAppearance)
		{
			removeAppearanceFromStack();
		}

		glPopMatrix();
	}
}

//TP2
bool SceneNode::hasDisplayList()
{
	return (displayListIndex != 0);
}

void SceneNode::createDisplayTree()
{
	for(unsigned int i = 0; i < this->descendants.size(); i++)
	{
		this->descendants.at(i)->createDisplayTree();
	}

	if(usesDisplayList)
	{
		createDisplayList();
	}
}

void SceneNode::createDisplayList()
{
	bool firstTime = false;

	if(usesDisplayList)
	{
		if(hasDisplayList())
		{
			glCallList(this->displayListIndex);
		}
		else
		{
			firstTime = true;
			displayListIndex = glGenLists(1);
			glNewList(displayListIndex, GL_COMPILE);
		}
	}

	glPushMatrix();
	glMultMatrixf(getTransformMatrix());

	if(hasAppearance)
	{
		addAppearanceToStack(this->appearance);
	}

	drawPrimitives();

	for(unsigned int i = 0; i < descendants.size(); i++) 
	{
		descendants.at(i)->createDisplayList();
	}

	if(hasAppearance)
	{
		removeAppearanceFromStack();
	}

	glPopMatrix();

	if(usesDisplayList && firstTime)
	{
		glEndList();
	}
}

void SceneNode::drawPrimitives()
{
	for(unsigned int i = 0; i < primitives.size(); i++) 
	{
		if(!appearancesStack->empty())
		{
			if(appearancesStack->top())
			{
				if(appearancesStack->top()->hasTexture())
				{
					Texture* tex = appearancesStack->top()->getTexture();

					primitives.at(i)->setTextureParams(tex->S(), tex->T());
				}
			}
		}

		primitives.at(i)->draw();
	}
}