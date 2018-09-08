#ifndef UNIT__H
#define UNIT__H

#include <iostream>
#include <string>
#include <random>
#include "multiSprite.h"
#include "followSprite.h"
#include "animation.h"
#include "projectile.h"
#include "explodingSprite.h"

enum TARGET_TYPE{
  ALLY = 0,     //must match current unit's player
  ENEMY = 1,    //cannot match current unit's player
  BOTH = 2,     //can match either player or enemy
  NEITHER = 3,  //has to target an empty space
  SELF = 4      //targets only self
};

enum ACTION_ID{
  NONE = 0,
  ATTACK = 1,
  DEFEND = 2,
  SHOUT = 3,
  SMASH = 4,
  FIREBALL = 5,
  BLIZZARD = 6,
  TRAP = 7,
  AIM = 8,
  HEAL = 9,
  LIGHTNING = 10
};

class Unit{
private:
  std::string characterClass;
  std::string name;
  MultiSprite* sprite;
  
  int player; //Which player this unit is controlled by
  FollowSprite* partySprite;
  FollowSprite* shadow;
  Animation* animation;
  Projectile* projectile;
  ExplodingSprite* explosion;
  
  int atk;
  int def;
  int mov;
  int hp;
  int curhp;
  int mp;
  int curmp;
  int speed;
  
  int ability1;
  int ability2;
  int atkRange;
  
  int turnCounter;
  int x;  //this refers to x and y location on the board
  int y;
  int animating;  //used to block input/animate while moving
  
public:
  Unit(std::string nClass, std::string nName, int nPlayer, int nX, int nY);
  Unit(const Unit& other);
  ~Unit();
  Unit& operator=(const Unit& other);
  bool operator==(const Unit& other);
  //common game object functions
  void draw() const;
  void update(Uint32 ticks);
  //getters & setters
  std::string getName(){return name;}
  std::string getCharacterClass(){return characterClass;}
  int getAtk(){return atk;}
  int getDef(){return def;}
  int getHp(){return hp;}
  int getCurhp(){return curhp;}
  int getMp(){return mp;}
  int getCurmp(){return curmp;}
  int getSpeed(){return speed;}
  void setCurhp(int nCurhp){curhp = nCurhp;}
  int getX(){return x;}
  int getY(){return y;}
  int getMov(){return mov;}
  int getPlayer(){return player;}
  MultiSprite* getSprite();
  int getPrimary() const {return ability1;}
  int getSecondary() const {return ability2;}
  int getAnimating() const {return animating;}
  void setAtk(int nAtk){ atk = nAtk;}
  void setDef(int nDef){ def = nDef;}
  void setFacing(int d){ sprite->setVariation(d);}
  void setX(int nX){ x = nX; sprite->setX(nX * 16);}
  void setY(int nY){y = nY;sprite->setY(nY * 16);}
  void incrementTurn(){turnCounter += speed;}
  void resetTurn(){turnCounter = 0;}
  int getTurnCounter(){return turnCounter;}

  
  //common commands
  void act(int action_id, Unit* target);
  int battleCalculations(int action_id, Unit* target, int* additionalEffect);
  int attack(Unit* target); //returns remaining health
  void move(direction d);
  int actionTargetType(int action_id);
  int actionRange(int action_id);
  bool canTarget(int targetType, Unit* target);
  void deathAnimation();
  
  //Will be implemented in the last project
  //  Warrior
  int shout();
  int smash(Unit* target);
  //  Mage
  int fireball(Unit* target);
  int blizzard(Unit* target);
  //  Archer
  int trap(Unit* target);
  int aim(Unit* target);
  //  Paladin
  int lightning(Unit* target);
  int heal(Unit* target);
};
#endif
