#include "Include.h"

#ifndef TokenMasterHeader
#define TokenMasterHeader

class TokenMaster
{
private:
	vector<Cell> cellList;
	map<int, map<int, Token> > tokenList;

	vector<char> births = { 4 };
	vector<char> stays = { 2,3 };

public:
	TokenMaster();
	void ClearCellList();
	void AddCell(int, int);
	void Turn(int);
	string GetOutputString();

	vector<Cell>* GetCellList() { return &cellList; };

	void AddBirths(int add);
	void AddStays(int add);

	void PopBirths(int pop);
	void PopStays(int pop);

	void ResetRule() { births.clear(); stays.clear(); };

	string GetCurrentRule();

	~TokenMaster();
};

#endif
