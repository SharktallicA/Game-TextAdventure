#pragma once
#include "Utility.h"
#include "Costs.h"
#include "RoomChangeData.h"
#include "Room.h"
#include <fstream>
#include <vector>

using namespace std;

//Encapsulates all game functionality
class Game
{
private:
	//Current room the player is in
	Room* currentRoom;
	//Temporary room data for when needed
	RoomChangeData* tempRoom = nullptr;

	//Loads and returns the raw room data from the "rooms.txt" file
	vector<vector<string>> loadRooms(void)
	{
		ifstream ifRooms("rooms.txt");
		if (!ifRooms) throw "CRITICAL ERROR: ""rooms.txt"" data file is not present\n!";

		vector<vector<string>> rawRooms;
		string line;

		while (getline(ifRooms, line))
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
				rawRooms.push_back(delimited);
			}
		}
		return rawRooms;
	}

	//Constructs the game's map as doubly-linked list of Room objects
	void assembleRooms(void)
	{
		// 1) Attempts to call for the loading of the rooms data text file
		// and get the result returned for processing

		vector<vector<string>> raw;
		try { raw = loadRooms(); }
		catch (char* msg)
		{
			cout << msg << endl;
			throw;
		}

		// 2) Constructs a vector of Room objects from the raw data, using
		// the room name and travel costs only at this point

		vector<Room*> rooms;
		for (vector<string> rawRoom : raw) rooms.push_back(new Room(rawRoom[0], new Costs(stoi(rawRoom[5]), stoi(rawRoom[6]), stoi(rawRoom[7]), stoi(rawRoom[8]))));
		
		// 3) Maps out the relative (north, east, south, west) adjacent rooms
		// for each Room object; for each Room object in rooms vector, use the raw
		// room data to find out the names of its neighbours and them link
		// their Room objects into the current one (and repeat) until (an up to
		// four-way) doubly-linked list is formed

		for (int i = 0; i < rooms.size(); i++)
		{
			vector<Room*> relativeRooms;

			//find and get the Room object adjacent to the current one
			for (int j = 1; j <= 4; j++)
			{
				if (raw[i][j] != "-") //work out the relative Room if one is specified in the raw data
				{
					for (Room* room : rooms) if (room->GetName() == raw[i][j]) relativeRooms.push_back(room);
				}
				else relativeRooms.push_back(nullptr); //nullptr to represent blank wall
			}

			//call for the mapping of the rooms to form the linked bonds
			rooms[i]->MapRooms(relativeRooms[0], relativeRooms[1], relativeRooms[2], relativeRooms[3]);
		}

		// 4) Randomly select starting Room object

		currentRoom = rooms[Utility::generateNumber32(0, rooms.size())];
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
	Game(void) { assembleRooms(); }
	~Game(void) { delete currentRoom, tempRoom; }

	void play(void)
	{
		while (true)
		{
			cout << "You're in the " << currentRoom->GetName() << "!" << endl;
			string cmd = GetHumanCommand();

			if (cmd == "q") break;
			else if (cmd == "i") cout << "Invalid command!" << endl << endl;
			else if (cmd == "h") cout << "Command list: (n) go north, (e) go east, (s) go south, (w) go west, (q) quit" << endl << endl;
			else
			{
				if (cmd == "n") tempRoom = currentRoom->GoNorth();
				else if (cmd == "e") tempRoom = currentRoom->GoEast();
				else if (cmd == "s") tempRoom = currentRoom->GoSouth();
				else if (cmd == "w") tempRoom = currentRoom->GoWest();

				if (tempRoom->nextRoom == currentRoom) cout << "There is no room in that direction!" << endl << endl;
				else
				{
					currentRoom = tempRoom->nextRoom;
					cout << "You spent " << tempRoom->costOfTravel << " seconds walking!" << endl << endl;
					tempRoom = nullptr;
				}
			}
		}
	}
};