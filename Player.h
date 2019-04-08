#pragma once

class Player
{
private:
	int attack, health = 0;

public:
	Player() { attack = 5; health = 100; }

	int GetAttackStrength(void) { return attack; }

	void ReceiveDamage(int damage) { health -= damage; }
};