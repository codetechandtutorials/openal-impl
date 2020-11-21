#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include "../SoundDevice.h"
#include "../SoundEffectsLibrary.h"
#include "../MusicBuffer.h"
#include "../SoundEffectsPlayer.h"

namespace TTD {
SoundDevice* sd = LISTENER->Get();
int walksound = SE_LOAD("../res/walksounds/stomp.ogg");
int aiwalksound = SE_LOAD("../res/walksounds/Fantozzi-SandL1.ogg");
SoundEffectsPlayer walkplayer;
SoundEffectsPlayer aiwalkplayer;
const constexpr int ROWS = 20, COLUMNS = 20;
char map[ROWS][COLUMNS];
bool mapHasChanged = true;  //process a map draw update
int prevPP = 0;
int PP = 0;         //PlayerPosition: MAX IS (ROWS*COLUMNS)-1
char Pdir = '>';    //character to represent facing direction
bool playerHasMoved = false;
int prevAIP = COLUMNS * ROWS - 1;  // AI opponent starting location is the last slot.
int AIP = COLUMNS * ROWS - 1;  // AI opponent starting location is the last slot.
bool AIHasMoved = false;
void defaultMap()
{
	for (int x = 0; x < ROWS; x++)  //rows
	{
		for (int y = 0; y < COLUMNS; y++)  //columns
		{
			map[x][y] = '0';
		}
	}
}
void moveUp()
{
	if (Pdir != '^') {
		mapHasChanged = true;
		playerHasMoved = true;
	}

	Pdir = '^';
	PP -= COLUMNS;
	if (PP < 0)  // off map?
	{
		PP += COLUMNS;  // move back, illegal move
	}
	{
		mapHasChanged = true;  // show updates
		playerHasMoved = true;
	}
}
void moveDown()
{
	if (Pdir != 'v') {
		mapHasChanged = true;
		playerHasMoved = true;
	}
	Pdir = 'v';
	if (PP >= (COLUMNS * ROWS) - COLUMNS)
		return; // on last row

	PP += COLUMNS;
	mapHasChanged = true;  // show updates
	playerHasMoved = true;
}
void moveLeft()
{
	if (Pdir != '<') {
		mapHasChanged = true;
		playerHasMoved = true;
	}

	Pdir = '<';
	if (PP == 0)
		return;
	if (PP % COLUMNS == 0)
		return; // against left wall, do nothing

	PP -= 1;
	mapHasChanged = true;  // show updates
	playerHasMoved = true;
}
void moveRight()
{
	if (Pdir != '>') {
		mapHasChanged = true;
		playerHasMoved = true;
	}

	Pdir = '>';
	if (PP % COLUMNS == COLUMNS - 1)
		return;

	PP += 1;
	mapHasChanged = true;  // show updates
	playerHasMoved = true;  // show updates
}
void parseUserInput(float dt)
{
	static const float MOVECD = .1677f;
	static float movetimeout = MOVECD;
	movetimeout += dt;
	if ((GetAsyncKeyState('W') & 0x8000) && movetimeout > MOVECD)
	{
		moveUp();
		movetimeout = 0;
	}
	if ((GetAsyncKeyState('S') & 0x8000) && movetimeout > MOVECD)
	{
		moveDown();
		movetimeout = 0;
	}
	if ((GetAsyncKeyState('A') & 0x8000) && movetimeout > MOVECD)
	{
		moveLeft();
		movetimeout = 0;
	}
	if ((GetAsyncKeyState('D') & 0x8000) && movetimeout > MOVECD)
	{
		moveRight();
		movetimeout = 0;
	}
}
void updateAI(float dt) {
	static const float AISPEED = .2667f;
	static float AITIMEOUT = 0;
	AITIMEOUT += dt;
	if (AITIMEOUT > AISPEED) {
		AITIMEOUT = 0;
		int cpos = 0;
		if (AIP < COLUMNS) // IN TOP ROW DON'T CHECK UP
		{
		}
		else  // NOT IN TOP ROW CHECK UP
		{
			cpos = AIP - COLUMNS;
			if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
			{
				AIP = cpos;
				mapHasChanged = true;
				AIHasMoved = true;
				return;
			}
		}

		if (AIP == 0 || AIP % COLUMNS == 0)  // AGAINST LEFT WALL DON'T CHECK LEFT
		{
		}
		else  // NOT AGAINST LEFT WALL
		{
			cpos = AIP - 1;
			if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
			{
				AIP = cpos;
				mapHasChanged = true;
				AIHasMoved = true;
				return;
			}
		}


		if (AIP % COLUMNS == COLUMNS - 1)  // agianst right wall dont che3ck right
		{
		}
		else // not against right wall
		{
			cpos = AIP + 1;
			if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
			{
				AIP = cpos;
				mapHasChanged = true;
				AIHasMoved = true;
				return;
			}
		}

		if (AIP / COLUMNS == ROWS)  // on last row dont check down
		{

		}
		else // not on last row
		{
			cpos = AIP + COLUMNS;
			if (map[cpos / COLUMNS][cpos < COLUMNS ? cpos : cpos % COLUMNS] == '0')
			{
				AIP = cpos;
				mapHasChanged = true;
				AIHasMoved = true;
				return;
			}
		}

		//fail case, no 0's around
		if (AIP < COLUMNS) // IN TOP ROW DON'T CHECK UP
		{
		}
		else  // NOT IN TOP ROW CHECK UP
		{
			cpos = AIP - COLUMNS;
			AIP = cpos;
			mapHasChanged = true;
			AIHasMoved = true;
			return;
		}

		if (AIP == 0 || AIP % COLUMNS == 0)  // AGAINST LEFT WALL DON'T CHECK LEFT
		{
		}
		else  // NOT AGAINST LEFT WALL
		{
			cpos = AIP - 1;
			AIP = cpos;
			mapHasChanged = true;
			AIHasMoved = true;
			return;
		}


		if (AIP % COLUMNS == COLUMNS - 1)  // agianst right wall dont che3ck right
		{
		}
		else // not against right wall
		{
			cpos = AIP + 1;
			AIP = cpos;
			mapHasChanged = true;
			AIHasMoved = true;
			return;
		}

		if (AIP / COLUMNS == ROWS)  // on last row dont check down
		{

		}
		else // not on last row
		{
			cpos = AIP + COLUMNS;
			AIP = cpos;
			mapHasChanged = true;
			AIHasMoved = true;
			return;
		}

	}
}
void clearscreen()
{
	system("cls");
};
void showscene()
{
	if (mapHasChanged)
	{
		mapHasChanged = false;

		if (playerHasMoved) {
			playerHasMoved = false;
			if (map[PP / COLUMNS][PP < COLUMNS ? PP : PP % COLUMNS] == '0')
				walkplayer.Play(walksound);
			map[prevPP / COLUMNS][prevPP < COLUMNS ? prevPP : prevPP % COLUMNS] = '.';  // restore old slot to blank
			map[PP / COLUMNS][PP < COLUMNS ? PP : PP % COLUMNS] = Pdir; // update new slot
			prevPP = PP;  // keep track of current position for backtracking
		}
		if (AIHasMoved)
		{
			AIHasMoved = false;
			if (map[AIP / COLUMNS][AIP < COLUMNS ? AIP : AIP % COLUMNS] == '0')
				aiwalkplayer.Play(aiwalksound);
			map[prevAIP / COLUMNS][prevAIP < COLUMNS ? prevAIP : prevAIP % COLUMNS] = '.';  // restore old slot to blank
			map[AIP / COLUMNS][AIP < COLUMNS ? AIP : AIP % COLUMNS] = 'B'; // update new slot
			prevAIP = AIP;  // keep track of current position for backtracking
		}
		clearscreen();
		std::cout << ' ';
		for (int col = 0; col < COLUMNS; col++)
			std::cout << ' ' << '_';

		for (int row = 0; row < ROWS; row++)
		{
			std::cout << '\n';
			for (int col = 0; col < COLUMNS; col++) {
				if (col == 0) std::cout << '|';
				std::cout << ' ' << map[row][col];
				if (col == COLUMNS - 1) std::cout << '|';
			}
		}

		std::cout << '\n' << ' ';
		for (int col = 0; col < COLUMNS; col++)
			std::cout << ' ' << '_';

		std::cout << "\n\n\n MOVE: 'WASD'\n";

	}
};
}  // end namespace TTD