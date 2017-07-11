#include "StartStateRow.h"

StartStateRow::StartStateRow(int s)
{
	ChangeSize(s);
}

void StartStateRow::ChangeSize(int s)
{
	size = s;
	data.clear();

	for (int i = 0; i < s; i++)
	{
		data.push_back(false);
	}

	data.shrink_to_fit();
}

int StartStateRow::TotalCellsAlive()
{
	int counter = 0;
	for (bool b : data)
	{
		if (b)
		{
			counter++;
		}
	}

	return counter;
}

bool StartStateRow::AdvanceState()
{
	int alive = TotalCellsAlive();
	index++;


	for (int i = size - 1; i >= 0; i--)
	{
		if (data.at(i))
		{
			if (i != size - 1 && !data.at(i + 1))
			{
				data.at(i) = false;
				data.at(i + 1) = true;
				return false;
			}
		}
	}

	ChangeSize(size);

	if (alive == size)
	{
		index = 0;
		return true;
	}

	for (int j = 0; j < alive + 1; j++)
	{
		data.at(j) = true;
	}

	return false;
}

StartStateRow::~StartStateRow()
{
}
