#include "TokenMaster.h"


TokenMaster::TokenMaster()
{
}

void TokenMaster::ClearCellList()
{
	cellList.clear();
}

void inline TokenMaster::AddCell(int x, int y)
{
	Cell newCell;
	newCell.x = x;
	newCell.y = y;
	for (Cell c : cellList)
	{
		if (c.x == x && c.y == y)
			throw logic_error("Tried to double add Cell");
	}
	cellList.push_back(newCell);
}

void TokenMaster::Turn(int amount)
{
	int bufx, bufy;

	for (int i = 0; i < amount; i++)
	{
		tokenList.clear();
		for (Cell c : cellList)
		{
			for (char x = -1; x < 2; x++)
			{
				for (char y = -1; y < 2; y++)
				{
					bufx = c.x + x;
					bufy = c.y + y;

					if (!tokenList.count(bufx))
					{
						tokenList.emplace(bufx, map<int, Token>());
						tokenList[bufx].emplace(c.y + y, Token());
						tokenList[bufx][bufy].x = bufx;
						tokenList[bufx][bufy].y = bufy;
					}
					else if (!tokenList[bufx].count(bufy))
					{
						tokenList[bufx].emplace(c.y + y, Token());
						tokenList[bufx][bufy].x = bufx;
						tokenList[bufx][bufy].y = bufy;
					}

					if (x == 0 && y == 0)
					{
						tokenList[bufx][bufy].aliveOnMe = true;
					}
					else
					{
						tokenList[bufx][bufy].aliveNextToMe++;
					}

				}
			}
		}

		cellList.clear();

		for (map<int, map<int, Token>>::iterator firstIt = tokenList.begin(); firstIt != tokenList.end(); firstIt++)
		{
			for (map<int, Token>::iterator secondIt = firstIt->second.begin(); secondIt != firstIt->second.end(); secondIt++) //OBJECTION
			{
				if (secondIt->second.aliveOnMe)
				{
					for (char i : stays)
					{
						if (secondIt->second.aliveNextToMe == i)
						{
							AddCell(secondIt->second.x, secondIt->second.y);
							break;
						}
					}
				}
				else
				{
					for (char i : births)
					{
						if (secondIt->second.aliveNextToMe == i)
						{
							AddCell(secondIt->second.x, secondIt->second.y);
							break;
						}
					}
				}
			}
		}
	}
}

string TokenMaster::GetOutputString()
{
	string output = string();

	output.append("c\n");
	for (Cell c : cellList)
	{
		output.append(to_string(c.x));
		output.append("/");
		output.append(to_string(c.y));
		output.append("\n");
	}

	output.append("e");

	return output;
}


void TokenMaster::AddBirths(int add)
{
	for (char n : births)
	{
		if (n == add)
		{
			return;
		}
	}
	births.push_back(add);
	births.shrink_to_fit();
}

void TokenMaster::AddStays(int add)
{
	for (char n : stays)
	{
		if (n == add)
		{
			return;
		}
	}
	stays.push_back(add);
	stays.shrink_to_fit();
}

void TokenMaster::PopBirths(int pop)
{
	births.erase(remove(births.begin(), births.end(), pop), births.end());
	births.shrink_to_fit();
}

void TokenMaster::PopStays(int pop)
{
	stays.erase(remove(stays.begin(), stays.end(), pop), stays.end());
	stays.shrink_to_fit();
}

string TokenMaster::GetCurrentRule()
{
	string buf = "";

	for (char n : births)
	{
		buf += (n + 48); //changing the char numbers from 0-9 to ASCII characters from 0-9 and adding them
	}
	buf.append(",");
	for (char n : stays)
	{
		buf += (n + 48);
	}

	return buf;
}

TokenMaster::~TokenMaster()
{
}
