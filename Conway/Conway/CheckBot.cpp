#include "CheckBot.h"
#include "StartStateRow.h"

CheckBot::CheckBot()
{
	nd = new NodeMaster();
}

void CheckBot::IncreaseCurrentTxt()
{
	for (int i = currentTxt.size() - 1; i >= -1; i--)
	{
		if (i == -1)
		{
			for (int j = 0; j < currentTxt.size(); j++)
			{
				currentTxt.at(j) = 'a';
			}

			currentTxt.append("a");
			break;
		}

		if (currentTxt.at(i) != 'z')
		{
			currentTxt.at(i) = currentTxt.at(i) + 1;

			for (int j = 0; j < currentTxt.size() - i - 1; j++)
			{
				currentTxt.at(currentTxt.size() - j - 1) = 'a';
			}

			break;
		}
	}
}


string CheckBot::SaveStateToTxt(string path, string folder)
{
	string outputString = nd->GetOutputString(false);
	string name = currentTxt;
	string ruleset = nd->GetTokenMaster()->GetCurrentRule() + '\n';

	if (outputString != "")
	{

		ofstream output;

		if (path != "")
		{
			output.open(path);
			name = path;
		}
		else
		{
			name.append(".cgol");

			if (folder == "")
			{
				output.open(".\\data\\end\\" + name);
			}
			else
			{
				output.open(folder + name);
				IncreaseCurrentTxt();
			}

			if (nd->GetDebugMode() && folder == "")
			{
				cout << "Found something (" << name << ")" << endl;
			}
		}

		output << ruleset << outputString << endl;

		output.close();

	}

	return name;
}

bool CheckBot::LoadStateFromTxt(string path)
{
	ifstream input;
	input.open(path);

	int x, y;
	string buffer;
	char inp;
	bool births = true;

	if (!input.good())
	{
		return false;
	}
	

	//reading the ruleset in
	nd->GetTokenMaster()->ResetRule();
	nd->ResetHash(); //technicly this makes the programme slower but it only occures when a file gets read in, so it does not happen during analysing and
					 //only during visually looking at the results through the python script which will be the bottlenecking factor anyways.
	inp = input.get();
	while (inp != '\n')
	{
		if (inp == ',')
		{
			births = false;
			inp = input.get();
			continue;
		}

		if (births)
		{
			nd->GetTokenMaster()->AddBirths(inp - 48);
		}
		else
		{
			nd->GetTokenMaster()->AddStays(inp - 48);
		}

		inp = input.get();
	}

	//reading the cells in
	while (!input.eof())
	{
		input >> x;
		input.get();
		input >> y;
		nd->AddCell(x, y);
		input.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	input.close();

	return true;
}

void CheckBot::SearchForAnomalies()
{
	totalTries++;
	nd->ResetTurnsAndTime();

	PutStateInNode();

	topmostBuffer = nd->GetTopmostNode();

	int result = nd->Turn(0);
	dTime += nd->GetTotalTime();

	if (nd->GetDebugMode() == 2)
	{
		cout << "Try number: " << totalTries << " Simulated: " << nd->GetSimulatedNodesCount() << "/65536  Turns: " << nd->GetTotalTurns() << " Size: " << startState.size()
			<< " Text: " << currentTxt << " Time: " << nd->GetTotalTime() << "ms" << endl;
	}

	if (totalTries % 50 == 0 && nd->GetDebugMode() == 3)
	{
		cout << "Try number: " << totalTries << " Simulated: " << nd->GetSimulatedNodesCount() << "/65536 Size: " << startState.size() << " Time: " << dTime << endl;
		dTime = 0;
	}

	if (result)
	{
		SaveStateToTxt();
		nd->SetTopmostNode(topmostBuffer);
		SaveStateToTxt("", ".\\data\\start\\");
	}

	AdvanceState(true);
}

void CheckBot::SetCurrentSize(int s)
{
	currentSize = s;
	startState.clear();

	for (int y = 0; y < s; y++)
	{
		startState.push_back(StartStateRow(s));
	}

	startState.shrink_to_fit();
}

void CheckBot::AdvanceState(bool funcDebug)
{
	do
	{
		for (int i = 0; i < startState.size() + 1; i++)
		{
			if (i == startState.size())
			{
				if (nd->GetDebugMode() && funcDebug)
				{
					cout << "Increased search size to: " << currentSize + 1 << endl;
				}
				SetCurrentSize(currentSize + 1);
				break;
			}

			if (!startState.at(startState.size() - i - 1).AdvanceState())
			{
				break;
			}
		}
	} while (!startState.at(startState.size() - 1).GetIndex());
}

void CheckBot::PutStateInNode()
{
	nd->ClearAllCells();
	for (int y = 0; y < startState.size(); y++)
	{
		for (int x = 0; x < startState.size(); x++)
		{
			if (startState.at(y).GetCell(x))
			{
				nd->AddCell(x, y);
			}
		}
	}
}



CheckBot::~CheckBot()
{
	delete nd;
}
