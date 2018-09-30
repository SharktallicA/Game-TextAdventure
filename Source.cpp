/*
	Khalid Ali 2018

	Notes:
		Based on a tutorial exercise from University module for Artificial Intelligence for Game Developers
		Map construction; the rooms (represented as Room objects) are linked to together in an up to four-way doubly-linked list of sorts
		Human play; played as a simple text adventure-esque fashion
*/

#include <iostream>
#include <string>
#include "Game.h"
#include "Costs.h"
#include "RoomChangeData.h"
#include "Room.h"

using namespace std;

int main(void)
{
	Game nGame;
	while (nGame.playing());
	return 0;
}