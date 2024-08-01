#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <string>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

//NOTE TO SELF: Always have foward declarations
class Iceman;
class Ice;
class Boulder;
class Actor;
class Sonarkits;
class GoldNuggets;
class RegularProtestor;
class HardcoreProtestor;
class BarrelsofOil;


class StudentWorld : public GameWorld{

public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), pt1(nullptr){
	}

	virtual int init() override;
	virtual int move() override;
	virtual void cleanUp() override;
	virtual void clearIce(int x, int y);
	void setDisplayText();
	void removeDeadGameObjects();

	bool isIceAt(int x, int y);
	bool isBoulderAt(int x, int y);
	bool isNearIceman(Boulder* boulder, int radius);
	void annoy(int points);
	void annoyIceman(int points);
	void annoyProtester(Boulder* boulder, int points);
	bool isIcemanAt(int x, int y);

	int getBarrels() const;

	bool noverlap(int x, int y);

	void addActor(Actor* actor);
	void removeActor(Actor* actor);

	void pickGold();
	void pickSonar();
	void pickWater();
	void pickOil();
	
	Iceman* getIceman() const { return pt1; }
	
	
	//bool isNearProtester(Boulder* boulder, int radius);
	//bool isNearIceman(Boulder* boulder, int radius);
	//bool isBoulderAt(int x, int y);


	

private:

	//container to store ice objects
	Iceman* pt1;
	Iceman* icemanXY;
	//might have to adjust size as needed, creating 2D array
	Ice* iceblocks[64][64];

	std::vector <Actor*> actors;
	std::vector<GoldNuggets*> nuggets;

	//Create barrel of Oil
	std::vector<BarrelsofOil*> oils;

	

	//create a regular protestor 
	RegularProtestor* prot1;
	HardcoreProtestor* prot2;

private:
	int m_barrels;

};

#endif // STUDENTWORLD_H_
