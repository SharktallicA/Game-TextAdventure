#pragma once
#include <string>

using namespace std;

//Contains data that represents a place and its relative neighbours
class Place
{
private:
	string name = "";

	//linked list hardpoints
	Place* north = nullptr;
	Place* east = nullptr;
	Place* south = nullptr;
	Place* west = nullptr;

	Costs* costs;

public:
	Place(string nName, Costs* nCosts) { name = nName; costs = nCosts; }
	~Place() { delete north, east, south, west, costs; }

	//Defines the layout of places relative to this one
	void MapPlaces(Place* nNorth = nullptr, Place* nEast = nullptr, Place* nSouth = nullptr, Place* nWest = nullptr)
	{
		if (north || east || south || west)
		{
			cout << "ILLEGAL CALL: Place already mapped!" << endl;
			return;
		}

		north = nNorth;
		east = nEast;
		south = nSouth;
		west = nWest;
	}

	//Returns this places's name
	string GetName() { return name; }

	PlaceChangeData* GoNorth()
	{
		if (!north) return new PlaceChangeData(this, 0);
		return new PlaceChangeData(north, costs->n);
	}
	PlaceChangeData* GoEast()
	{
		if (!east) return new PlaceChangeData(this, 0);
		return new PlaceChangeData(east, costs->e);
	}
	PlaceChangeData* GoSouth()
	{
		if (!south) return new PlaceChangeData(this, 0);
		return new PlaceChangeData(south, costs->s);
	}
	PlaceChangeData* GoWest()
	{
		if (!west) return new PlaceChangeData(this, 0);
		return new PlaceChangeData(west, costs->w);
	}
};