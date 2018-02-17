/*
Name: David Zheng
Homework #4: Warriors
Due Date: 3/4/2017
*/

#include<iostream>
#include<vector>
#include<fstream>
#include<cstring>
#include<string>
using namespace std;

// Class Definitions
class Warrior {
public:
	// Warrior Constructor
	Warrior(const string& name, int str) :
		warriorName(name), warriorStrength(str)
	{};

	// Getters
	int getStrength() const
	{
		return warriorStrength;
	}

	string getName() const
	{
		return warriorName;
	}

	// Helper function
	// Allows the warrior to take damage
	void takeDmg(int hit)
	{
		warriorStrength -= hit;
		if (warriorStrength < 0)
		{
			warriorStrength = 0;
		}
	}
	// Kills off the warrior
	void die()
	{
		warriorStrength = 0;
	}
	// Displays the warriors stats
	void display() const
	{
		cout << "  " << warriorName << " : " << warriorStrength << endl;
	}
private:
	string warriorName;
	int warriorStrength;
};

class Noble {
public:
	Noble(const string& name) : nobleName(name), armyStrength(0)
	{};

	// Hires the warrior and increases the overall armyStrength
	// The address of the warrior gets pushed into the vector of Warrior pointers 

	// a variable armyStrength is used to prevent the need to loop through the whole
	// vector to recalculate the army strength
	void hire(Warrior& knight)
	{
		armyStrength += knight.getStrength();
		myArmy.push_back(&knight);
	}
	
	// Battle function
	// compares the army strength between two nobles to determine the winner
	void battle(Noble& defender)
	{
		cout << nobleName << " battles " << defender.nobleName << endl;

		if (armyStrength == defender.armyStrength)
		{
			// Double checks if both armys are dead
			if (armyStrength == 0)
			{
				cout << nobleName << " and " << defender.nobleName << " are both dead" << endl;
				return;
			}

			killArmy();
			defender.killArmy();
			cout << "Mutual Annihalation : ";
			cout << nobleName << " and " << defender.nobleName << " died in each other's hands" << endl;
		}

		else if(armyStrength > defender.armyStrength)
		{
			if (defender.armyStrength == 0)
			{
				cout << defender.nobleName << " is already dead, sire" << endl;
				return;
			}

			int damageRatio = defender.armyStrength / armyStrength;
			armyTakeDamage(damageRatio);
			defender.killArmy();
			cout << nobleName << " defeats " << defender.nobleName << endl;
		}
		
		else
		{
			int damageRatio = armyStrength / defender.armyStrength;
			defender.armyTakeDamage(damageRatio);
			killArmy();
			cout << defender.nobleName << " defeats " << nobleName << endl;
		}
	}

	// Removes the warrior from the vector of Warrior pointers
	// By shifting all elements after the indicated warrior down by 1 index
	// and popping off the last element

	// Could have just swapped the last element with the warrior that is about to be fired
	// If arrangement or ordering was not a concern (Could have save time from shifting)
	void fire(Warrior& knight)
	{
		size_t warriorPos = findKnight(myArmy,knight);
		armyStrength -= myArmy[warriorPos]->getStrength();
		cout << nobleName << " fired " << myArmy[warriorPos]->getName() << endl;
		
		for (size_t i = warriorPos; i < myArmy.size()-1; i++)
		{
			myArmy[i] = myArmy[i + 1];
		}
		myArmy.pop_back();
	}
	
	// Display method
	void display() const
	{
		cout << nobleName << " has a army size of: " << myArmy.size() << endl;

		for (size_t i = 0; i < myArmy.size(); i++)
		{
			myArmy[i]->display();
		}
	}

private:
	string nobleName;
	int armyStrength;
	vector<Warrior*> myArmy;

	//Helps find the Warrior in the army
	size_t findKnight(const vector<Warrior*>& army, const Warrior& knight) const
	{
		for (size_t i = 0; i < army.size(); i++)
		{
			if (knight.getName() == army[i]->getName())
				return i;
		}
		return army.size();
	}

	//
	void killArmy()
	{
		armyStrength = 0;
		for (size_t i = 0; i < myArmy.size(); i++)
		{
			myArmy[i]->die();
		}
	}

	// Needs improvement 
	// Could cut down O(2n) runtime by adding a few checks.
	// - David Zheng
	void armyTakeDamage(int dmgRatio)
	{
		for (size_t i = 0; i < myArmy.size(); i++)
		{
			myArmy[i]->takeDmg(dmgRatio);
		}

		armyStrength = 0;

		for (size_t i = 0; i < myArmy.size(); i++)
		{
			armyStrength += myArmy[i]->getStrength();
		}
	}

};

// Main()
int main()
{
	Noble art("King Arthur");
	Noble lance("Lancelot du Lac");
	Noble jim("Jim");
	Noble linus("Linus Torvalds");
	Noble billie("Bill Gates");

	Warrior cheetah("Tarzan", 10);
	Warrior wizard("Merlin", 15);
	Warrior theGovernator("Conan", 12);
	Warrior nimoy("Spock", 15);
	Warrior lawless("Xena", 20);
	Warrior mrGreen("Hulk", 8);
	Warrior dylan("Hercules", 3);

	jim.hire(nimoy);
	lance.hire(theGovernator);
	art.hire(wizard);
	lance.hire(dylan);
	linus.hire(lawless);
	billie.hire(mrGreen);
	art.hire(cheetah);

	jim.display();
	lance.display();
	art.display();
	linus.display();
	billie.display();

	
	art.fire(cheetah);
	art.display();
	
	art.battle(lance);
	jim.battle(lance);
	linus.battle(billie);
	billie.battle(lance);

}


