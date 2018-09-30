#pragma once

class Place;

//Contains change in place data
class PlaceChangeData
{
public:
	Place* nextPlace = nullptr;
	int costOfTravel;

	PlaceChangeData(Place* nPlace, int nTravel) { nextPlace = nPlace; costOfTravel = nTravel; }
};