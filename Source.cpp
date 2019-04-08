
0 /*
	Game-TextAdventure
	Khalid Ali 2018
	http://khalidali.co.uk/

	Notes:
		Based on a tutorial exercise from University module "Artificial Intelligence for Game Developers"
*/

#include "Game.h"

using namespace std;

int main(void)
{
	Utility::setWindowTitle("Game-TextAdventure");
	Utility::setColour(WHITE, BLACK);

	try
	{
		Game nGame;
		nGame.Play();
	}
	catch (...) { return 1; }
	return 0;
}