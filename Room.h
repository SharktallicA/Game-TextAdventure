#pragma once
#include <string>

using namespace std;

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