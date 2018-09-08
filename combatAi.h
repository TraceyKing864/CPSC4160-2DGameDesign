#ifndef combatAi_h
#define combatAi_h

#include <iostream>
#include <list>
#include <queue>
#include "board.h"
#include "unit.h"

class CombatAi{
private:
  std::vector<Unit*>& gameObjects;
  std::list<std::pair<int, int>>& moveList;
  std::queue<int> actionQueue; //this holds all the key presses
  bool usingAi;
  
  void buildActionQueue(int currentUnit);
  int findClosestEnemy(int currentUnit);
  void findClosestTile(int currentUnit, int enemyUnit, int* endX, int* endY);
  void findPath(int currentUnit, int endX, int endY);
  bool canAttack(int currentUnit, int enemyUnit, int endX, int endY);
  void selectAttack(int enemyUnit, int endX, int endY);
  void selectDefend();
  
public:
  CombatAi(std::vector<Unit*>& nGameObjects, std::list<std::pair<int, int>>& nMoveList);
  ~CombatAi();
  CombatAi(const CombatAi& other);
  CombatAi& operator=(const CombatAi& rhs);
  
  int getAction(int currentUnit);
  void resetActions();
  bool getUsingAi();
  void toggleAi();
};


#endif
