#pragma once
#include "Utility.h"
#include "Player.h"
#include "Costs.h"
#include "PlaceChangeData.h"
#include "Place.h"
#include <fstream>
#include <vector>
#include <list>

using namespace std;

//Encapsulates all game functionality
class Game
{
private:
	Player* player;

	//Vector reference to all places
	vector<Place*> places;
	//Current place the player is in
	Place* currentPlace;
	//Temporary place data for when needed
	PlaceChangeData* tempPlace = nullptr;

	//Name of the loaded game
	string gameName;
	//Name of the travel method the player uses
	string travelMethod;
	//Unit of distance the player travels at
	string distanceUnit;
	//Flags whether area map is enabled for the loaded game
	bool enableAreaMap;

	//Loads and returns the raw place data from the "game.txt" file
	vector<vector<string>> loadPlaces(void)
	{
		ifstream ifPlaces("game.txt");
		if (!ifPlaces) throw "CRITICAL ERROR: game.txt data file is not present!\n";

		vector<vector<string>> rawPlaces;
		string line;

		while (getline(ifPlaces, line))
		{
			vector<string> delimited;
			stringstream pStream(line);

			if (line[0] == ':')
			{
				while (pStream.good())
				{
					string sub;
					getline(pStream, sub, ',');
					delimited.push_back(sub);
				}
				Utility::setWindowTitle(gameName = delimited[0].substr(1, line.length())); //read the first metadata line for the game's name
				travelMethod = delimited[1];
				distanceUnit = delimited[2];
				if (delimited[3][0] == 'Y' || delimited[3][0] == 'y') enableAreaMap = true;
				else if (delimited[3][0] == 'n' || delimited[3][0] == 'n') enableAreaMap = false;
			}
			else if (line[0] != '#' && line[0] != ':' && line[0] != ';') //prevent operation for any comment or metadata lines within the text file
			{
				while (pStream.good())
				{
					string sub;
					getline(pStream, sub, ',');
					delimited.push_back(sub);
				}
				rawPlaces.push_back(delimited);
			}
		}
		return rawPlaces;
	}

	//Constructs the game's place map as doubly-linked list of place objects
	void assemblePlaces(void)
	{
		// 1) Attempts to call for the loading of the places data text file
		// and get the result returned for processing

		vector<vector<string>> rawPlaces;
		try { rawPlaces = loadPlaces(); }
		catch (char* msg)
		{
			cout << msg << endl;
			throw;
		}

		// 2) Constructs a vector of Place objects from the raw data, using
		// the place name and travel costs only at this point

		for (vector<string> rawPlace : rawPlaces) places.push_back(new Place(rawPlace[0], new Costs(stoi(rawPlace[5]), stoi(rawPlace[6]), stoi(rawPlace[7]), stoi(rawPlace[8]))));
		
		// 3) Maps out the relative (north, east, south, west) adjacent places
		// for each Place object; for each Place object in places vector, use the raw
		// place data to find out the names of its neighbours and them link
		// their place objects into the current one (and repeat) until (an up to
		// four-way) doubly-linked list is formed

		for (int i = 0; i < places.size(); i++)
		{
			vector<Place*> relativePlaces;

			//find and get the Place object adjacent to the current one
			for (int j = 1; j <= 4; j++)
			{
				if (rawPlaces[i][j] != "-") //work out the relative Place if one is specified in the raw data
				{
					for (Place* place : places) if (place->GetName() == rawPlaces[i][j]) relativePlaces.push_back(place);
				}
				else relativePlaces.push_back(nullptr); //nullptr to represent blank wall
			}

			//call for the mapping of the places to form the linked bonds
			places[i]->MapPlaces(relativePlaces[0], relativePlaces[1], relativePlaces[2], relativePlaces[3]);
		}
		
		// 4) Assign starting Place

		currentPlace = places[0];
	}

	//Inteprets player input and resolves it to a short-hand single letter command
	string getPlayerInput(void)
	{
		//get command
		string input = Utility::getString("CMD: ");

		//intepret command
		if (input == "n" || input == "N" || input == "north" || input == "North" || input == "go north" || input == "Go North") return "n";
		else if (input == "e" || input == "E" || input == "east" || input == "East" || input == "go east" || input == "Go East") return  "e";
		else if (input == "s" || input == "S" || input == "south" || input == "South" || input == "go south" || input == "Go South") return  "s";
		else if (input == "w" || input == "W" || input == "west" || input == "West" || input == "go west" || input == "Go West") return  "w";
		else if ((input == "m" || input == "M" || input == "map" || input == "Map" || input == "get map" || input == "Get Map") && enableAreaMap) return  "m";
		else if (input == "h" || input == "H" || input == "help" || input == "Help" || input == "c" || input == "C" || input == "commands" || input == "Commands") return  "h";
		else if (input == "exit" || input == "Exit" || input == "q" || input == "quit" || input == "Quit" || input == "kill" || input == "Kill") return  "q";
		else return "z";
	}

public:
	Game(void)
	{
		try { assemblePlaces(); }
		catch(...)
		{
			cout << "Program cannot continue.\n";
			Utility::pause();
			throw;
		}
		player = new Player();
	}
	~Game(void) { delete currentPlace, tempPlace, player; }

	void Play(void)
	{
		while (true)
		{
			//reset console colour
			Utility::setColour(WHITE);
			//probe any update methods
			currentPlace->Update();

			cout << "You're at " << currentPlace->GetName() << "!" << endl;
			string cmd = getPlayerInput();

			if (cmd == "q") break;
			else if (cmd == "z")
			{
				Utility::setColour(RED);
				cout << "Invalid command!" << endl << endl;
			}
			else if (cmd == "h" && enableAreaMap)
			{
				Utility::setColour(YELLOW);
				cout << "Command list: (n) go north (e) go east (s) go south (w) go west (m) show area map (q) quit" << endl << endl;
			}
			else if (cmd == "h" && !enableAreaMap)
			{
				Utility::setColour(YELLOW);
				cout << "Command list: (n) go north (e) go east (s) go south (w) go west (q) quit" << endl << endl;
			}
			else if (cmd == "m")
			{
				Utility::setColour(LIGHT_BLUE);
				cout << currentPlace->GetNeighbours() << endl << endl;
			}
			else
			{
				if (cmd == "n") tempPlace = currentPlace->GoNorth();
				else if (cmd == "e") tempPlace = currentPlace->GoEast();
				else if (cmd == "s") tempPlace = currentPlace->GoSouth();
				else if (cmd == "w") tempPlace = currentPlace->GoWest();

				if (tempPlace->nextPlace == currentPlace)
				{
					Utility::setColour(LIGHT_PURPLE);
					cout << "There is no place in that direction!" << endl << endl;
				}
				else
				{
					Utility::setColour(LIGHT_GREEN);
					currentPlace = tempPlace->nextPlace;
					cout << "You " << travelMethod << "ed " << tempPlace->costOfTravel << " " << distanceUnit << "s!" << endl << endl;
					tempPlace = nullptr;
				}
			}
		}
	}

	void breath(string start = "")
	{
	}

	void depth(string start = "")
	{
	}
};