#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include "../SoundDevice.h"
#include "../SoundEffectsLibrary.h"
#include "../MusicBuffer.h"
#include "../SoundEffectsPlayer.h"

namespace TTD{
SoundDevice* sd = LISTENER->Get();
int walksound = SE_LOAD("../res/walksounds/stomp.ogg");
SoundEffectsPlayer walkplayer;
const constexpr int ROWS = 20, COLUMNS = 20;
char map[ROWS][COLUMNS];
bool mapHasChanged = true;  //process a map draw update
int prevPP = 0;
int PP = 0;         //PlayerPosition: MAX IS (ROWS*COLUMNS)-1
char Pdir = '>';    //character to represent facing direction
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
	if (Pdir != '^')
		mapHasChanged = true;

	Pdir = '^';
	PP -= COLUMNS; 
	if (PP < 0)  // off map?
	{
		PP += COLUMNS;  // move back, illegal move
	}
	{
		mapHasChanged = true;  // show updates
	}
}
void moveDown()
{
	if (Pdir != 'v')
		mapHasChanged = true;

	Pdir = 'v';
	if(PP >= (COLUMNS*ROWS)-COLUMNS)
		return; // on last row

	PP+=COLUMNS;
	mapHasChanged = true;  // show updates
}
void moveLeft()
{
	if (Pdir != '<')
		mapHasChanged = true;

	Pdir = '<';
	if(PP==0)
		return;
	if(PP%COLUMNS==0)
		return; // against left wall, do nothing

	PP -= 1;
	mapHasChanged = true;  // show updates
}
void moveRight()
{
	if (Pdir != '>')
		mapHasChanged = true;

	Pdir = '>';
	if(PP%COLUMNS==COLUMNS-1)
		return;

	PP += 1;
	mapHasChanged = true;  // show updates
}
void parseMovement(float dt)
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
void clearscreen()
{
	system("cls");
};
void showscene()
{
	if (mapHasChanged)
	{
		mapHasChanged = false;

		if (map[PP / COLUMNS][PP < COLUMNS ? PP : PP % COLUMNS] == '0')
			walkplayer.Play(walksound);


		clearscreen();
		map[prevPP / COLUMNS][prevPP < COLUMNS ? prevPP : prevPP % COLUMNS] = '.';  // restore old slot to blank
		map[PP/COLUMNS][PP<COLUMNS?PP:PP%COLUMNS] = Pdir; // update new slot
		prevPP = PP;  // keep track of current position for backtracking

		for (int row = 0; row < ROWS; row++)
		{
			std::cout << '\n';
			for (int col = 0; col < COLUMNS; col++) {
				std::cout << ' ' << map[row][col];
			}
		}

		std::cout << "\n\n\n MOVE: 'WASD'\n";

	}
};
}  // end namespace TTD