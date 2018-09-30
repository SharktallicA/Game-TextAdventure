#pragma once
#include "Utility.h"
#include "Costs.h"
#include "PlaceChangeData.h"
#include "Place.h"
#include <fstream>
#include <vector>

using namespace std;

//Encapsulates all game functionality
class Game
{
private:
	//Current place the player is in
	Place* currentPlace;
	//Temporary place data for when needed
	PlaceChangeData* tempPlace = nullptr;

	//Loads and returns the raw place data from the "places.txt" file
	vector<vector<string>> loadPlaces(void)
	{
		ifstream ifPlaces("places.txt");
		if (!ifPlaces) throw "CRITICAL ERROR: places.txt data file is not present!\n";

		vector<vector<string>> rawPlaces;
		string line;

		while (getline(ifPlaces, line))
		{
			vector<string> delimited;
			stringstream pStream(line);

			if (line[0] != '#') //prevent operation for any comment lines within the text file
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

		vector<vector<string>> raw;
		try { raw = loadPlaces(); }
		catch (char* msg)
		{
			cout << msg << endl;
			throw;
		}

		// 2) Constructs a vector of Place objects from the raw data, using
		// the place name and travel costs only at this point

		vector<Place*> places;
		for (vector<string> rawPlace : raw) places.push_back(new Place(rawPlace[0], new Costs(stoi(rawPlace[5]), stoi(rawPlace[6]), stoi(rawPlace[7]), stoi(rawPlace[8]))));
		
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
				if (raw[i][j] != "-") //work out the relative Place if one is specified in the raw data
				{
					for (Place* place : places) if (place->GetName() == raw[i][j]) relativePlaces.push_back(place);
				}
				else relativePlaces.push_back(nullptr); //nullptr to represent blank wall
			}

			//call for the mapping of the places to form the linked bonds
			places[i]->MapPlaces(relativePlaces[0], relativePlaces[1], relativePlaces[2], relativePlaces[3]);
		}

		// 4) Randomly select starting Place object

		currentPlace = places[Utility::generateNumber32(0, places.size())];
	}

	//Inteprets human input and resolves it to a short-hand single letter command
	string GetHumanCommand(void)
	{
		//get command
		string input = Utility::getString("CMD: ");

		//intepret command
		if (input == "n" || input == "N" || input == "north" || input == "North") return "n";
		else if (input == "e" || input == "E" || input == "east" || input == "East") return  "e";
		else if (input == "s" || input == "S" || input == "south" || input == "South") return  "s";
		else if (input == "w" || input == "W" || input == "west" || input == "West") return  "w";
		else if (input == "h" || input == "H" || input == "help" || input == "Help" || input == "c" || input == "C" || input == "commands" || input == "Commands") return  "h";
		else if (input == "exit" || input == "Exit" || input == "q" || input == "quit" || input == "Quit" || input == "kill" || input == "Kill") return  "q";
		else return "i";
	}

public:
	Game(void) { assemblePlaces(); }
	~Game(void) { delete currentPlace, tempPlace; }

	void play(void)
	{
		while (true)
		{
			cout << "You're in the " << currentPlace->GetName() << "!" << endl;
			string cmd = GetHumanCommand();

			if (cmd == "q") break;
			else if (cmd == "i") cout << "Invalid command!" << endl << endl;
			else if (cmd == "h") cout << "Command list: (n) go north, (e) go east, (s) go south, (w) go west, (q) quit" << endl << endl;
			else
			{
				if (cmd == "n") tempPlace = currentPlace->GoNorth();
				else if (cmd == "e") tempPlace = currentPlace->GoEast();
				else if (cmd == "s") tempPlace = currentPlace->GoSouth();
				else if (cmd == "w") tempPlace = currentPlace->GoWest();

				if (tempPlace->nextPlace == currentPlace) cout << "There is no place in that direction!" << endl << endl;
				else
				{
					currentPlace = tempPlace->nextPlace;
					cout << "You spent " << tempPlace->costOfTravel << " seconds walking!" << endl << endl;
					tempPlace = nullptr;
				}
			}
		}
	}
};