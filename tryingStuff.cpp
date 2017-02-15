#include <iostream>
#include <ctime>
#include <Windows.h>
#include "GameMap.h"
//#include "WORLD.h"

using namespace std;

int posx = 500; int posy = 500;

int main()
{

	//srand(time(0));

	//seed for map generator
	int seed = 683575564;

	seed = rand() % 10000000;

	

	srand(seed);
	//temporary environent to see the map in action! 

	MYRECT window;
	window.tall = 80;
	window.wide = 80;

	HWND console = GetConsoleWindow();
	MoveWindow(console, 0, 0, 1800, 1000, TRUE);

	string name = "";

	cout << "This is the Map generator test environment" << endl;
	cout << "Created by Mitchell Marsh..." << endl;
	cout << "Please enter the name of the map to generate: ";
	cin >> name;
	cout << endl << endl << endl;
	
	GameMap myGame;

	myGame.CreateNewMap(name);

	//manage player position! 
	//pass player position into display funciton
	// have an input collector to look for WASD or arrows 
	//display each positions contents to the screen next to it

	//display the current position

	bool quit = false;
	char in;
	while (!quit)
	{
		system("CLS");
		myGame.displayMap(posx, posy, window);
		cout << "Map Position: "; myGame.displayWorldCoords();  // only for debug

		cin >> in;
		if (in == 'w')
		{
			posy = posy - 5;
		}
		else if (in == 's')
		{
			posy = posy + 5;
		}
		else if (in == 'a')
		{
			posx = posx - 5;
		}
		else if (in == 'd')
		{
			posx = posx + 5;
		}
		else if (in == 'q' || in == 'Q')
		{
			quit = true;
		}
		
	}


	
	cout << "Press Enter to End" << endl;
	cin.ignore(256, '\n');
	return 0;
}

/*








*/