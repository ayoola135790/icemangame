#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
public:

    bool isAlive() const;
    virtual void setDead();
    virtual void doSomething() = 0;
    void annoy(int points);

    bool pCheck(int prox, Actor* a);

    void setAlive(bool alive);
    bool canPick() const;
    void setCanPick(bool canPickUp);
    void setLifespan(int lifespan);

    //protected so that it will allow derived classes to access the variables directly
protected:
    int imageID;
    int startX;
    int startY;
    GraphObject::Direction startDirection;
    float size;
    Actor(int id, int sx, int sy, Direction startingDirection, StudentWorld* sw, double size, int depth);
    //virtual ~Actor();
    //To make a pure virtual function

    bool n_isAlive;
    bool m_canPick;
    int m_lifespan;


    //Getter for key function
    StudentWorld* getWorld(){
        return m_studentWorld;
    }
 
private:
    StudentWorld* m_studentWorld;

};


class Iceman : public Actor {

private: 
    int n_health;
    int n_squirts;
    int n_lives;
    int n_sonarCharge;
    int n_barrels;
    int n_goldSack;
    Direction n_icefacing;

    int n_score;

public: 

    //IID_PLAYER
    Iceman(StudentWorld* student);
    virtual void doSomething() override;


    void incrGoldCount();
    void decrGoldCount();
    void incrSonarCount();
    void decrSonarCount();
    void incrWater();
    void decrWater();
    void incrOilCount();

    void increaseScore(int y);
    void incrBarrelCount();

   
    int getScore() const;

    int getLevel() const;
    int getHealth() const;
    int getSquirts() const;
    int getLives() const;
    int getGold() const;
    int getBarrels() const;
    int getSonar() const;

    int getOil() const;

    void annoy(int points);

    bool proximityCheck(int ix, int iy, std::string status, int bx, int by);

  


};

class Protestor : public Actor {
public:
    Protestor(int id, int sx, int sy, GraphObject::Direction startingDirection, StudentWorld* sw, double size, int depth);

};

class RegularProtestor : public Protestor {
    //IID_PROTESTER
public:
    RegularProtestor(int id, int sx, int sy, StudentWorld* sw, int hitpoints);



    virtual void doSomething() override;
    virtual ~RegularProtestor() {};

    bool leaveOilField() const;
    void setLeaveOil(bool leaveOil);

    int getHitPoints() const;
    void setHitPoints(int hitpts);

    void chooseNewDirection();
    void moveInCurrentDirection();
    //void shoutIfNearIceman();

private:
    int m_hitpoints, m_restTicks;
    int numSquaresToMoveInCurrentDirection;
    bool m_leaveOil;


};

class HardcoreProtestor : public Protestor {
public:
    //ID_HARD_CORE_PROTESTER
    HardcoreProtestor(int id, int sx, int sy, StudentWorld* sw, int hitpoints);
    virtual void doSomething() override;
    virtual ~HardcoreProtestor() {};

    bool leaveOilField() const;
    void setLeaveOil(bool leaveOil);

    int getHitPoints() const;
    void setHitPoints(int hitpts);

    void chooseNewDirection();
    void moveInCurrentDirection();
    //void shoutIfNearIceman();

private:
    int m_hitpoints, m_restTicks;
    int numSquaresToMoveInCurrentDirection;
    bool m_leaveOil;
};


class BarrelsofOil : public Actor {
public:    
    //IID_BARREL
    BarrelsofOil(int id, int sx, int sy, StudentWorld* sw, int lifespan);
    virtual void doSomething();
    virtual ~BarrelsofOil();

    bool radiusThreeOil();

private:

     int bm_lifespan;
};

class Squirts: public Actor{
    //IID_WATER_SPURT
public:
    Squirts(int id, int sx, int sy, Direction staringdirection, StudentWorld* sw, double size, int depth);
    virtual void doSomething();
    virtual ~Squirts();

private:
    int m_travelDistance;
};

class GoldNuggets : public Actor {
    //IID_GOLD
public:
    GoldNuggets(int id, int sx, int sy, StudentWorld* sw, int lifespan);
    bool hasGold(int x, int y);
    bool withinRadiusThree();
    bool withinRadiusFour();
    virtual void doSomething() override;
    void dropGold(Iceman* iceman);
    void decrLifespan();
    virtual ~GoldNuggets() {};

private:
    int m_lifespan;
    //bool canPick = true;
    //bool isAlive = true;
    int x, y;

};

class Sonarkits: public Actor{
public:   
    
    //IID_SONAR 
   Sonarkits(int id, int sx, int sy, StudentWorld* sw, int lifespan);
   virtual void doSomething();
   virtual ~Sonarkits() {};

   bool radiusThreeSonar();

private:
    int m_lifespan;

};


class Water: public Actor{
public:
    //IID_WATER_POOL
    Water(int id, int sx, int sy, StudentWorld* sw, int lifespan);
    virtual void doSomething() override;
    virtual ~Water() {};

    bool radiusThreeWater();

private:
    int m_lifespan;

};

class Ice : public Actor {

    //IID_ICE

public:
    Ice(StudentWorld* sw, int ID, int startX, int startY);
    virtual void doSomething() override;
    virtual ~Ice() {};
};

class Boulder : public Actor {
public:
    //IID_WATER_POOL
    Boulder(StudentWorld* sw, int sx, int sy);
    virtual void doSomething();
    virtual ~Boulder();

    enum BoulderState {
        stable,
        waiting,
        falling
    };


private:
    BoulderState m_state;
    int m_waitingTicks;

};

#endif // ACTOR_H_


//bool checkProxyBoulder(int ix, it, string status) {
//    if(status == "falling" && by > iy &&)
//}