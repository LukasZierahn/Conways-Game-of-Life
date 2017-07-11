#include "Node.h"
#include "NodeMaster.h"


Node::Node(NodeMaster* nodmas, int l, int d, Node* set_nw, Node* set_ne, Node* set_sw, Node* set_se)
{
	nd = nodmas;
	level = l;
	id = d;

	nw = set_nw;
	ne = set_ne;
	sw = set_sw;
	se = set_se;
}

Node* Node::Get_subnode(char subnode)
{
	
	if (subnode == 0 || subnode == '0')
	{
		return nw;
	}
	else if (subnode == 1 || subnode == '1')
	{
		return ne;
	}
	else if (subnode == 2 || subnode == '2')
	{
		return sw;
	}
	else if (subnode == 3 || subnode == '3')
	{
		return se;
	}

	throw logic_error("Called Get_subnode with wrong parameters");
	return nullptr;

}

Node* Node::Get_subnode(string subnode)
{
	Node* currentNode = this;
	for (char sub : subnode)
	{
		if (!currentNode)
		{
			return nullptr;
		}
		else if (sub == 0 || sub == '0')
		{
			currentNode = currentNode->Get_nw();
		}
		else if (sub == 1 || sub == '1')
		{
			currentNode =  currentNode->Get_ne();
		}
		else if (sub == 2 || sub == '2')
		{
			currentNode =  currentNode->Get_sw();
		}
		else if (sub == 3 || sub == '3')
		{
			currentNode = currentNode->Get_se();
		}
		else
		{
			throw logic_error("Called Get_subnode with wrong parameters");
		}
	}

	return currentNode;
}

bool Node::Get_subAlive(char subnode)
{
	if (level != 1)
	{
		throw logic_error("Called Get_subAlive on none level 1 Node");
	}

	if ((subnode == 0 || subnode == '0') && nw)
	{
		return nw->Get_lives();
	}
	else if ((subnode == 1 || subnode == '1') && ne)
	{
		return ne->Get_lives();
	}
	else if ((subnode == 2 || subnode == '2') && sw)
	{
		return sw->Get_lives();
	}
	else if ((subnode == 3 || subnode == '3') && se)
	{
		return se->Get_lives();
	}

	return false;
}

bool Node::AnyNodeLives(string subnodes, string path)
{
	Node* inpNode = this;

	if (path.length())
	{
		inpNode = Get_subnode(path);
		if (!inpNode)
		{
			return false;
		}
	}

	if (inpNode->GetLevel() == 0)
	{
		return inpNode->Get_lives();
	}

	if (!inpNode->GetAnyLivesInsideMe())
	{
		return false;
	}

	for (char c : subnodes)
	{
		if ((c == 0 || c == '0') && inpNode->Get_nw())
		{
			if (inpNode->Get_nw()->AnyNodeLives())
			{
				return true;
			}
		}
		else if ((c == 1 || c == '1') && inpNode->Get_ne())
		{
			if (inpNode->Get_ne()->AnyNodeLives())
			{
				return true;
			}
		}
		else if ((c == 2 || c == '2') && inpNode->Get_sw())
		{
			if (inpNode->Get_sw()->AnyNodeLives())
			{
				return true;
			}
		}
		else if ((c == 3 || c == '3') && inpNode->Get_se())
		{
			if (inpNode->Get_se()->AnyNodeLives())
			{
				return true;
			}
		}
	}

	inpNode->SetAnyLivesInsideMe(false);
	return false;
}

Node* Node::Get_safe_nw()
{
	if (level == 0)
	{
		throw logic_error("called Get_safe_nw() on a level 0 Node");
	}

	if (nw == nullptr && level >= 1)
	{
		nw = nd->GetNode(nullptr, nullptr, nullptr, nullptr, level - 1);
	}
	else if (nw == nullptr && level == 1)
	{
		nw = nd->GetNode(false);
	}

	return nw;
}

Node* Node::Get_safe_ne()
{
	if (level == 0)
	{
		throw logic_error("called Get_safe_ne() on a level 0 Node");
	}


	if (ne == nullptr && level >= 1)
	{
		ne = nd->GetNode(nullptr, nullptr, nullptr, nullptr, level - 1);
	}
	else if (ne == nullptr && level == 1)
	{
		ne = nd->GetNode(false);
	}

	return ne;
}

Node* Node::Get_safe_sw()
{
	if (level == 0)
	{
		throw logic_error("called Get_safe_sw() on a level 0 Node");
	}

	if (sw == nullptr && level >= 1)
	{
		sw = nd->GetNode(nullptr, nullptr, nullptr, nullptr, level - 1);
	}
	else if (sw == nullptr && level == 1)
	{
		sw = nd->GetNode(false);
	}

	return sw;
}

Node* Node::Get_safe_se()
{
	if (level == 0)
	{
		throw logic_error("called Get_safe_se() on a level 0 Node");
	}

	if (se == nullptr && level >= 1)
	{
		se = nd->GetNode(nullptr, nullptr, nullptr, nullptr, level - 1);
	}
	else if (se == nullptr && level == 1)
	{
		se = nd->GetNode(false);
	}

	return se;
}


Node::~Node()
{
}
