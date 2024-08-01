#include "Actor.h"
#include "StudentWorld.h"
#include <string>
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(int id, int sx, int sy, Direction startingDirection, StudentWorld* sw, double size,
	int depth) : GraphObject(id, sx, sy, startingDirection, size, depth), m_studentWorld(sw), n_isAlive(true) {

	setVisible(true);
}

bool Actor::isAlive() const {
	return n_isAlive;
}

void Actor::setDead() {
	n_isAlive = false;
}

void Actor::setAlive(bool alive) {
	n_isAlive = alive;
}

bool Actor::canPick() const {
	return m_canPick;
}

void Actor::setCanPick(bool canPickUp) {
	m_canPick = canPickUp;
}

void Actor::setLifespan(int lifespan) {
	m_lifespan = lifespan;
}

Protestor::Protestor(int id, int sx, int sy, GraphObject::Direction startingDirection, StudentWorld* sw, double size, int depth)
	: Actor(id, sx, sy, left, sw, 1.0, 0) {
	setVisible(true);
}


bool proximityCheck(int ix, int iy, string status, int bx, int by) {

	if (status == "falling") {
		if (((ix >= bx - 4) && (ix <= bx + 4)) && ((iy >= by - 4) && (iy <= by + 4))) {
			return true;
		}
		return false;
	}
	return false;
}

bool Actor::pCheck(int prox, Actor* a) {
	int gX = getX();
	int gY = getY();

	int aX = a->getX();
	int aY = a->getY();

	if (std::abs(aX - gX) <= prox && std::abs(aY - gY) <= prox) {
		return true;
	}
	return false;
}


//initilize health, squirt, sonar charge, gold sack (keeps track of inventory of how much in ice field), ice_facing 
Iceman::Iceman(StudentWorld* student) : Actor(IID_PLAYER, 30, 60, right, student, 1.0, 0),
n_health(100), n_lives(3), n_barrels(0), n_squirts(5), n_sonarCharge(1), n_goldSack(0), n_icefacing(right), n_score(0)
{
	setVisible(true);
}

void Iceman::increaseScore(int y) {
	n_score += y;
}

void Iceman::incrBarrelCount(){
	n_barrels++;
}

void Iceman::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getDirection() != left) {
				setDirection(left);
			}
			else if (getX() > 0 && !getWorld()->isBoulderAt(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
				getWorld()->clearIce(getX(), getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right) {
				setDirection(right);
			}
			else if (getX() < VIEW_WIDTH - 4 && !getWorld()->isBoulderAt(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
				getWorld()->clearIce(getX(), getY());
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			else if (getY() < VIEW_HEIGHT - 4 && !getWorld()->isBoulderAt(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
				getWorld()->clearIce(getX(), getY());
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			else if (getY() > 0 && !getWorld()->isBoulderAt(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
				getWorld()->clearIce(getX(), getY());
			}
			break;

		case KEY_PRESS_SPACE:
			//implement water when found activate space
			//then if water found update the menu on water used
			if (n_squirts > 0) {
				int squirtX = getX();
				int squirtY = getY();

				switch (getDirection()) {
				case up:
					squirtY += 4;
					break;
				case down:
					squirtY -= 4;
					break;

				case left:
					squirtX -= 4;
					break;

				case right:
					squirtX += 4;
					break;
				}
				getWorld()->addActor(new Squirts(IID_WATER_SPURT, squirtX, squirtY, getDirection(), getWorld(), 1.0, 1));
				n_squirts--;
				getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			}
			break;

		case KEY_PRESS_TAB:
			cout << "TAB PRESSED! " << endl;
			if (getGold() != 0) {
				GoldNuggets* goldNugget = new GoldNuggets(IID_GOLD, getX(), getY(), getWorld(), 20);
				goldNugget->dropGold(this);
				getWorld()->addActor(goldNugget);
				cout << "Dropped a gold nugget!" << endl;
				break;
			}
			else
				cout << "No more gold to drop." << endl;
			break;
		}

	}
}

//need to return a value here
int Iceman::getHealth() const { return n_health; }
int Iceman::getSquirts() const { return n_squirts; }
int Iceman::getGold() const { return n_goldSack; }
int Iceman::getLives() const { return n_lives; }
int Iceman::getBarrels() const { return n_barrels; }
int Iceman::getSonar() const { return n_sonarCharge; }
int Iceman::getScore() const { return n_score;  }


void Iceman::incrOilCount() {
	n_barrels++;
}

void Iceman::incrGoldCount() {
	n_goldSack++;
}

void Iceman::decrGoldCount() {
	if (n_goldSack != 0) {
		n_goldSack--;
	}
	else
		cout << "No more gold!" << endl;
}

void Iceman::incrSonarCount() {
	n_sonarCharge++;
}

void Iceman::decrSonarCount() {
	n_sonarCharge--;
}

void Iceman::incrWater() {
	n_squirts = n_squirts + 5;
}

void Iceman::decrWater() {
	n_squirts--;
}

void Iceman::annoy(int points) {
	n_health -= points;
	if (n_health <= 0) {
		setDead();
	}
}

Ice::Ice(StudentWorld* sw, int ID, int startX, int startY) :
	Actor(IID_ICE, startX, startY, right, sw, 0.25, 3) {
	setVisible(true);
}

//I know you said Ice doesn't need a doSomething but I included
//it as practice for my other functions
void Ice::doSomething() {}

Boulder::Boulder(StudentWorld* sw, int sx, int sy)
	: Actor(IID_BOULDER, sx, sy, down, sw, 1.0, 1), m_state(stable), m_waitingTicks(0) {
	setVisible(true);
}

void Boulder::doSomething() {
	// Check if boulder is alive
	if (!isAlive()) {
		return;
	}


	// Perform actions based on boulder's state
	switch (m_state) {
	case stable:
		// Check if no ice below the boulder
		if (!getWorld()->isIceAt(getX(), getY() - 1) &&
			!getWorld()->isIceAt(getX() + 1, getY() - 1) &&
			!getWorld()->isIceAt(getX() + 2, getY() - 1) &&
			!getWorld()->isIceAt(getX() + 3, getY() - 1)) {
			m_state = waiting;
			m_waitingTicks = 30;
		}
		break;

	case waiting:
		// Decrement waiting ticks
		m_waitingTicks--;
		if (m_waitingTicks <= 0) {
			m_state = falling;
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		break;

	case falling:
		// Clear ice directly below the boulder
		getWorld()->clearIce(getX(), getY() - 1);

		// Check for collision with Iceman
		if (getWorld()->isIcemanAt(getX(), getY())) {
			// Annoy Iceman to set health to 0
			getWorld()->annoyIceman(getWorld()->getIceman()->getHealth());
		}

		// Move the boulder down by one square
		moveTo(getX(), getY() - 1);

		// Check if the boulder has reached the bottom or hit another boulder
		if (getY() == 0 || getWorld()->isBoulderAt(getX(), getY() - 1)) {
			setDead();
		}
		break;
	}


}


Boulder::~Boulder() {}// Cleanup code, if any

BarrelsofOil::BarrelsofOil(int id, int sx, int sy, StudentWorld* sw, int lifespan) :
	Actor(IID_BARREL, sx, sy, right, sw, 1.0, 2), bm_lifespan(lifespan) {
	setVisible(true);
	setCanPick(true);
	setAlive(true);
}

void BarrelsofOil::doSomething() {
	if (!isAlive()) {
		return;
	}

	if (pCheck(4, getWorld()->getIceman()))
	{
		setVisible(true);
	}
	if (isVisible() && pCheck(3, getWorld()->getIceman()))
	{
		setAlive(false);

		getWorld()->getBarrels();
		getWorld()->increaseScore(1000);
		getWorld()->playSound(SOUND_FOUND_OIL);
		setVisible(false);
	}
}

bool BarrelsofOil::radiusThreeOil() {

	Iceman* iceman = getWorld()->getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	int dx = getX() - icemanX;
	int dy = getY() - icemanY;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance <= 3.0) {
		return true;
	}
	else {
		return false;
	}
}


BarrelsofOil::~BarrelsofOil() {}

Squirts::Squirts(int id, int sx, int sy, Direction staringdirection, StudentWorld* sw, double size, int depth) :
	Actor(IID_WATER_SPURT, sx, sy, down, sw, 1.0, 1), m_travelDistance(4)
{
	setVisible(true);
}

void Squirts::doSomething()
{
	if (!isAlive()) {
		return;
	}

	// Check if the squirt has traveled its maximum distance
	if (m_travelDistance <= 0) {
		setDead();
		return;
	}

	// Move the squirt in its current direction
	switch (getDirection()) {
	case up:
		if (getWorld()->isIceAt(getX() + 1, getY()) || getWorld()->isBoulderAt(getX(), getY())) {
			setDead();
		}
		else {
			//set direction of water to the direction of player, 
			moveTo(getX(), getY() + 1);
		}
		break;
	case down:
		if (getWorld()->isIceAt(getX(), getY() - 1) || getWorld()->isBoulderAt(getX(), getY() - 1)) {
			setDead();
		}
		else {
			moveTo(getX(), getY() - 1);
		}
		break;
	case left:
		if (getWorld()->isIceAt(getX() - 1, getY()) || getWorld()->isBoulderAt(getX() - 1, getY())) {
			setDead();
		}
		else {
			moveTo(getX() - 1, getY());
		}
		break;
	case right:
		if (getWorld()->isIceAt(getX() + 1, getY()) || getWorld()->isBoulderAt(getX() + 1, getY())) {
			setDead();
		}
		else {
			moveTo(getX() + 1, getY());
		}
		break;
	}

	m_travelDistance--;

	// Check if the squirt hit a protester
	// Implement the logic to annoy protesters here
}

Squirts::~Squirts() {}

//while set(visible true) for ice, if iceman approches ice, make the ice broken, set(visible false)	
GoldNuggets::GoldNuggets(int id, int sx, int sy, StudentWorld* sw, int lifespan)
	: Actor(IID_GOLD, sx, sy, right, sw, 1.0, 2), m_lifespan(lifespan) {
	setVisible(false);
	setAlive(true);
	setCanPick(true);

};

bool GoldNuggets::hasGold(int iceX, int iceY) {
	// Calculate the distance between the Iceman and the GoldNugget
	int dx = getX() - iceX;
	int dy = getY() - iceY;
	double distance = sqrt(dx * dx + dy * dy);

	// If the distance is less than or equal to 3.0, the Iceman can pick up the GoldNugget
	return distance <= 3.0;
}


bool GoldNuggets::withinRadiusThree() { // returns whether the iceman is within 3 radius of the nugget 
	Iceman* iceman = getWorld()->getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	int dx = getX() - icemanX;
	int dy = getY() - icemanY;
	double distance = sqrt(dx * dx + dy * dy);

	//cout << "rad 3" << endl; // prints so it does run 

	if (distance <= 3.0) {
		return true;
	}
	else {
		return false;
	}
}

bool GoldNuggets::withinRadiusFour() {
	Iceman* iceman = getWorld()->getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	int dx = getX() - icemanX;
	int dy = getY() - icemanY;
	double distance = sqrt(dx * dx + dy * dy);

	//cout << "rad 4" << endl; 

	if (distance <= 4.0) {
		return true;
	}
	else {
		return false;
	}
}

void GoldNuggets::dropGold(Iceman* iceman) {
	iceman = getWorld()->getIceman();
	// Get the Iceman's position
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	setCanPick(false);
	GoldNuggets* newNugget = new GoldNuggets(IID_GOLD, icemanX, icemanY, getWorld(), 200);

	// Add the new nugget to the game world's actors vector
	getWorld()->addActor(newNugget);
	iceman->decrGoldCount();

}

void GoldNuggets::decrLifespan() {
	if (m_lifespan > 0) {
		m_lifespan--;
	}
}


void GoldNuggets::doSomething() {

	if (!isAlive()) {
		//	cout << "not alive" << endl; 
		return;
	}

	if (canPick() == true && withinRadiusThree() == true) {
		//	cout << "rad4 && vis" << endl;
		setVisible(false);
		setAlive(false);
	}

	if (!isVisible() && withinRadiusFour() == true) {
		//	cout << "rad 4 and invis" << endl; 
		setVisible(true);
		//	cout << "call invis" << endl; 
		return;
	}

	if (m_lifespan <= 0) {
		setVisible(false);
		getWorld()->removeActor(this);
		return;
	}
	m_lifespan--;
}

Sonarkits::Sonarkits(int id, int sx, int sy, StudentWorld* sw, int lifespan)
	: Actor(IID_SONAR, sx, sy, right, sw, 1.0, 2), m_lifespan(lifespan) {
	setVisible(true);
	setCanPick(true);
	setAlive(true);

	// changes based on the level - temp is level 1 for now 
	//setLifespan(max(100, 300 - 10 * 1)); 

	if (m_lifespan <= 0) {
		setVisible(false);
		getWorld()->removeActor(this);
		return;
	}
	m_lifespan--;

}

void Sonarkits::doSomething() {
	if (!isAlive()) {
		return;
	}

	//setAlive(false);
	//getWorld()->increaseScore(75);

	if (m_lifespan <= 0) {
		setVisible(false);
		getWorld()->removeActor(this);
		return;
	}
	m_lifespan--;

}

bool Sonarkits::radiusThreeSonar() {
	Iceman* iceman = getWorld()->getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	int dx = getX() - icemanX;
	int dy = getY() - icemanY;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance <= 3.0) {
		return true;
	}
	else {
		return false;
	}
}

Water::Water(int id, int sx, int sy, StudentWorld* sw, int lifespan)
	: Actor(IID_WATER_POOL, sx, sy, right, sw, 1.0, 2), m_lifespan(lifespan) {
	setVisible(true);
	setCanPick(true);
	setAlive(true);
	setLifespan(max(100, 300 - 10 * 1));

	if (m_lifespan <= 0) {
		setVisible(false);
		getWorld()->removeActor(this);
		return;
	}
	m_lifespan--;

}

void Water::doSomething() {
	if (!isAlive()) {
		return;
	}


	if (m_lifespan <= 0) {
		setVisible(false);
		getWorld()->removeActor(this);
		return;
	}
	m_lifespan--;

}

bool Water::radiusThreeWater() {
	Iceman* iceman = getWorld()->getIceman();
	int icemanX = iceman->getX();
	int icemanY = iceman->getY();

	int dx = getX() - icemanX;
	int dy = getY() - icemanY;
	double distance = sqrt(dx * dx + dy * dy);

	if (distance <= 3.0) {
		return true;
	}
	else {
		return false;
	}
}


RegularProtestor::RegularProtestor(int id, int sx, int sy, StudentWorld* sw, int hitpoints)
	: Protestor(IID_PROTESTER, sx, sy, left, sw, 1.0, 0), m_hitpoints(hitpoints), m_restTicks(5) {
	// add bool to protestor for bribe later 
	setLeaveOil(false);

	int maxLeft = startX - 1; // max steps to left 
	int maxRight = 63 - startX; // Maximum steps to the right
	int maxUp = 63 - startY; // Maximum steps upwards
	int maxDown = startY - 1; // Maximum steps downwards

	int minSteps = std::max(8, 1); // Ensure minimum steps are at least 8
	int maxSteps = std::min({ maxLeft, maxRight, maxUp, maxDown, 60 });

	numSquaresToMoveInCurrentDirection = rand() % (maxSteps - minSteps + 1) + minSteps;

}

void RegularProtestor::setLeaveOil(bool leaveOil) {
	m_leaveOil = leaveOil;
}

bool RegularProtestor::leaveOilField() const {
	return m_leaveOil;
}

int RegularProtestor::getHitPoints() const
{
	return m_hitpoints;
}

void RegularProtestor::setHitPoints(int hitpts) {
	m_hitpoints = hitpts;
}


void RegularProtestor::doSomething() {
	if (!isAlive()) {
		return;
	}

	if (m_restTicks > 0) {
		m_restTicks--;
		return;
	}
	//if (m_restTicks == 0) {
	//	chooseNewDirection();
	//	m_restTicks = 5;
	//}

	if (numSquaresToMoveInCurrentDirection > 0) {
		moveInCurrentDirection();
	}
	else {
		chooseNewDirection();
	}

	m_restTicks = 10; // Example value; adjust according to game rules
}


void RegularProtestor::moveInCurrentDirection() {
	int x = getX();
	int y = getY();

	switch (getDirection()) {
	case left:
		if (x > 0) moveTo(x - 1, y);
		break;
	case right:
		if (x < 63) moveTo(x + 1, y);
		break;
	case up:
		if (y < 63) moveTo(x, y + 1);
		break;
	case down:
		if (y > 0) moveTo(x, y - 1);
		break;
	default:
		break;
	}

	numSquaresToMoveInCurrentDirection--;
}

void RegularProtestor::chooseNewDirection() {
	Direction newDirection;
	int maxSteps;

	do {
		newDirection = static_cast<Direction>(rand() % 4);
	} while (newDirection == getDirection());

	setDirection(newDirection);

	switch (newDirection) {
	case left:
		maxSteps = getX() - 1;
		break;
	case right:
		maxSteps = 63 - getX();
		break;
	case up:
		maxSteps = 63 - getY();
		break;
	case down:
		maxSteps = getY() - 1;
		break;
	default:
		maxSteps = 8;
		break;
	}

	maxSteps = std::min(maxSteps, 60);
	//numSquaresToMoveInCurrentDirection = rand() % (maxSteps - 8 + 1) + 8;
}

HardcoreProtestor::HardcoreProtestor(int id, int sx, int sy, StudentWorld* sw, int hitpoints)
	: Protestor(IID_HARD_CORE_PROTESTER, sx, sy, left, sw, 1.0, 0), m_hitpoints(hitpoints), m_restTicks(5) {
	// add bool to protestor for bribe later 
	setLeaveOil(false);

	int maxLeft = startX - 1; // max steps to left 
	int maxRight = 63 - startX; // Maximum steps to the right
	int maxUp = 63 - startY; // Maximum steps upwards
	int maxDown = startY - 1; // Maximum steps downwards

	int minSteps = std::max(8, 1); // Ensure minimum steps are at least 8
	int maxSteps = std::min({ maxLeft, maxRight, maxUp, maxDown, 60 });

	numSquaresToMoveInCurrentDirection = rand() % (maxSteps - minSteps + 1) + minSteps;

}

void HardcoreProtestor::setLeaveOil(bool leaveOil) {
	m_leaveOil = leaveOil;
}

bool HardcoreProtestor::leaveOilField() const {
	return m_leaveOil;
}

int HardcoreProtestor::getHitPoints() const
{
	return m_hitpoints;
}

void HardcoreProtestor::setHitPoints(int hitpts) {
	m_hitpoints = hitpts;
}


void HardcoreProtestor::doSomething() {
	if (!isAlive()) {
		return;
	}

	if (m_restTicks > 0) {
		m_restTicks--;
		return;
	}
	//if (m_restTicks == 0) {
	//	chooseNewDirection();
	//	m_restTicks = 5;
	//}

	if (numSquaresToMoveInCurrentDirection > 0) {
		moveInCurrentDirection();
	}
	else {
		chooseNewDirection();
	}

	m_restTicks = 10; // Example value; adjust according to game rules
}


void HardcoreProtestor::moveInCurrentDirection() {
	int x = getX();
	int y = getY();

	switch (getDirection()) {
	case left:
		if (x > 0) moveTo(x - 1, y);
		break;
	case right:
		if (x < 63) moveTo(x + 1, y);
		break;
	case up:
		if (y < 63) moveTo(x, y + 1);
		break;
	case down:
		if (y > 0) moveTo(x, y - 1);
		break;
	default:
		break;
	}

	numSquaresToMoveInCurrentDirection--;
}

void HardcoreProtestor::chooseNewDirection() {
	Direction newDirection;
	int maxSteps;

	do {
		newDirection = static_cast<Direction>(rand() % 4);
	} while (newDirection == getDirection());

	setDirection(newDirection);

	switch (newDirection) {
	case left:
		maxSteps = getX() - 1;
		break;
	case right:
		maxSteps = 63 - getX();
		break;
	case up:
		maxSteps = 63 - getY();
		break;
	case down:
		maxSteps = getY() - 1;
		break;
	default:
		maxSteps = 8;
		break;
	}

	maxSteps = std::min(maxSteps, 60);
	//numSquaresToMoveInCurrentDirection = rand() % (maxSteps - 8 + 1) + 8;
}

//void RegularProtestor::shoutIfNearIceman() {
//	// Implement shouting logic
//}
