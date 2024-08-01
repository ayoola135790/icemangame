#include "StudentWorld.h"
#include "Actor.h"

#include <string>
#include <array>
#include <vector>
#include <sstream>x
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int health = pt1->getHealth();
	int lives = pt1->getLives();
	int squirts = pt1->getSquirts();
	int gold = pt1->getGold();
	int barrelsLeft = pt1->getBarrels();

	int score = getScore();

	ostringstream oss;

	oss << "Lvl: " << level << " Lives: " << lives << " Hlth: "
		<< health << "% " << " Wtr: " << " Gold: " << gold << " Oil: " <<
		barrelsLeft << " Sonar: " << " Squirts: " << squirts << " Scr: "
		<< score << endl;
	string s = oss.str();

	setGameStatText(s);
}



int StudentWorld::init() {


	//creates Iceman
	pt1 = new Iceman(this);
	icemanXY = pt1;


	//initialize all elements of iceblocks to nullpointer
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 64; x++) {
			iceblocks[x][y] = nullptr;
		}
	}

	//create iceblocks
	int ix = 64;
	int iy = 64;

	for (int y = 0; y < iy - 4; y++) {
		for (int x = 0; x < ix; x++) {
			iceblocks[x][y] = new Ice(this, IID_ICE, x, y);

		}
	}

	for (int i = 4; i < 60; i++) {
		for (int x = 30; x < 34; x++) {
			iceblocks[x][i]->setVisible(false);
		}
	}

	bool occupied[VIEW_WIDTH][VIEW_HEIGHT] = { false };

	// Place the boulder to the right of Iceman 4 units right
	unsigned int nBoulders = min<unsigned int>(getLevel() / 2 + 2, 9);
	for (unsigned int k = 0; k < nBoulders; k++) {
		int x = rand() % 61;
		int y = rand() % 37 + 20;

		do {
			// Placeholder condition
			if (1 == 1) {
				actors.push_back(new Boulder(this, x, y));
				clearIce(x, y);
			}
			else {
				k--;
			}
		} while (occupied[x][y]);
	}

	//create gold nuggets 
	int numNuggets = 5; // changes based on level - use 5 for now 

	for (int i = 0; i < numNuggets; i++) {
		int x, y;
		do {
			x = rand() % (VIEW_WIDTH - 2) + 1;
			y = rand() % (VIEW_HEIGHT - 2) + 1;


		} while (iceblocks[x][y] == nullptr && occupied[x][y]);
		// only spawns where there is an iceblock - most of the time, will occasionally appear halfway
		nuggets.push_back(new GoldNuggets(IID_GOLD, x, y, this, 999999999));
	}
	//nuggets.push_back(new GoldNuggets(IID_GOLD, 36, 60, this, 999999999));

	//create sonar kits 
	int numSonars = 0;
	for (int i = 0; i < numSonars; i++) {
		int x, y;
		do {
			x = rand() % (VIEW_WIDTH - 2) + 1;
			y = rand() % (VIEW_HEIGHT - 4) + 1;
		} while (occupied[x][y]);

		actors.push_back(new Sonarkits(IID_SONAR, x, y, this, 1000));
		//cout << "make sonar" << endl;
	}
	actors.push_back(new Sonarkits(IID_SONAR, 0, 60, this, 10000));

	//create waterpools 
	int numPools = 2;
	for (int i = 0; i < numPools; i++) {
		int x, y;
		do {
			x = rand() % (VIEW_WIDTH - 2) + 1;
			y = rand() % (VIEW_HEIGHT - 4) + 1;
		} while (occupied[x][y]);
		actors.push_back(new Water(IID_WATER_POOL, x, y, this, 2000));
	}

	//create barrels of oil 
	int Oils = 3;
	for (int i = 0; i < Oils; i++) {
		int x, y;
		do {
			x = rand() % (VIEW_WIDTH - 2) + 1;
			y = rand() % (VIEW_HEIGHT - 4) + 1;
		} while (occupied[x][y]);
		actors.push_back(new BarrelsofOil(IID_BARREL, x, y, this, 2000));
	}




	//create regular protestors 
	int numrProtestors = 2;
	for (int i = 0; i < numrProtestors; i++) {
		int x1, y, x2;
		do {
			x1 = 0;
			x2 = 60;
			y = 60;

		} while (occupied[x1][y] || occupied[x2][y]); // Ensure it doesn't spawn on top of other objects

		actors.push_back(new RegularProtestor(IID_PROTESTER, x1, y, this, 5));
		actors.push_back(new HardcoreProtestor(IID_HARD_CORE_PROTESTER, x2, y, this, 5));
	}




	return GWSTATUS_CONTINUE_GAME;
}



int StudentWorld::move() {

	//update the Iceman to move
	pt1->doSomething();

	pickSonar();
	pickWater();
	this->pickGold();

	//check for collision between iceman and boulders
	for (auto actor : actors) {
		if (Boulder* b = dynamic_cast<Boulder*> (actor)) {
			if (b->getX() == pt1->getX() && b->getY() == pt1->getY()) {
				playSound(SOUND_PLAYER_GIVE_UP);
				return GWSTATUS_PLAYER_DIED;
			}
		}
	}

	//update all actors
	//for (auto it = actors.begin(); it != actors.end(); it++) {
	//	if ((*it)->isAlive()) {
	//		(*it)->doSomething();
	//	}
	//}

	//update the Ice to appear
	for (int i = 0; i < 64; i++) {
		for (int y = 0; y < 64; y++) {
			if (iceblocks[i][y] != nullptr && !iceblocks[i][y]->isVisible()) {
				delete iceblocks[i][y];
				iceblocks[i][y] = nullptr;
			}
		}
	}


	// updates each tick - will cause the lifespan to decrease, etc 
	for (auto actor : actors) {
		if (actor->isAlive()) {
			actor->doSomething();
		}
	}

	removeDeadGameObjects();
	setDisplayText();
	decLives();


	// condition to end game
	if (pt1->getHealth() <= 0) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}


	//clearIce(int x, int y);

	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	return GWSTATUS_CONTINUE_GAME;

}

bool StudentWorld::isIcemanAt(int x, int y) {
	if (pt1 != nullptr && pt1->getX() == x && pt1->getY() == y) {
		return true;
	}
	return false;
}


void StudentWorld::annoyIceman(int points) {}

void StudentWorld::clearIce(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int xi = x + i;
			int yj = y + j;
			if (xi >= 0 && xi < 64 && yj >= 0 && yj < 64) {
				if (iceblocks[xi][yj] != nullptr) {
					iceblocks[xi][yj]->setVisible(false);
					playSound(SOUND_DIG);
					delete iceblocks[xi][yj];
					iceblocks[xi][yj] = nullptr;
				}
			}
		}
	}
}

bool StudentWorld::isIceAt(int x, int y) {
	if (x >= 0 && x < 64 && y >= 0 && y < 64 && iceblocks[x][y] != nullptr) {
		return true;
	}
	return false;
}

bool StudentWorld::isBoulderAt(int x, int y) {
	for (auto actor : actors) {
		if (Boulder* b = dynamic_cast<Boulder*>(actor)) {
			//check if position overlaps with (x,y) coordinates of boulder
			if (b->getX() <= x && x <= b->getX() + 3 && b->getY() <= y && y <= b->getY() + 3) {
				return true;
			}
		}
	}
	return false;

}

bool StudentWorld::isNearIceman(Boulder* boulder, int radius) {

	if (!boulder) {
		return false;
	}

	// Check if the boulder is near the Iceman within the given radius
	int dx = abs(boulder->getX() - pt1->getX());
	int dy = abs(boulder->getY() - pt1->getY());
	return (dx * dy + dy * dy <= radius * radius);
}


void StudentWorld::pickGold() {
	Iceman* iceman = this->getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	for (auto it = this->nuggets.begin(); it != this->nuggets.end(); ) {
		auto* nugget = *it;
		if (nugget->withinRadiusThree() == true) {
			cout << "Iceman PickUp" << endl;
			nugget->doSomething();
			nugget->setVisible(false);
			nugget->setAlive(false);
			playSound(SOUND_GOT_GOODIE);
			increaseScore(100);
			iceman->incrGoldCount();

			it = this->nuggets.erase(it);
		}
		else if (nugget->withinRadiusFour() == true && nugget->isVisible() == false) {
			nugget->doSomething();
		}
		else {
			++it;
		}
	}
}

void StudentWorld::pickSonar() {
	Iceman* iceman = getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	for (auto it = actors.begin(); it != actors.end(); ) {
		Sonarkits* kits = dynamic_cast<Sonarkits*>(*it);
		if (kits && kits->radiusThreeSonar() == true) {
			cout << "Iceman PickUp Sonar" << endl;
			kits->doSomething();
			playSound(SOUND_GOT_GOODIE);
			increaseScore(75);
			iceman->incrSonarCount();

			// Remove the sonar kit from the game world
			it = actors.erase(it);
			delete kits;
		}
		else {
			++it;
		}
	}
}

void StudentWorld::pickWater() {
	Iceman* iceman = getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	for (auto it = actors.begin(); it != actors.end(); ) {
		Water* waterpools = dynamic_cast<Water*>(*it);
		if (waterpools && waterpools->radiusThreeWater() == true) {
			cout << "Iceman PickUp Water: Gained 5 Squirts" << endl;
			waterpools->doSomething();
			playSound(SOUND_GOT_GOODIE);
			increaseScore(100);
			iceman->incrWater();

			// Remove the waterpool from the game world
			it = actors.erase(it);
			delete waterpools;
		}
		else {
			++it;
		}
	}
}


int StudentWorld::getBarrels() const
{
	return m_barrels;
}

void StudentWorld::addActor(Actor* actor) {
	actors.push_back(actor);
}

void StudentWorld::removeActor(Actor* actor) {
	auto it = std::find(actors.begin(), actors.end(), actor);
	if (it != actors.end()) {
		delete* it;  // Free the memory allocated for the actor
		actors.erase(it);
	}
}


//deletes dynamically allocated objects to prevent memory leaks
void StudentWorld::cleanUp() {

	//clean up ice blocks
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			delete iceblocks[x][y];
			iceblocks[x][y] = nullptr;
		}
	}

	if (pt1 != nullptr) {
		delete pt1;
		pt1 = nullptr;
	}


	for (auto actor : actors) {
		delete actor;
	}
	actors.clear();


}

bool StudentWorld::noverlap(int x, int y)
{
	for (auto it = actors.begin(); it != actors.end(); it++)
	{
		int square = pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2);
		if (sqrt(square) <= 6)
			return false;
	}

	if (isIceAt(x, y) || isIceAt(x + 3, y) || isIceAt(x, y + 3) || isIceAt(x + 3, y + 3))
		return false;

	return true;
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
void StudentWorld::removeDeadGameObjects() {

	//Remove dead ice blocks
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			if (iceblocks[x][y] != nullptr && !iceblocks[x][y]->isAlive()) {
				delete iceblocks[x][y];
				iceblocks[x][y] = nullptr;
			}
		}
	}

	auto it = actors.begin();
	while (it != actors.end()) {
		if (!(*it)->isAlive()) {
			delete* it;
			it = actors.erase(it);
		}
		else {
			it++;
		}
	}


};

