#pragma once

class Scenario
{
protected:
	string name = "";

public:
	//Returns scenario's name
	string GetName(void) { return name; }

	virtual void Interact(Player* player) {}

	virtual bool Update(void) { return true; }
};

class EnemyEncounter : public Scenario
{
private:
	string enemyName = "";
	int attack, defence, health = 0;

public:
	EnemyEncounter(string nEnemyName, int nAttack, int nDefence)
	{
		name = "Enemy Encounter";
		enemyName = nEnemyName;
		attack = nAttack;
		defence = nDefence;
		health = 100;
	}

	void Interact(Player* player)
	{
		//execute player's attack
		Utility::setColour(LIGHT_GREEN);
		int yourAttack = (player->GetAttackStrength() * Utility::generateNumber32(2, 10)) - defence;
		if (yourAttack < 0) yourAttack = 0;
		health -= yourAttack;
		cout << "You attacked " << enemyName << ", dealing " << yourAttack << " damage";

		if (health <= 0) cout << " which killed them!" << endl << endl; //catch if player killed enemy
		else //else execute enemy's attack
		{
			cout << endl;
			Utility::setColour(LIGHT_RED);
			int theirAttack = attack * Utility::generateNumber32(1, 3);
			player->ReceiveDamage(theirAttack);
			cout << "They retaliated, dealing " << theirAttack << " damage!" << endl << endl;
		}
		Utility::setColour(WHITE);
	}

	bool Update(void)
	{
		if (health <= 0) return false;
		else return true;
	}
};