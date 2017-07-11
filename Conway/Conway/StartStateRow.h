#include "Include.h"

#ifndef StartStateRowheader
#define StartStateRowheader

class StartStateRow
{
protected:
	int size;
	int index = 0;

	vector<bool> data;

public:
	StartStateRow(int);

	void ChangeSize(int);
	bool GetCell(int x) { return data.at(x); };
	int TotalCellsAlive();
	int GetIndex() { return index; };

	bool AdvanceState();

	~StartStateRow();
};

#endif