#include <sstream>
#include "combatScene.h"
#include "gamedata.h"
#include "viewport.h"
#include "clock.h"
#include "sound.h"

CombatScene::CombatScene(std::string name) : Scene(name),
      board(Gamedata::getInstance().getXmlStr(name+"/board/file"),
            Gamedata::getInstance().getXmlInt(name+"/board/width"),
            Gamedata::getInstance().getXmlInt(name+"/board/height")),
      gameObjects(), deadUnits(), curPlayer(), currentUnit(),
      action(0), target(0), cursor(),
      moveList(), atkList(), moveTile(new MultiSprite("range_blue", 0, 0)),
      atkTile(new MultiSprite("range_red", 0, 0)),
      combatAi(new CombatAi(gameObjects, moveList)),
      sceneState(0), inputScancode(0), reset(false), MALLOYMODE(false){
  //Load Game Objects
  std::string temp;
  for(int i = 0; i < Gamedata::getInstance().getXmlInt(name+"/units_player/total"); i++){
    temp = name+"/units_player/unit_"+(std::stringstream() << i).str();
    gameObjects.emplace_back(new Unit(
        Gamedata::getInstance().getXmlStr(temp+"/class"),
        Gamedata::getInstance().getXmlStr(temp+"/name"), 0,
        Gamedata::getInstance().getXmlInt(temp+"/x"),
        Gamedata::getInstance().getXmlInt(temp+"/y")));
  }
  for(int i = 0; i < Gamedata::getInstance().getXmlInt(name+"/units_ai/total"); i++){
    temp = name+"/units_ai/unit_"+(std::stringstream() << i).str();
    gameObjects.emplace_back(new Unit(
        Gamedata::getInstance().getXmlStr(temp+"/class"),
        Gamedata::getInstance().getXmlStr(temp+"/name"), 1,
        Gamedata::getInstance().getXmlInt(temp+"/x"),
        Gamedata::getInstance().getXmlInt(temp+"/y")));
  }
  cursor = new Cursor("cursor", gameObjects[currentUnit]->getX(),
      gameObjects[currentUnit]->getY());
  
  Viewport::getInstance().setObjectToTrack(cursor->getSprite());
  calculateMovTiles();
  resetCursor();
  select();
  sceneState = SCENE_STATE::MOVE_UNIT;
  curPlayer = gameObjects[currentUnit]->getPlayer();
  Viewport::getInstance().setPlayer(curPlayer);
}
CombatScene::~CombatScene(){
  if(!gameObjects.empty()){
    for(std::vector<Unit*>::iterator it = gameObjects.begin();
          *it != gameObjects.back(); it++){
      delete *it;
    }
    gameObjects.clear();
  }
  
  if(!deadUnits.empty()){
    for(std::vector<Unit*>::iterator it = deadUnits.begin();
          *it != deadUnits.back(); it++){
      delete *it;
    }
    deadUnits.clear();
  }
  
  delete cursor;
  delete moveTile;
  delete atkTile;
  delete combatAi;
}
CombatScene::CombatScene(const CombatScene& other) : Scene(other),
      board(other.board),
      gameObjects(other.gameObjects), deadUnits(other.deadUnits),
      curPlayer(other.curPlayer), currentUnit(other.currentUnit),
      action(other.action), target(other.target),
      cursor(other.cursor), moveList(other.moveList), atkList(other.atkList),
      moveTile(other.moveTile), atkTile(other.atkTile),
      combatAi(other.combatAi),
      sceneState(other.sceneState),
      inputScancode(other.inputScancode),
      reset(other.reset),
      MALLOYMODE(other.MALLOYMODE)
{}
CombatScene& CombatScene::operator=(const CombatScene& other){
  Scene::operator=( other );
  board = other.board;
  gameObjects = other.gameObjects;
  deadUnits = other.deadUnits;
  curPlayer = other.curPlayer;
  currentUnit = other.currentUnit;
  action = other.action;
  target = other.target;
  cursor = other.cursor;
  moveList = other.moveList;
  atkList = other.atkList;
  moveTile = other.moveTile;
  atkTile = other.atkTile;
  combatAi = other.combatAi;
  sceneState = other.sceneState;
  inputScancode = other.inputScancode;
  reset = other.reset;
  MALLOYMODE = other.MALLOYMODE;
  return *this;
}
void CombatScene::input(int scancode){
  inputScancode = scancode;
}
void CombatScene::update(Uint32 ticks){
  //catch input for any scene
  if(inputScancode == SDL_SCANCODE_I)
    combatAi->toggleAi();
  if(inputScancode == SDL_SCANCODE_M)
    toggleMALLOYMODE();
  //
  
  
  //if AI turn then AI will determine input and return
  if(curPlayer == 1 && combatAi->getUsingAi())
    inputScancode = aiTurn(ticks);
  //
  
  switch(sceneState){
    case SCENE_STATE::SELECT_UNIT:     //select unit
      if(cursor->getAnimating()){
        //if animating, do nothing, let it update
      }else if(inputScancode == SDL_SCANCODE_J){
        if(select()){  //if piece is valid
          sceneState = SCENE_STATE::MOVE_UNIT;
        }
      }else if(inputScancode == SDL_SCANCODE_K){
        resetCursor();
      }else if (inputScancode == SDL_SCANCODE_W){
        moveCursor(direction::up);
      }else if (inputScancode == SDL_SCANCODE_A){
        moveCursor(direction::left);
      }else if (inputScancode == SDL_SCANCODE_S){
        moveCursor(direction::down);
      }else if (inputScancode == SDL_SCANCODE_D){
        moveCursor(direction::right);
      }
      break;
      
    case SCENE_STATE::MOVE_UNIT:     //move unit
      if(cursor->getAnimating()){
        //if animating, do nothing, let it update
      }else if(inputScancode == SDL_SCANCODE_K){
        if(deselect())
          sceneState = SCENE_STATE::SELECT_UNIT;
      }else if(inputScancode == SDL_SCANCODE_J){
        if(openActionMenu()){
          sceneState = SCENE_STATE::SELECT_ACTION;
        }
      }else if (inputScancode == SDL_SCANCODE_W){
        move(direction::up);
      }else if (inputScancode == SDL_SCANCODE_A){
        move(direction::left);
      }else if (inputScancode == SDL_SCANCODE_S){
        move(direction::down);
      }else if (inputScancode == SDL_SCANCODE_D){
        move(direction::right);
      }
      break;
      
    case SCENE_STATE::SELECT_ACTION:     //select action
      if(inputScancode == SDL_SCANCODE_J){
        action = selectAction();
        if(action != 0){
          closeActionMenu();
          
          if(action == 2)   //if action is defend, end turn
            sceneState = SCENE_STATE::END_TURN;
          else if(action != 0){
            calculateAtkTiles();
            resetCursor();
            cursor->show();
            trackObject(*cursor);
            sceneState = SCENE_STATE::SELECT_TARGET;
          }
        }
      }else if(inputScancode == SDL_SCANCODE_K){
        cancelActionSelect();
        sceneState = SCENE_STATE::MOVE_UNIT;
      }else if (inputScancode == SDL_SCANCODE_W){
        navigateActionMenu(direction::up);
      }else if (inputScancode == SDL_SCANCODE_A){
        navigateActionMenu(direction::left);
      }else if (inputScancode == SDL_SCANCODE_S){
        navigateActionMenu(direction::down);
      }else if (inputScancode == SDL_SCANCODE_D){
        navigateActionMenu(direction::right);
      }
      break;
      
    case SCENE_STATE::SELECT_TARGET:     //select target
      if(cursor->getAnimating()){
        //if animating, do nothing, let it update
      }else if(inputScancode == SDL_SCANCODE_J){
        if(selectTarget()){
          act();
          sceneState = SCENE_STATE::PERFORM_ACTION;
        }
      }else if(inputScancode == SDL_SCANCODE_K){
        cancelTargetSelect();
        sceneState = SCENE_STATE::SELECT_ACTION;
      }else if (inputScancode == SDL_SCANCODE_W){
        moveCursor(direction::up);
      }else if (inputScancode == SDL_SCANCODE_A){
        moveCursor(direction::left);
      }else if (inputScancode == SDL_SCANCODE_S){
        moveCursor(direction::down);
      }else if (inputScancode == SDL_SCANCODE_D){
        moveCursor(direction::right);
      }
      break;
      
    case SCENE_STATE::PERFORM_ACTION://perform action
      if(!gameObjects[currentUnit]->getAnimating()){
        int additionalEffect = -1;
        int remainingHealth = battleCalculations(&additionalEffect);
        if(additionalEffect >= 0){
          
        }
        if(remainingHealth == 0){
          gameObjects[target]->deathAnimation();
          SDLSound::getInstance()[0];
        }
        sceneState = END_TURN;
      }
      break;
    case SCENE_STATE::END_TURN://sets up the next turn
      if(!gameObjects[target]->getAnimating()){
        if(gameObjects[target]->getCurhp() == 0){
          deadUnits.emplace_back(gameObjects.at(target));
          gameObjects.erase(gameObjects.begin()+target);
        }
        
        nextTurn();
        select();
        sceneState = SCENE_STATE::MOVE_UNIT;
        
        if(winCondition()){ //current win condition for game
          sceneState = SCENE_STATE::GAME_OVER;
          Viewport::getInstance().setWinCond(winCondition());
          Viewport::getInstance().toggleHud("game_over");
        }
      }
      break;
    case SCENE_STATE::GAME_OVER:
      //should either quit or reset here
      if(inputScancode == SDL_SCANCODE_R){
        Viewport::getInstance().toggleHud("game_over");
        reset = true;
      }
      break;
  }
  inputScancode = 0;

  board.update(ticks);
  
  for(auto& it : gameObjects)
    it->update(ticks);
  cursor->update(ticks);
  if(sceneState == SCENE_STATE::MOVE_UNIT)
    moveTile->update(ticks);
  if(sceneState == SCENE_STATE::SELECT_TARGET)
    atkTile->update(ticks);
}
void CombatScene::draw(){
  board.draw();
  if(sceneState == SCENE_STATE::MOVE_UNIT){ //if moving, show movetiles
    for(auto& it : moveList){
      moveTile->setX(it.first * 16);
      moveTile->setY(it.second * 16);
      moveTile->draw();
    }
  }
  if(sceneState == SCENE_STATE::SELECT_TARGET){ //if moving, show movetiles
    for(auto& it : atkList){
      atkTile->setX(it.first * 16);
      atkTile->setY(it.second * 16);
      atkTile->draw();
    }
  }
  for(auto& it: gameObjects)
    it->draw();
  gameObjects[currentUnit]->draw();//this is dumb but it puts the animation above other units
  cursor->draw();
}
int CombatScene::aiTurn(Uint32 ticks){
  const float updateInterval = 200;
  static float timeSinceUpdate = 0;
  timeSinceUpdate += ticks;
  if (timeSinceUpdate > updateInterval) {
    timeSinceUpdate = 0;
    return combatAi->getAction(currentUnit);
  }else{
    return 0;
  }
}
void CombatScene::trackObject(Unit& obj){
  Viewport::getInstance().setObjectToTrack(obj.getSprite());
}
void CombatScene::trackObject(Cursor& obj){
  Viewport::getInstance().setObjectToTrack(obj.getSprite());
}
bool CombatScene::checkCollision(Unit& obj, direction d){
  int x = obj.getX();
  int y = obj.getY();
  if(d == direction::up) y--;
  else if(d == direction::down) y++;
  else if(d == direction::left) x--;
  else if(d == direction::right) x++;
  
  for(auto& it : moveList){
    if(x == it.first && y == it.second)
      return true;  //in movement list
  }
  return false;
}

void CombatScene::setCursor(int inX, int inY){
  cursor->setX(inX);
  cursor->setY(inY);
  setUnitInfo();
}
void CombatScene::resetCursor(){
  cursor->setX(gameObjects[currentUnit]->getX());
  cursor->setY(gameObjects[currentUnit]->getY());
  setUnitInfo();
}

void CombatScene::move(direction d){
  if(checkCollision(*(gameObjects[currentUnit]), d))
    gameObjects[currentUnit]->move(d);
  else
    gameObjects[currentUnit]->setFacing(d);
}
void CombatScene::moveCursor(direction d){
  int x = cursor->getX();
  int y = cursor->getY();
  if(d == direction::up) y--;
  else if(d == direction::down) y++;
  else if(d == direction::left) x--;
  else if(d == direction::right) x++;
  
  //Check board bounds
  if(!(x >= board.getWidth() || y >= board.getHeight() || x < 0 || y < 0)){
    cursor->move(d);
    setUnitInfo();
  }
  
}
bool CombatScene::select(){
  if(cursor->getX() == gameObjects[currentUnit]->getX() &&
        cursor->getY() == gameObjects[currentUnit]->getY()){
    cursor->hide();
    trackObject(*gameObjects[currentUnit]);
    return true;
  }
  return false;
}
bool CombatScene::deselect(){
  int x = gameObjects[currentUnit]->getX();
  int y = gameObjects[currentUnit]->getY();
  for(auto& it : gameObjects){  //check if there's no other unit there
    if(x == it->getX() && y == it->getY() && it != gameObjects[currentUnit])
      return false;
  }
  setCursor(x, y);
  cursor->show();
  trackObject(*cursor);
  return true;
}
void CombatScene::calculateMovTiles(){

  moveList.clear();
  
  recursiveMov(gameObjects[currentUnit]->getX(),
        gameObjects[currentUnit]->getY(),
        gameObjects[currentUnit]->getMov());
  
}
void CombatScene::recursiveMov(int x, int y, int d){
  bool inList;
  inList = false;
  for(auto& it : moveList){
    if(it.first == x && it.second == y)
      inList = true;
  }

  bool inTheWay;
  inTheWay = false;
  for(auto& it : gameObjects){
    if((it->getX() == x) && (it->getY() == y)){
      if(gameObjects[currentUnit]->getPlayer() != it->getPlayer())
        inTheWay = true;
    }
  }
  if((board.getCollision(x, y) > 0) && !inTheWay){
    if(!inList)
      moveList.emplace_back(std::make_pair(x, y));
    if(d > 0){
      recursiveMov(x+1, y, d-1);
      recursiveMov(x-1, y, d-1);
      recursiveMov(x, y-1, d-1);
      recursiveMov(x, y+1, d-1);
    }
  }
  
}
void CombatScene::calculateAtkTiles(){
  atkList.clear();
  
  recursiveAtk(gameObjects[currentUnit]->getX(),
        gameObjects[currentUnit]->getY(),
        gameObjects[currentUnit]->actionRange(action));
}
void CombatScene::recursiveAtk(int x, int y, int d){
  bool inList = false;
  for(auto& it : atkList){
    if(it.first == x && it.second == y)
      inList = true;
  }
  if(!inList)
    atkList.emplace_back(std::make_pair(x, y));
  if(d > 0){
    d--;
    recursiveAtk(x-1, y, d);
    recursiveAtk(x+1, y, d);
    recursiveAtk(x, y-1, d);
    recursiveAtk(x, y+1, d);
  }
}
bool CombatScene::openActionMenu(){
  int x = gameObjects[currentUnit]->getX();
  int y = gameObjects[currentUnit]->getY();
  for(auto& it : gameObjects){  //check if there's no other unit there
    if(x == it->getX() && y == it->getY() && it != gameObjects[currentUnit])
      return false;
  }
  Viewport::getInstance().openActionMenu(*gameObjects[currentUnit]);
  return true;
}
void CombatScene::closeActionMenu(){
  Viewport::getInstance().closeActionMenu();
}
void CombatScene::navigateActionMenu(direction d){
  Viewport::getInstance().navigateActionMenu(d);
}
int CombatScene::selectAction(){
  return Viewport::getInstance().selectActionMenu();
}
void CombatScene::cancelActionSelect(){
  Viewport::getInstance().closeActionMenu();
}

bool CombatScene::selectTarget(){
  bool inList = false;
  for(auto& it : atkList){
    if(cursor->getX() == it.first && cursor->getY() == it.second)
      inList = true;
  }
  if(inList){
    bool canTarget = false;
    int targetType = gameObjects[currentUnit]->actionTargetType(action);
    bool targetingEmptyTile = true;
    for(int i = 0; i < gameObjects.size(); i++){
      if(cursor->getX() == gameObjects[i]->getX() && cursor->getY() == gameObjects[i]->getY()){
        canTarget = gameObjects[currentUnit]->canTarget(targetType, gameObjects[i]);
        targetingEmptyTile = false;
        if(canTarget){
          target = i;
        }
      }
    }
    if(targetingEmptyTile)
      canTarget = gameObjects[currentUnit]->canTarget(targetType, NULL);
    
    return canTarget;
  }
  return false;
}
void CombatScene::cancelTargetSelect(){
  cursor->hide();
  trackObject(*gameObjects[currentUnit]);
  Viewport::getInstance().openActionMenu(*gameObjects[currentUnit]);
}

void CombatScene::act(){
  cursor->hide();
  gameObjects[currentUnit]->act(action, gameObjects[target]);
}

int CombatScene::battleCalculations(int* additionalEffect){
  return gameObjects[currentUnit]->battleCalculations(action, gameObjects[target], additionalEffect);
}

void CombatScene::nextTurn(){
  gameObjects[currentUnit]->setFacing(direction::down);
  for(auto it : gameObjects)
    it->incrementTurn();
  gameObjects[currentUnit]->resetTurn();
  for(int i = 0; i < gameObjects.size(); i++){
    if(gameObjects[currentUnit]->getTurnCounter() <
        gameObjects[i]->getTurnCounter())
      currentUnit = i;
  }
  resetCursor();
  cursor->show();
  calculateMovTiles();
  trackObject(*gameObjects[currentUnit]);
  curPlayer = gameObjects[currentUnit]->getPlayer();
  Viewport::getInstance().setPlayer(curPlayer);
  combatAi->resetActions();
}
void CombatScene::setUnitInfo(){
  Unit* u = NULL;
  for(auto& it : gameObjects){
    if(cursor->getX() == it->getX() && cursor->getY() == it->getY()){
      u = it;
    }
  }
  Viewport::getInstance().setUnitInfo(u);
}
int CombatScene::winCondition(){
  bool playerAlive = false;
  for(auto& it : gameObjects){
    if(it->getPlayer() == 0)
      playerAlive = true;
  }
  if(!playerAlive)
    return 2; //you lose :(
  
  bool enemyAlive = false;
  for(auto& it : gameObjects){
    if(it->getPlayer() == 1)
      enemyAlive = true;
  }
  if(!enemyAlive)
    return 1; //you win :)
  
  return 0; //condition not reached yet
}
void CombatScene::toggleMALLOYMODE(){
  Viewport::getInstance().toggleHud("MALLOYMODE");
  if(MALLOYMODE){
    exitmalloymode();
  }else{
    ENTERMALLOYMODE();
  }
}
void CombatScene::ENTERMALLOYMODE(){
  for(auto it : gameObjects){
    if(it->getPlayer() == 0){
      it->setAtk(100);
      it->setDef(100);
    }
  }
  MALLOYMODE = true;
}
void CombatScene::exitmalloymode(){
  for(auto it : gameObjects){
    if(it->getPlayer() == 0){
      it->setAtk(Gamedata::getInstance().getXmlInt(it->getCharacterClass() + "/atk"));
      it->setDef(Gamedata::getInstance().getXmlInt(it->getCharacterClass() + "/def"));
    }
  }
  MALLOYMODE = false;
}
