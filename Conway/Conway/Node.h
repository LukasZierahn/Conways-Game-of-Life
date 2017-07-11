#include "Include.h"
#include "TokenMaster.h"

#ifndef NodeHeader
#define NodeHeader

class NodeMaster;

class Node
{
private:
	//subnodes (Nodes one level below this one)
	Node* nw = nullptr;	Node* ne = nullptr;
	Node* sw = nullptr;	Node* se = nullptr;

	Node* result = nullptr;

	//if the node is level 0
	bool lives;
	bool anyLivesInsideMe = true;

	int level;
	int id = 1;

	NodeMaster* nd;

public:
	Node(NodeMaster* parent, int level, int id, Node* nw = nullptr, Node* ne = nullptr, Node* sw = nullptr, Node* se = nullptr);

	int GetLevel() { return level; };
	int GetId() { return id; };

	Node* Get_subnode(char subnode);
	Node* Get_subnode(string subnode);
	bool Get_subAlive(char subnode);
	bool AnyNodeLives(string subnodes = string("0123"), string path = string());

	inline bool GetAnyLivesInsideMe() { return anyLivesInsideMe; };
	inline void SetAnyLivesInsideMe(bool set) { anyLivesInsideMe = set; };

	Node* Get_nw() { return nw; };
	Node* Get_ne() { return ne; };
	Node* Get_sw() { return sw; };
	Node* Get_se() { return se; };

	Node* Get_safe_nw();
	Node* Get_safe_ne();
	Node* Get_safe_sw();
	Node* Get_safe_se();

	inline Node* Get_result() { return result; };
	inline void Set_result(Node* newResult) { result = newResult; };

	inline void Set_lives(bool alive) { lives = alive; };
	inline bool Get_lives() { return lives; }
	~Node();
};

inline Node* Get_subnode(Node* inpNode, char subnode)
{
	if (inpNode)
	{
		if (subnode == 0 || subnode == '0')
		{
			return inpNode->Get_nw();
		}
		else if (subnode == 1 || subnode == '1')
		{
			return inpNode->Get_ne();
		}
		else if (subnode == 2 || subnode == '2')
		{
			return inpNode->Get_sw();
		}
		else if (subnode == 3 || subnode == '3')
		{
			return inpNode->Get_se();
		}
	}

	return nullptr;
}

#endif