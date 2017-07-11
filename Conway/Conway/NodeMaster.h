#ifndef NodeMasterHeader
#define NodeMasterHeader

#include "Include.h"
#include "TokenMaster.h"
#include "Node.h"

#define MaxSameNumberConstructions 20
class Node;

class NodeMaster
{
private:
	TokenMaster* tk;

	Node* topmostNode;
	map<string, Node*> nodeMap;

	vector<int> idVector;

	int totalTurns = 0;
	int totalTime = 0;
	int CRFN_Calls = 0;
	int directlyHandedBack = 0;
	int simulatedHandedBack = 0;
	int totalSim = 0;
	int emptyHandedBack = 0;
	int createdHandedBack = 0;
	int createdHandedBackOld = 0;
	int createdHandedBackOldCounter = 0;
	int lowestConstructed = 0;
	int debugMode = 0;

	int GetIdAndIncrease(int level);
	string AppendLivingCellsToOutputString(Node* inpNode, int currentx, int currenty);
	Node* EmplaceNode(string path, Node* inpNode, Node* startingNode);
	Node* ComputeResulteForNodes(Node* inpNode);

public:
	NodeMaster();
	void ExpandUniverse();
	Node* AddCell(int, int, Node* inp = nullptr);
	void ClearAllCells();
	int Turn(int turns, bool force = false);
	string GetOutputString(bool py = true);

	int GetSimulatedNodesCount() { return totalSim; };

	void SetDebugMode(int db) { debugMode = db; };
	int GetDebugMode() { return debugMode; };

	void ResetTurnsAndTime() { totalTime = 0; totalTurns = 0; createdHandedBackOldCounter = 0; };
	int GetTotalTime() { return totalTime; };
	int GetTotalTurns() { return totalTurns; };

	Node* GetTopmostNode() { return topmostNode; };
	void SetTopmostNode(Node* tmn) { topmostNode = tmn; };

	TokenMaster* GetTokenMaster() { return tk; }

	void ResetHash();

	Node* GetNode(Node* nw, Node* ne, Node* sw, Node* se, int level = -1);
	Node* GetNode(bool alive);

	string HashFunc(Node* nw, Node* ne, Node* sw, Node* se, int level = -1);
	string HashFunc(Node* node);

	~NodeMaster();
};

#endif