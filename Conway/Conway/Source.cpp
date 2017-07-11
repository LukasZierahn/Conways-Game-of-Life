#include "TokenMaster.h"
#include "NodeMaster.h"
#include "Node.h"
#include "Include.h"
#include "CheckBot.h"

int main(int argc, char* args[]) 
{

	bool quit = false;
	CheckBot bot = CheckBot();
	bot.SetCurrentSize(1);
	char inp;
	bool search = false;

	bot.GetNodeMaster()->AddCell(0, 0);
	bot.GetNodeMaster()->AddCell(1, 0);
	bot.GetNodeMaster()->AddCell(1, -2);
	bot.GetNodeMaster()->AddCell(3, -1);
	bot.GetNodeMaster()->AddCell(4, 0);
	bot.GetNodeMaster()->AddCell(5, 0);
	bot.GetNodeMaster()->AddCell(6, 0);

	//nd.AddCell(7, -10);
	//nd.AddCell(8, -10);
	//nd.AddCell(8, -12);
	//nd.AddCell(10, -11);
	//nd.AddCell(11, -10);
	//nd.AddCell(12, -10);
	//nd.AddCell(13, -10);

	//nd.AddCell(0, 0);
	//nd.AddCell(1, 0);
	//nd.AddCell(2, 0);
	//nd.AddCell(3, 0);
	//nd.AddCell(4, 0);
	//nd.AddCell(5, 0);
	//nd.AddCell(6, 0);	
	
	//nd.AddCell(0, 0);
	//nd.AddCell(0, 1);
	//nd.AddCell(0, 2);
	//nd.AddCell(0, 3);
	//nd.AddCell(0, 4);
	//nd.AddCell(0, 5);
	//nd.AddCell(0, 6);

	//nd.AddCell(10, -10);
	//nd.AddCell(10, -11);
	//nd.AddCell(10, -12);

	//nd.AddCell(2, 1);
	//nd.AddCell(4, 0);
	//nd.AddCell(4, 1);
	//nd.AddCell(5, 2);
	//nd.AddCell(5, 1);

	//nd.AddCell(-1, 0);
	//nd.AddCell(0, 0);
	//nd.AddCell(0, -1);
	////nd.AddCell(-1, -1);

	//nd.AddCell(-1, 5);
	//nd.AddCell(0, 5);
	//nd.AddCell(1, 5);

	while (!quit)
	{
		inp = cin.get();

		switch (inp)
		{
		case('q'):
			quit = true;
			break;

		case('t'):
			int turns;
			cin >> turns;

			long long timeBefore = clock();
			bot.GetNodeMaster()->Turn(turns);
			break;

		case('r'):
			cout << bot.GetNodeMaster()->GetOutputString() << endl;
			break;

		case('s'):
			int x, y, length;
			bot.GetNodeMaster()->ClearAllCells();

			cin >> length;

			for (int i = 0; i < length; i++)
			{
				cin >> x >> y;
				bot.GetNodeMaster()->AddCell(x, y);
			}
			break;

		case('d'):
			int db = bot.GetNodeMaster()->GetDebugMode();

			db++;
			if (db == 4)
			{
				db = 0;
			}

			bot.GetNodeMaster()->SetDebugMode(db);
			cout << "Debug Mode is: " << db << endl;
			break;

		case('l'):
		{
			string livingCells = bot.GetNodeMaster()->GetOutputString();
			int living = -1;
			for (char c : livingCells)
			{
				if (c == '\n')
				{
					living++;
				}
			}
			cout << "Alive: " << living << endl;
			break;
		}

		case('y'):
			cout << bot.SaveStateToTxt() << endl;
			break;

		case('x'):
		{
			string buffer;
			cin >> buffer;
			bot.LoadStateFromTxt(buffer);
			break;
		}

		case('a'):
		{
			inp = cin.get();

			int tries = -1;
			if (inp == 'i')
			{
				cin >> tries;
				for (int i = 0; i < tries; i++)
				{
					bot.SearchForAnomalies();
				}
			}
			else if (inp == 's')
			{
				int s;
				cin >> s;
				int realS = pow(2, s * s);

				bot.SetCurrentSize(s);

				if (bot.GetNodeMaster()->GetDebugMode())
				{
					cout << "going out to: " << realS << " wish me luck!" << endl;
				}

				for (int i = 0; i < realS; i++)
				{
					bot.SearchForAnomalies();
				}
			}

			if (bot.GetNodeMaster()->GetDebugMode())
			{
				cout << "done!" << endl;
			}
			break;
		}

		case('c'):
		{
			bot.SetCurrentSize(0);
			int pos;
			cin >> pos;
			for (int i = 0; i < pos; i++)
			{
				bot.AdvanceState();
			}
			bot.PutStateInNode();
			break;
		}

		case('j'):
		{
			int buf = 0;
			inp = cin.get();
			if (inp == 'b')
			{
				inp = cin.get();
				if (inp == 'a')
				{
					cin >> buf;
					bot.GetTokenMaster()->AddBirths(buf);
				}
				else if (inp == 'r')
				{
					cin >> buf;
					bot.GetTokenMaster()->PopBirths(buf);
				}
			}
			else if (inp == 's')
			{
				inp = cin.get();
				if (inp == 'a')
				{
					cin >> buf;
					bot.GetTokenMaster()->AddStays(buf);
				}
				else if (inp == 'r')
				{
					cin >> buf;
					bot.GetTokenMaster()->PopStays(buf);
				}
			}
			else if (inp == 'a')
			{
				bot.GetTokenMaster()->ResetRule();
			}
			else if (inp == 'p')
			{
				cout << bot.GetNodeMaster()->GetTokenMaster()->GetCurrentRule() << endl;
			}
			bot.GetNodeMaster()->ResetHash();
			break;
		}

		default:
			break;
		}
	}
	return 0;
}