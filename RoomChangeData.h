#pragma once

class Room;

//Contains change in room data
class RoomChangeData
{
public:
	Room* nextRoom = nullptr;
	int costOfTravel;

	RoomChangeData(Room* nRoom, int nTravel) { nextRoom = nRoom; costOfTravel = nTravel; }
};