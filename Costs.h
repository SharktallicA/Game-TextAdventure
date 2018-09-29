#pragma once
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