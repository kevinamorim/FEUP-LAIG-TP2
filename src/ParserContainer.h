#ifndef _PARSERGRAPH_H_
#define _PARSERGRAPH_H_

#include "Primitives.h"
#include "Transform.h"
#include "Appearance.h"

#include <string>
#include <vector>
#include <queue>
#include <stdio.h>

#include <iostream>
#include <iomanip>

using namespace std;

// Contains all the temporary information
class ParserNode
{
private:
	// The node's id
	string id;

	// A reference to the 'appearance' of this node (which will be applied for primitives or child nodes, unless they overwrite it)
	string appearanceRef;

	// Contains the tranforms.
	// It is a queue for the simple reason that the
	//	transforms will be read in the inverse order of their application.
	std::queue<Transform*> transforms;

	// Contains the reference to the primitives of this node
	std::vector<Primitive*> primitives;

	// Contains the id of the child nodes.
	// This information will later be linked to the actual scene graph
	// It is built this way by the parser because a child CAN be declared PRIOR to its declaration,
	//	therefore the linkage is done only after all the nodes are read.
	std::vector<string> descendants;

public:
	ParserNode(string nodeID);
	~ParserNode();

	string getID();
	string  getAppearance();
	vector<Primitive*> getPrimitives();
	vector<string> getDescendants();
	queue<Transform*> getTransforms();

	Primitive* getPrimitive(int index);
	string getDescendant(int index);
	Transform* getTransform(int index);

	void print();

	void addTransform(Transform*);
	void addDescendant(string descendant);
	void addPrimitive(Primitive* primitive);
	void addAppearance(string appearanceRef);
};

class ParserContainer
{
private:
	// GRAPH STUFF
	vector<ParserNode*> nodes;
	string rootID;

	// TEXTURES STUFF
	vector<Texture* > textures;

	// APPEARANCE STUFF
	vector<Appearance* > appearances; 

public:
	ParserContainer();
	~ParserContainer();

	// ---------------
	//  GRAPH METHODS
	// ---------------
	// SET / ADD
	void setGraphRootID(string rootID);
	int addGraphNode(ParserNode* node);

	// GET
	string getGraphRootID();
	vector<ParserNode*> getGraphNodes();
	ParserNode* getGraphNode(int index);
	ParserNode* getGraphNode(string nodeID);

	// OTHERS
	bool existsInGraph(string nodeID);
	int verifyGraphCoherence(ostream & out);

	// ------------------
	//  TEXTURES
	// ------------------
	void addTexture(Texture* tex);
	Texture* getTexture(unsigned int index);
	vector<Texture* > getTextures();

	// ------------------
	//  TEXTURES
	// ------------------
	void addAppearance(Appearance* app);
	Appearance* getAppearance(unsigned int index);
	Appearance* getAppearance(string id);
	vector<Appearance* > getAppearances();
};

#endif