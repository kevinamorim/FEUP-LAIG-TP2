#include "ParserContainer.h"

const int ERROR_WIDTH = 45;

// **************************************
//	PARSER CONTAINER
// **************************************
ParserContainer::ParserContainer()
{
	this->nodes = vector<ParserNode* >();
	this->textures = vector<Texture* >();
	this->appearances = vector<Appearance* >();
}

string ParserContainer::getGraphRootID()
{
	return rootID;
}

vector<ParserNode*> ParserContainer::getGraphNodes()
{
	return this->nodes;
}

ParserNode* ParserContainer::getGraphNode(int index)
{
	return this->nodes.at(index);
}

ParserNode* ParserContainer::getGraphNode(string nodeID)
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes.at(i)->getID() == nodeID)
		{
			return nodes.at(i);
		}
	}

	return NULL;
}

void ParserContainer::setGraphRootID(string rootID)
{
	this->rootID = rootID;
}

int ParserContainer::addGraphNode(ParserNode* node)
{
	if(existsInGraph(node->getID()) != 0) { 
		return -1;
	}

	this->nodes.push_back(node);

	return 0;
}

bool ParserContainer::existsInGraph(string nodeID) 
{
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes.at(i)->getID() == nodeID)
		{
			return true;
		}
	}

	return false;
}

int ParserContainer::verifyGraphCoherence(ostream & out)
{
	int localErrors = 0;

	out << "            > Existance..." << endl;
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		for(unsigned int j = 0; j < nodes.at(i)->getDescendants().size(); j++)
		{
			if(!existsInGraph(nodes.at(i)->getDescendant(j)))
			{
				out << std::internal << setw(ERROR_WIDTH) << "[ERROR]";
				out << " : Invalid descendant <" << nodes.at(i)->getDescendant(j) << "> the descendant is not defined (the parent node refering it is <" << nodes.at(i)->getID() << ">" << endl;
				localErrors++;
			}
		}
	}

	out << "            > Linkage..." << endl;
	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		ParserNode* node = nodes.at(i);

		if(node->getID() != this->rootID)
		{
			bool foundReference = false;

			for(unsigned int j = 0; j < nodes.size(); j++)
			{
				ParserNode* compNode = nodes.at(j);

				if(node->getID() != compNode->getID())
				{
					for(unsigned int w = 0; w < compNode->getDescendants().size(); w++)
					{
						if(compNode->getDescendant(w) == node->getID())
						{
							foundReference = true;
							break;
						}
					}
				}
			}

			if(!foundReference)
			{
				out << std::internal << setw(ERROR_WIDTH) << "[ERROR]";
				out << " : although the node '" << node->getID() << "' is not the root, it is never referenced as a descendant (unreachable node)" << endl;
				localErrors++;
			}
		}
	}

	if(!localErrors)
	{
		return 0;
	}
	else
	{
		return localErrors;
	}
}

// -----------
//	textures
// -----------
void ParserContainer::addTexture(Texture* tex)
{
	this->textures.push_back(tex);
}

Texture* ParserContainer::getTexture(unsigned int index)
{
	if(textures.size() > index)
	{
		return textures.at(index);
	}
	
	return NULL;
}

vector<Texture* > ParserContainer::getTextures()
{
	return textures;
}

// -----------
//	appearances
// -----------
void ParserContainer::addAppearance(Appearance* app)
{
	this->appearances.push_back(app);
}

Appearance* ParserContainer::getAppearance(unsigned int index)
{
	if(appearances.size() > index)
	{
		return appearances.at(index);
	}
	
	return NULL;
}

Appearance* ParserContainer::getAppearance(string id)
{
	for(unsigned int i = 0; i < appearances.size(); i++)
	{
		Appearance* app = appearances.at(i);
		if(app->getID() == id)
		{
			return app;
		}
	}

	return NULL;
}

vector<Appearance* > ParserContainer::getAppearances()
{
	return appearances;
}


// **************************************
//	PARSER NODE
// **************************************
ParserNode::ParserNode(string nodeID) : id(nodeID)
{
	this->transforms = std::queue<Transform*>();
	this->descendants = std::vector<string>();
	this->primitives = std::vector<Primitive*>();
	this->appearanceRef = "";
}

void ParserNode::addAppearance(string appearanceRef)
{
	this->appearanceRef = appearanceRef;
}

void ParserNode::addDescendant(string descendant)
{
	this->descendants.push_back(descendant);
}

void ParserNode::addPrimitive(Primitive* primitive)
{
	this->primitives.push_back(primitive);
}

void ParserNode::addTransform(Transform* transform)
{
	this->transforms.push(transform);
}

string ParserNode::getID()
{
	return id;
}

string ParserNode::getAppearance()
{
	return this->appearanceRef;
}

std::vector<Primitive*> ParserNode::getPrimitives()
{
	return this->primitives;
}

std::vector<string> ParserNode::getDescendants()
{
	return this->descendants;
}

std::queue<Transform*> ParserNode::getTransforms()
{
	return this->transforms;
}

Primitive* ParserNode::getPrimitive(int index)
{
	return this->primitives.at(index);
}

string ParserNode::getDescendant(int index)
{
	return this->descendants.at(index);
}

void ParserNode::print()
{
	printf("ID: %s\n", id);

	std::queue<Transform*> temp = std::queue<Transform*>();

	printf("  Transforms:\n");
	while(!transforms.empty())
	{
		Transform* t = transforms.front();

		printf("    ");t->print();

		temp.push(t);
		transforms.pop();
	}
	while(!temp.empty())
	{
		Transform* t = temp.front();
		transforms.push(t);
		temp.pop();
	}

	cout << " Appearance:" << endl;
	if(!appearanceRef.empty())
	{
		cout << " > " << appearanceRef << endl;
	}

	cout << " Primitives:" << endl;
	for(unsigned int i = 0; i < primitives.size(); i++)
	{
		cout << " > " << primitives[i]->Type() << endl;
	}

	cout << " Descendants:" << endl;
	for(unsigned int i = 0; i < descendants.size(); i++)
	{
		cout << " > " << descendants[i] << endl;
	}
}


