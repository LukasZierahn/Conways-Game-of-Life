#include "NodeMaster.h"

NodeMaster::NodeMaster()
{
	tk = new TokenMaster();
	topmostNode = GetNode(nullptr, nullptr, nullptr, nullptr, 2);
}

int NodeMaster::GetIdAndIncrease(int level)
{
	while (level >= idVector.size())
	{
		idVector.push_back(1);
	}

	idVector.at(level)++;
	return 	idVector.at(level) - 1;
}

string NodeMaster::HashFunc(Node* nw, Node* ne, Node* sw, Node* se, int lev)
{
	int level = lev;
	int nwId = 0, neId = 0, swId = 0, seId = 0;

	if (nw != nullptr)
	{
		nwId = nw->GetId();
	}

	if (ne != nullptr)
	{
		neId = ne->GetId();
	}

	if (sw != nullptr)
	{
		swId = sw->GetId();
	}

	if (se != nullptr)
	{
		seId = se->GetId();
	}

	string key;

	int buffer = level;
	char charBuffer[2] = "A";

	for (short i = 0; i < 2; i++)
	{
		if (buffer != 0)
		{
			charBuffer[0] = (buffer % 93) + 33;
			key.append(charBuffer);
			buffer = (int)buffer / 93;
		}
		else
		{
			key.append(" ");
		}
	}


	for (int id : { nwId, neId, swId, seId })
	{
		buffer = id;

		for (short i = 0; i < 3; i++)
		{
			if (buffer != 0)
			{
				charBuffer[0] = (buffer % 93) + 33;
				key.append(charBuffer);
				buffer = (int)buffer / 93;
			}
			else
			{
				key.append(" ");
			}
		}
	}

	return key;
}

string NodeMaster::HashFunc(Node* node)
{
	Node* nw = node->Get_nw();
	Node* ne = node->Get_ne();
	Node* sw = node->Get_sw();
	Node* se = node->Get_se();

	int level = node->GetLevel() - 1;

	int nwId = 0, neId = 0, swId = 0, seId = 0;

	if (nw != nullptr)
	{
		nwId = nw->GetId();
	}

	if (ne != nullptr)
	{
		neId = ne->GetId();
	}

	if (sw != nullptr)
	{
		swId = sw->GetId();
	}

	if (se != nullptr)
	{
		seId = se->GetId();
	}

	string key;

	int buffer = level;
	char charBuffer[2] = "A";

	for (short i = 0; i < 2; i++)
	{
		if (buffer != 0)
		{
			charBuffer[0] = (buffer % 93) + 33;
			key.append(charBuffer);
			buffer = (int)buffer / 93;
		}
		else
		{
			key.append(" ");
		}
	}


	for (Node* nod : { nw, ne, sw, se })
	{
		buffer = nod->GetId();

		for (short i = 0; i < 4; i++)
		{
			if (buffer != 0)
			{
				charBuffer[0] = (buffer % 93) + 33;
				key.append(charBuffer);
				buffer = (int)buffer / 93;
			}
			else
			{
				key.append(" ");
			}
		}
	}

	return key;

}

void NodeMaster::ExpandUniverse()
{
	Node* new_nw = GetNode(nullptr, nullptr, nullptr, topmostNode->Get_nw(), topmostNode->GetLevel());
	Node* new_ne = GetNode(nullptr, nullptr, topmostNode->Get_ne(), nullptr, topmostNode->GetLevel());
	Node* new_sw = GetNode(nullptr, topmostNode->Get_sw(), nullptr, nullptr, topmostNode->GetLevel());
	Node* new_se = GetNode(topmostNode->Get_se(), nullptr, nullptr, nullptr, topmostNode->GetLevel());

	topmostNode = GetNode(new_nw, new_ne, new_sw, new_se, topmostNode->GetLevel() + 1);

}

Node* NodeMaster::AddCell(int x, int y, Node* inpNode)
{
	if (!inpNode)
	{
		while (pow(2, topmostNode->GetLevel() - 1) <= abs(x) || pow(2, topmostNode->GetLevel() - 1) <= abs(y))
		{
			ExpandUniverse();
		}
	}

	int currentx = 1, currenty = 1;
	x = x * 2;
	y = y * 2;

	Node* currentNode = inpNode;
	if (!inpNode)
	{
		currentNode = topmostNode;
	}
	string path;

	while (currentNode->GetLevel() > 0)
	{
		if (currentx > x && currenty > y)
		{
			currentx -= pow(2, currentNode->GetLevel() - 1);
			currenty -= pow(2, currentNode->GetLevel() - 1);
			currentNode = currentNode->Get_safe_sw();
			path.append("2");
		}
		else if (currentx < x && currenty > y)
		{
			currentx += pow(2, currentNode->GetLevel() - 1);
			currenty -= pow(2, currentNode->GetLevel() - 1);
			currentNode = currentNode->Get_safe_se();
			path.append("3");
		}

		else if (currentx > x && currenty < y)
		{
			currentx -= pow(2, currentNode->GetLevel() - 1);
			currenty += pow(2, currentNode->GetLevel() - 1);
			currentNode = currentNode->Get_safe_nw();
			path.append("0");
		}

		else if (currentx < x && currenty < y)
		{
			currentx += pow(2, currentNode->GetLevel() - 1);
			currenty += pow(2, currentNode->GetLevel() - 1);
			currentNode = currentNode->Get_safe_ne();
			path.append("1");
		}
	}

	if (!inpNode)
	{
		topmostNode = EmplaceNode(path, GetNode(true), topmostNode);
	}
	else
	{
		return EmplaceNode(path, GetNode(true), inpNode);
	}
}

void NodeMaster::ClearAllCells()
{
	topmostNode = GetNode(nullptr, nullptr, nullptr, nullptr, 2);
}

int NodeMaster::Turn(int turns, bool force)
{
	long long newTime = clock();

	int i = 0;
	while (turns == 0 || turns > i)
	{
		i++;

		if (createdHandedBack == createdHandedBackOld)
		{
			createdHandedBackOldCounter++;
		}
		else
		{
			createdHandedBackOldCounter = 0;
		}

		while (topmostNode->AnyNodeLives("012", "0") || topmostNode->AnyNodeLives("013", "1") || topmostNode->AnyNodeLives("023", "2") || topmostNode->AnyNodeLives("123", "3"))
		{
			ExpandUniverse();
			if (debugMode == 1)
			{
				cout << "Current Universe size: " << topmostNode->GetLevel() << endl;
			}
		}

		createdHandedBackOld = createdHandedBack;
		simulatedHandedBack = 0;
		directlyHandedBack = 0;
		emptyHandedBack = 0;
		createdHandedBack = 0;
		CRFN_Calls = 0;
		lowestConstructed = -1;

		ExpandUniverse();
		topmostNode = ComputeResulteForNodes(topmostNode);

		totalTurns++;
		totalSim += simulatedHandedBack;

		if (debugMode == 1)
		{
			cout << "Turn: " << totalTurns << " Total Nodes: " << nodeMap.size() << " CRFN: " << CRFN_Calls
				<< " Simulations: " << simulatedHandedBack << "/" << totalSim << "/65536 Results: " << directlyHandedBack << " Constructed: " << createdHandedBack << " Empties: " << emptyHandedBack
				<< " LC: " << lowestConstructed << " t: " << clock() - newTime << "/" << totalTime << endl;
		}

		totalTime += clock() - newTime;
		newTime = clock();

		if ((directlyHandedBack == 1 && CRFN_Calls == 1) || createdHandedBackOldCounter > MaxSameNumberConstructions || totalTurns > 1000)
		{
			if (!force)
			{
				if (createdHandedBackOldCounter > MaxSameNumberConstructions || totalTurns > 1000)
				{
					createdHandedBackOldCounter = 0;
					return 2;
				}
				else
				{
					createdHandedBackOldCounter = 0;
					return 0;
				}
			}
		}
	}

	return 0;
}

Node* NodeMaster::ComputeResulteForNodes(Node* inpNode)
{
	CRFN_Calls++;
	if (!inpNode)
	{
		emptyHandedBack++;
		return nullptr;
	}

	if (inpNode->Get_result())
	{
		directlyHandedBack++;
		return inpNode->Get_result();
	}

	Node* result = nullptr;

	if (inpNode->GetLevel() == 2)
	{
		tk->ClearCellList();
		for (char i = 0; i < 4; i++)
		{
			Node* subnode = inpNode->Get_subnode(i);
			if (subnode)
			{
				for (char j = 0; j < 4; j++)
				{
					if (subnode->Get_subAlive(j))
					{
						char x, y;

						if (i == 0)
						{
							x = -1;
							y = 3;
						}
						else if (i == 1)
						{
							x = 3;
							y = 3;
						}
						else if (i == 2)
						{
							x = -1;
							y = -1;
						}
						else if (i == 3)
						{
							x = 3;
							y = -1;
						}

						if (j == 0)
						{
							x -= 1;
							y += 1;
						}
						else if (j == 1)
						{
							x += 1;
							y += 1;
						}
						else if (j == 2)
						{
							x -= 1;
							y -= 1;
						}
						else if (j == 3)
						{
							x += 1;
							y -= 1;
						}

						tk->AddCell((x / 2),(y / 2));
					}
				}
			}
		}

		tk->Turn(1);

		result = GetNode(nullptr, nullptr, nullptr, nullptr, 2);
		simulatedHandedBack++;

		for (Cell c : (*tk->GetCellList()))
		{
			if (0 <= c.x <= 1 && 0 <= c.y <= 1)
			{
				result = AddCell(c.x, c.y, result);
			}
		}
		
		result = GetNode(result->Get_subnode("03"), result->Get_subnode("12"), result->Get_subnode("21"), result->Get_subnode("30"), 1);
		int stop = 1;
	}
	else
	{
		Node* nw = nullptr; Node* n = nullptr;	Node* ne = nullptr;
		Node* cw = nullptr; Node* c = nullptr;	Node* ce = nullptr;
		Node* sw = nullptr;	Node* s = nullptr;	Node* se = nullptr;

		nw	= ComputeResulteForNodes(inpNode->Get_nw());
		n = ComputeResulteForNodes(GetNode(inpNode->Get_subnode("01"), inpNode->Get_subnode("10"), inpNode->Get_subnode("03"), inpNode->Get_subnode("12"), inpNode->GetLevel() - 1));
		ne	= ComputeResulteForNodes(inpNode->Get_ne());

		cw = ComputeResulteForNodes(GetNode(inpNode->Get_subnode("02"), inpNode->Get_subnode("03"), inpNode->Get_subnode("20"), inpNode->Get_subnode("21"), inpNode->GetLevel() - 1));
		c = ComputeResulteForNodes(GetNode(inpNode->Get_subnode("03"), inpNode->Get_subnode("12"), inpNode->Get_subnode("21"), inpNode->Get_subnode("30"), inpNode->GetLevel() - 1));
		ce = ComputeResulteForNodes(GetNode(inpNode->Get_subnode("12"), inpNode->Get_subnode("13"), inpNode->Get_subnode("30"), inpNode->Get_subnode("31"), inpNode->GetLevel() - 1));

		sw = ComputeResulteForNodes(inpNode->Get_sw());
		s = ComputeResulteForNodes(GetNode(inpNode->Get_subnode("21"), inpNode->Get_subnode("30"), inpNode->Get_subnode("23"), inpNode->Get_subnode("32"), inpNode->GetLevel() - 1));
		se = ComputeResulteForNodes(inpNode->Get_se());

		createdHandedBack++;

		result = GetNode(
			GetNode(Get_subnode(nw, '3'), Get_subnode(n, '2'), Get_subnode(cw, '1'), Get_subnode(c, '0'), inpNode->GetLevel() - 2),
			GetNode(Get_subnode(n, '3'), Get_subnode(ne, '2'), Get_subnode(c, '1'), Get_subnode(ce, '0'), inpNode->GetLevel() - 2),
			GetNode(Get_subnode(cw, '3'), Get_subnode(c, '2'), Get_subnode(sw, '1'), Get_subnode(s, '0'), inpNode->GetLevel() - 2),
			GetNode(Get_subnode(c, '3'), Get_subnode(ce, '2'), Get_subnode(s, '1'), Get_subnode(se, '0'), inpNode->GetLevel() - 2),
			inpNode->GetLevel() - 1);

		if (lowestConstructed > result->GetLevel() || lowestConstructed == -1)
			lowestConstructed = result->GetLevel();
	}

	inpNode->Set_result(result);
	return result;
}

string NodeMaster::AppendLivingCellsToOutputString(Node* inpNode, int currentx, int currenty)
{
	string output;

	if (inpNode)
	{
		if (inpNode->GetLevel() > 1)
		{
			output.append(AppendLivingCellsToOutputString(inpNode->Get_nw(), currentx - pow(2, inpNode->GetLevel() - 1), currenty + pow(2, inpNode->GetLevel() - 1)));
			output.append(AppendLivingCellsToOutputString(inpNode->Get_ne(), currentx + pow(2, inpNode->GetLevel() - 1), currenty + pow(2, inpNode->GetLevel() - 1)));
			output.append(AppendLivingCellsToOutputString(inpNode->Get_sw(), currentx - pow(2, inpNode->GetLevel() - 1), currenty - pow(2, inpNode->GetLevel() - 1)));
			output.append(AppendLivingCellsToOutputString(inpNode->Get_se(), currentx + pow(2, inpNode->GetLevel() - 1), currenty - pow(2, inpNode->GetLevel() - 1)));

		}
		else if (inpNode->GetLevel() == 1)
		{
			if (inpNode->Get_nw())
			{
				if (inpNode->Get_nw()->Get_lives())
				{
					output.append(to_string((currentx - 1) / 2));
					output.append("/");
					output.append(to_string((currenty + 1) / 2));
					output.append("\n");
				}
			}

			if (inpNode->Get_ne())
			{
				if (inpNode->Get_ne()->Get_lives())
				{
					output.append(to_string((currentx + 1) / 2));
					output.append("/");
					output.append(to_string((currenty + 1) / 2));
					output.append("\n");
				}
			}

			if (inpNode->Get_sw())
			{
				if (inpNode->Get_sw()->Get_lives())
				{
					output.append(to_string((currentx - 1) / 2));
					output.append("/");
					output.append(to_string((currenty - 1) / 2));
					output.append("\n");
				}
			}

			if (inpNode->Get_se())
			{
				if (inpNode->Get_se()->Get_lives())
				{
					output.append(to_string((currentx + 1) / 2));
					output.append("/");
					output.append(to_string((currenty - 1) / 2));
					output.append("\n");
				}
			}
		}
	}

	return output;
}

Node* NodeMaster::EmplaceNode(string path, Node* inpNode, Node* startingNode)
{
	if (path.length() != startingNode->GetLevel() - inpNode->GetLevel())
	{
		throw logic_error("EmplaceNode with unfitting path called");
	}

	Node* currentNode = startingNode;
	vector<Node*> nodeList;

	for (char c : path)
	{
		nodeList.push_back(currentNode);

		if (c == '0')
		{
			currentNode = currentNode->Get_safe_nw();
		}
		else if (c == '1')
		{
			currentNode = currentNode->Get_safe_ne();
		}		
		else if (c == '2')
		{
			currentNode = currentNode->Get_safe_sw();
		}
		else if (c == '3')
		{
			currentNode = currentNode->Get_safe_se();
		}
		else
		{
			throw logic_error("EmplaceNode path was flawed");
		}
	}

	if (path[path.length() - 1] == '0')
	{
		nodeList[nodeList.size() - 1] = GetNode(inpNode, nodeList[nodeList.size() - 1]->Get_ne(), nodeList[nodeList.size() - 1]->Get_sw(), nodeList[nodeList.size() - 1]->Get_se(), nodeList[nodeList.size() - 1]->GetLevel());
	}
	else if (path[path.length() - 1] == '1')
	{
		nodeList[nodeList.size() - 1] = GetNode(nodeList[nodeList.size() - 1]->Get_nw(), inpNode, nodeList[nodeList.size() - 1]->Get_sw(), nodeList[nodeList.size() - 1]->Get_se(), nodeList[nodeList.size() - 1]->GetLevel());
	}
	else if (path[path.length() - 1] == '2')
	{
		nodeList[nodeList.size() - 1] = GetNode(nodeList[nodeList.size() - 1]->Get_nw(), nodeList[nodeList.size() - 1]->Get_ne(), inpNode, nodeList[nodeList.size() - 1]->Get_se(), nodeList[nodeList.size() - 1]->GetLevel());
	}
	else if (path[path.length() - 1] == '3')
	{
		nodeList[nodeList.size() - 1] = GetNode(nodeList[nodeList.size() - 1]->Get_nw(), nodeList[nodeList.size() - 1]->Get_ne(), nodeList[nodeList.size() - 1]->Get_sw(), inpNode, nodeList[nodeList.size() - 1]->GetLevel());
	}
	else
	{
		throw logic_error("EmplaceNode path was flawed");
	}

	for (int i = nodeList.size() - 1; i != 0; i--)
	{
		if (path[i - 1] == '0')
		{
			nodeList[i - 1] = GetNode(nodeList[i], nodeList[i - 1]->Get_ne(), nodeList[i - 1]->Get_sw(), nodeList[i - 1]->Get_se(), nodeList[i - 1]->GetLevel());
		}
		else if (path[i - 1] == '1')
		{
			nodeList[i - 1] = GetNode(nodeList[i - 1]->Get_nw(), nodeList[i], nodeList[i - 1]->Get_sw(), nodeList[i - 1]->Get_se(), nodeList[i - 1]->GetLevel());
		}
		else if (path[i - 1] == '2')
		{
			nodeList[i - 1] = GetNode(nodeList[i - 1]->Get_nw(), nodeList[i - 1]->Get_ne(), nodeList[i], nodeList[i - 1]->Get_se(), nodeList[i - 1]->GetLevel());
		}
		else if (path[i - 1] == '3')
		{
			nodeList[i - 1] = GetNode(nodeList[i - 1]->Get_nw(), nodeList[i - 1]->Get_ne(), nodeList[i - 1]->Get_sw(), nodeList[i], nodeList[i - 1]->GetLevel());
		}
		else
		{
			throw logic_error("EmplaceNode path was flawed");
		}
	}

	return nodeList[0];
}

string NodeMaster::GetOutputString(bool py)
{
	string output = string();

	if (py)
	{
		output.append("c\n");
	}

	output.append(AppendLivingCellsToOutputString(topmostNode, 1, 1));

	if (py)
	{
		output.append("e");
	}

	return output;
}

void NodeMaster::ResetHash()
{
	for (map<string, Node*>::iterator it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		Node* nod = it->second;
		delete nod;
	}
	nodeMap.clear();
	idVector.clear();

	ClearAllCells();
}

Node* NodeMaster::GetNode(Node* nw, Node* ne, Node* sw, Node* se, int lev)
{
	for (Node* n : { nw, ne, sw, se })
	{
		if (n)
		{
			if (n->GetLevel() != lev - 1)
			{
				throw logic_error("GetNode called with Node of a unfitting level");
			}
		}
	}

	string key = HashFunc(nw, ne, sw, se, lev);
	Node* buffer;

	if (nodeMap.count(key))
	{
		return nodeMap.at(key);;
	}
	else
	{

		buffer = new Node(this, lev, GetIdAndIncrease(lev), nw, ne, sw, se);
		nodeMap.emplace(key, buffer);
		return buffer;
	}
}

Node* NodeMaster::GetNode(bool alive)
{
	if (alive)
	{
		string key;
		key.append("00000000000001");

		if (nodeMap.count(key))
		{
			return nodeMap[key];
		}
		else
		{
			Node* buffer = new Node(this, 0, GetIdAndIncrease(0), nullptr, nullptr, nullptr, nullptr);
			buffer->Set_lives(alive);
			nodeMap.emplace(key, buffer);
			return buffer;
		}
	}
	else
	{
		return nullptr;
	}
}

NodeMaster::~NodeMaster()
{
	delete tk;
	for (map<string, Node*>::iterator it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		delete it->second;
	}
}
