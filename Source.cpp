/*
	"Tunnels of Terror" class exercise implementation
	Khalid Ali 2018

	Notes:
		Based on a tutorial exercise from University module for Artificial Intelligence for Game Developers
		Map construction; the rooms (represented as Room objects) are linked to together in an up to four-way doubly-linked list of sorts
		Human play; played as a simple text adventure-esque fashion
*/

#include <iostream>
#include <string>

using namespace std;

//Contains direction travel costs for a room
class Costs
{
public:
	int n;
	int e;
	int s;
	int w;

	Costs(int nNorth = 0, int nEast = 0, int nSouth = 0, int nWest = 0)
	{
		n = nNorth;
		e = nEast;
		s = nSouth;
		w = nWest;
	}
};

class Room;

//Contains change in room data
class RoomChangeData
{
public:
	Room* nextRoom = nullptr;
	int costOfTravel;

	RoomChangeData(Room* nRoom, int nTravel) { nextRoom = nRoom; costOfTravel = nTravel; }
};

//Contains data that represents a room and its relative neighbours
class Room
{
private:
	string name = "";

	//linked list hardpoints
	Room* north = nullptr;
	Room* east = nullptr;
	Room* south = nullptr;
	Room* west = nullptr;

	Costs* costs;

public:
	Room(string nName, Costs* nCosts) { name = nName; costs = nCosts; }
	~Room() { delete north, east, south, west, costs; }

	//Defines the layout of rooms relative to this one
	void MapRooms(Room* nNorth = nullptr, Room* nEast = nullptr, Room* nSouth = nullptr, Room* nWest = nullptr)
	{
		if (north || east || south || west)
		{
			cout << "ILLEGAL CALL: room already mapped!" << endl;
			return;
		}

		north = nNorth;
		east = nEast;
		south = nSouth;
		west = nWest;
	}

	//Returns this room's name
	string GetName() { return name; }

	RoomChangeData* GoNorth()
	{
		if (!north) return new RoomChangeData(this, 0);
		return new RoomChangeData(north, costs->n);
	}
	RoomChangeData* GoEast()
	{
		if (!east) return new RoomChangeData(this, 0);
		return new RoomChangeData(east, costs->e);
	}
	RoomChangeData* GoSouth()
	{
		if (!south) return new RoomChangeData(this, 0);
		return new RoomChangeData(south, costs->s);
	}
	RoomChangeData* GoWest()
	{
		if (!west) return new RoomChangeData(this, 0);
		return new RoomChangeData(west, costs->w);
	}
};

//Assembles the maps
Room* AssembleRooms(void)
{
	// 1) Construct all possible rooms as separate Room objects with a pre-given name
	// and a Costs object with (north, east, south, west) direction travel costs.

	Room* library = new Room("Library", new Costs(0, 3, 1));
	Room* mysteriousRoom = new Room("Mysterious Room", new Costs(3, 0, 4, 3));
	Room* trollsRoom = new Room("Trolls Room", new Costs(1, 3, 2));
	Room* greatHall = new Room("Great Hall", new Costs(2, 4, 1, 2));
	Room* bottleRoom = new Room("Bottle Room", new Costs(0, 0, 0, 2));
	Room* larder = new Room("Larder", new Costs(0, 0, 10, 1));
	Room* kitchen = new Room("Kitchen", new Costs(1, 1, 2, 2));
	Room* iceBox = new Room("Ice Box", new Costs(10));
	Room* dampCavern = new Room("Damp Cavern", new Costs(1, 2, 2, 1));
	Room* wellRoom = new Room("Well Room", new Costs(0, 0, 1, 10));
	Room* barracks = new Room("Barracks", new Costs(0, 0, 2));
	Room* guardRoom = new Room("Guard Room", new Costs(2, 1));
	Room* entrance = new Room("Entrance", new Costs(1));

	// 2) Map out the relative (north, east, south, west) adjacent rooms for each
	// Room object, with nullptr representing a lack of room for that specific direction.
	// The Room objects are connected to eachother in a form of (up to four-way) 
	// doubly-linked list.

	entrance->MapRooms(dampCavern);
	guardRoom->MapRooms(barracks, dampCavern);
	barracks->MapRooms(nullptr, nullptr, guardRoom);
	wellRoom->MapRooms(nullptr, nullptr, dampCavern, larder);
	dampCavern->MapRooms(wellRoom, kitchen, entrance, guardRoom);
	iceBox->MapRooms(kitchen);
	kitchen->MapRooms(greatHall, larder, iceBox, dampCavern);
	larder->MapRooms(bottleRoom, nullptr, wellRoom, kitchen);
	bottleRoom->MapRooms(nullptr, nullptr, larder, greatHall);
	greatHall->MapRooms(bottleRoom, mysteriousRoom, kitchen, trollsRoom);
	trollsRoom->MapRooms(library, mysteriousRoom, greatHall);
	mysteriousRoom->MapRooms(library, nullptr, greatHall, trollsRoom);
	library->MapRooms(nullptr, mysteriousRoom, trollsRoom);

	// 3) Return the room where the game starts (the doubly-linked list will ensure
	// that returning any of the rooms will allow the game to function without issue).
	return entrance;
}

//Inteprets human input and resolves it to a short-hand single letter command
string GetHumanCommand(void)
{
	//get command
	string input;
	cin >> input;

	//intepret command
	if (input == "n" || input == "N" || input == "north" || input == "North") return "n";
	else if (input == "e" || input == "E" || input == "east" || input == "East") return  "e";
	else if (input == "s" || input == "S" || input == "south" || input == "South") return  "s";
	else if (input == "w" || input == "W" || input == "west" || input == "West") return  "w";
	else if (input == "h" || input == "H" || input == "help" || input == "Help" || input == "c" || input == "C" || input == "commands" || input == "Commands") return  "h";
	else if (input == "exit" || input == "Exit" || input == "q" || input == "quit" || input == "Quit" || input == "kill" || input == "Kill") return  "q";
	else return "i";
}

bool Game(void)
{
	//assemble rooms object structure

	Room* current = AssembleRooms();
	RoomChangeData* tempRoom = nullptr;

	//game loop
	while (true)
	{
		cout << "You're in the " << current->GetName() << "!" << endl << "CMD: ";
		string cmd = GetHumanCommand();

		if (cmd == "q") break;
		else if (cmd == "i") cout << "Invalid command!" << endl << endl;
		else if (cmd == "h") cout << "Command list: (n) go north, (e) go east, (s) go south, (w) go west, (q) quit" << endl << endl;
		else
		{
			if (cmd == "n") tempRoom = current->GoNorth();
			else if (cmd == "e") tempRoom = current->GoEast();
			else if (cmd == "s") tempRoom = current->GoSouth();
			else if (cmd == "w") tempRoom = current->GoWest();

			if (tempRoom->nextRoom == current) cout << "There is no room in that direction!" << endl << endl;
			else
			{
				current = tempRoom->nextRoom;
				cout << "You spent " << tempRoom->costOfTravel << " seconds walking!" << endl << endl;
				tempRoom = nullptr;
			}
		}
	}

	//game has run its course; release memory and flag game as complete
	delete current, tempRoom;
	return false;
}

int main(void)
{
	while (Game());
	return 0;
}