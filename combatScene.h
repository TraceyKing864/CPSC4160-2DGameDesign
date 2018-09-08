#ifndef COMBATSCENE__H
#define COMBATSCENE__H

#include <iostream>
#include <vector>
#include <list>
#include "scene.h"
#include "board.h"
#include "unit.h"
#include "cursor.h"
#include "combatAi.h"

enum SCENE_STATE{
  SELECT_UNIT = 0,
  MOVE_UNIT = 1,
  SELECT_ACTION = 2,
  SELECT_TARGET = 3,
  PERFORM_ACTION = 4,
  END_TURN = 5,
  GAME_OVER = 6
};

class CombatScene : public Scene {
private:
  Board board;
  std::vector<Unit*> gameObjects;
  std::vector<Unit*> deadUnits;
  
  int curPlayer;
  int currentUnit;  //this is the unit being controlled by player
  int action;
  int target;
  Cursor* cursor;
  
  //these are used to check the range of movement
  //  and attack. Possible tiles are here
  std::list<std::pair<int, int>> moveList;
  std::list<std::pair<int, int>> atkList;
  MultiSprite* moveTile;
  MultiSprite* atkTile;
  
  CombatAi* combatAi;
  
  int inputScancode;
  int sceneState;
  bool reset;
  bool MALLOYMODE;
  
  void recursiveMov(int x, int y, int d);
  void recursiveAtk(int x, int y, int d);
  
  int aiTurn(Uint32 ticks); //returns the AI input
  
  void trackObject(Unit& obj);
  void trackObject(Cursor& obj);
  bool checkCollision(Unit& obj, direction d); //true if no collision
  void setCursor(int inX, int inY);
  void resetCursor();
  
  void move(direction d); //moves current unit
  void moveCursor(direction d);
  
  bool select();    //returns success
  bool deselect();  //returns success
  void calculateMovTiles();
  void calculateAtkTiles();
  
  bool openActionMenu();
  void closeActionMenu();
  void navigateActionMenu(direction d);
  int selectAction();
  void cancelActionSelect();
  
  bool selectTarget();
  void cancelTargetSelect();
  
  void act(); //tells the currentUnit to perform action
  int battleCalculations(int* additionalEffect);
  
  void nextTurn();
  void setUnitInfo();
  int winCondition();
  
  void toggleMALLOYMODE();
  void ENTERMALLOYMODE();
  void exitmalloymode();
public:
  CombatScene(std::string name);
  virtual ~CombatScene();
  CombatScene(const CombatScene& other);
  CombatScene& operator=(const CombatScene& other);
  
  virtual void input(int scancode);
  virtual void update(Uint32 ticks);
  virtual void draw();
  int getPlayer(){return curPlayer;}
  virtual bool resetMe(){return reset;}
};
#endif
