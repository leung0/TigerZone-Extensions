
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Game.h"
#include <string>
#pragma comment(lib, "ws2_32.lib")
#include <Winsock2.h>

vector<string> split(const char *str, char c = ' ')
{
	vector<string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(string(begin, str));
	} while (0 != *str++);

	return result;
}

int main()
{
	string tp = "TIGERZONE";
	string user = "TEAMV";
	string pass = "IAMV";
	string pid = "";
	string g1id = "";
	string g2id = "";
	bool game1 = false;
	bool game2 = false;
	string output;

	//Start winsock
	WSADATA wsaData;
	WORD Dllversion = MAKEWORD(2, 1);

	if (WSAStartup(Dllversion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("128.227.248.254");
	addr.sin_port = htons(4444);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0)
	{
		MessageBoxA(NULL, "Failed to connect", "error", MB_OK | MB_ICONERROR);
		return 0;
	}
	cout << "connected" << endl;
	cout << "client" << endl;
	cout << endl;
	char yoo[256];
	recv(Connection, yoo, sizeof(yoo), NULL);
	cout << yoo << endl;

	string question = "JOIN " + tp;
	char s[256];
	strcpy_s(s, question.c_str());;
	send(Connection, s, sizeof(s), NULL);
	cout << "JOIN " << tp << endl;

	yoo[256];
	recv(Connection, yoo, sizeof(yoo), NULL);
	cout << yoo << endl;

	question = "I AM " + user + " " + pass;
	s[256];
	strcpy_s(s, question.c_str());;
	send(Connection, s, sizeof(s), NULL);
	cout << "I AM " << user << " " << pass << endl;

	yoo[256];
	recv(Connection, yoo, sizeof(yoo), NULL);
	cout << yoo << endl;

	vector <string> scan = split(yoo, ' ');
	pid = scan[1];

	Game* ga;
	Player* p1a;
	Player* p2a;
	Board* ba;
	Game* gb;
	Player* p1b;
	Player* p2b;
	Board* bb;

	while (Connection)
	{
		yoo[256];
		recv(Connection, yoo, sizeof(yoo), NULL);
		cout << yoo << endl;

		vector <string> scan = split(yoo, ' ');

		if (scan[0] == "NEW")
		{
			//create boards and stuff
			cout << "game start" << endl;
			ga = new Game();
			p1a = ga->getPlayer1();
			p2a = ga->getPlayer2();
			ba = ga->getBoard();
			gb = new Game();
			p1b = gb->getPlayer1();
			p2b = gb->getPlayer2();
			bb = gb->getBoard();
			game1 = true;
			game2 = true;
			g1id = "";
			g2id = "";
		}
		while (game1)
		{
			yoo[256];
			recv(Connection, yoo, sizeof(yoo), NULL);
			cout << yoo << endl;
			scan = split(yoo, ' ');
			cout << scan[0] << endl;
			if (scan[6] == "FORFEITED") {
				break;
			}
			else if (scan[0] == "STARTING")
			{
				//STARTING TILE IS <tile> AT <x> <y> <orientation>
				Tile* firstTile = new Tile(scan[3]);
				firstTile->setPosition(0, 0);
				ba->addTile(firstTile);
				bb->addTile(firstTile);
				cout << endl << "starting tile placed" << endl << endl;
			}
			else if (scan[0] == "MAKE")
			{
				if (g1id == "")
				{
					g1id = scan[5];
				}
				if (g1id == scan[5])
				{
					//MAKE YOUR MOVE IN GAME <gid> WITHIN <timemove> SECOND: MOVE <#> PLACE <tile>
					cout << "Our turn" << endl;
					Tile* turnTile = new Tile(scan[12]);
					p1a->setCurrentTile(turnTile);
					string move = p1a->makeSmarterMove();
					p1a->getTigersBack();
					p2a->setCurrentTile(turnTile);
					p2a->getTigersBack();
					//GAME <gid> MOVE <#> PLACE <tile> AT <x> <y> <orientation> NONE

					output = "GAME " + scan[5] + " MOVE " + scan[10] + " " + move;
					s[256];
					strcpy_s(s, output.c_str());;
					send(Connection, s, sizeof(s), NULL);
				}
				else
				{
					//MAKE YOUR MOVE IN GAME <gid> WITHIN <timemove> SECOND: MOVE <#> PLACE <tile>
					cout << "Our turn" << endl;
					Tile* turnTile = new Tile(scan[12]);
					p1b->setCurrentTile(turnTile);
					string move = p1b->makeSmarterMove();
					p1b->getTigersBack();
					p2b->setCurrentTile(turnTile);
					p2b->getTigersBack();
					//GAME <gid> MOVE <#> PLACE <tile> AT <x> <y> <orientation> NONE

					output = "GAME " + scan[5] + " MOVE " + scan[10] + " " + move;
					s[256];
					strcpy_s(s, output.c_str());;
					send(Connection, s, sizeof(s), NULL);
				}
			}
			else if (scan[5] != pid)
			{
				if (g2id == "")
				{
					g2id = scan[1];
				}
				if(g2id == scan[1])
				{
					//their turn
					//GAME <gid> MOVE <#> PLAYER <pid> PLACED <tile> AT <x> <y> <orientation> NONE
					cout << "Their turn" << endl;
					Tile* oppTile = new Tile(scan[7]);
					p2b->setCurrentTile(oppTile);
					int x = stoi(scan[9]);
					int y = stoi(scan[10]);
					int or = stoi(scan[11]);

					cout << x << endl;
					cout << y << endl;
					cout << or << endl;

					cout << yoo << endl;
					p2b->placeTile(make_pair(make_pair(x, y), or ));
					p1b->getTigersBack();
					p1b->setCurrentTile(oppTile);
					if (scan[12] == "TIGER")
					{
						p2b->placeTiger(stoi(scan[13]));
					}
					else if (scan[12] == "CROCODILE") {
						p2b->placeCrocodile();
					}
				}
				else
				{
					//their turn
					//GAME <gid> MOVE <#> PLAYER <pid> PLACED <tile> AT <x> <y> <orientation> NONE
					cout << "Their turn" << endl;
					Tile* oppTile = new Tile(scan[7]);
					p2a->setCurrentTile(oppTile);
					int x = stoi(scan[9]);
					int y = stoi(scan[10]);
					int or = stoi(scan[11]);

					cout << x << endl;
					cout << y << endl;
					cout << or << endl;

					cout << yoo << endl;
					p2a->placeTile(make_pair(make_pair(x, y), or ));
					p1a->setCurrentTile(oppTile);
					p1a->getTigersBack();
					if (scan[12] == "TIGER")
					{
						p2a->placeTiger(stoi(scan[13]));
					}
					else if (scan[12] == "CROCODILE") {
						p2a->placeCrocodile();
					}
				}
				
			}
			else if (scan[2] == "CHALLENGES")
			{
				game1 = false;
			}
			else if (scan[4] == "GAME")
			{
				if (game1 != g1id)
					cout << "FORFEITED: DOES NOT KNOW OUTCOME" << endl;
				else if (game2 != g2id)
					cout << "FORFEITED: DOES NOT KNOW OUTCOME" << endl;
			}
		}

	}


	system("pause");



	return 0;
}
