#ifndef CheckBotheader
#define CheckBotheader

#include "Include.h"
#include "TokenMaster.h"
#include "NodeMaster.h"
#include "Node.h"

class StartStateRow;

class CheckBot
{
private:
	NodeMaster* nd;
	string currentTxt = "a";

	//stuff for the search
	Node* topmostBuffer = nullptr;
	int currentSize = 0;
	int totalTries = 0;
	int dTime = 0;
	vector<StartStateRow> startState;

	void IncreaseCurrentTxt();

public:
	CheckBot();
	NodeMaster* GetNodeMaster() { return nd; }
	string SaveStateToTxt(string path = "", string folder = "");
	bool LoadStateFromTxt(string);

	string GetCurrentTxt() { return currentTxt; };

	void SetCurrentSize(int);
	void AdvanceState(bool funcDebug = false);
	void PutStateInNode();

	TokenMaster* GetTokenMaster() { return nd->GetTokenMaster(); }

	void SearchForAnomalies();
	~CheckBot();
};

#endif