#include "combatAi.h"

CombatAi::CombatAi(std::vector<Unit*>& nGameObjects, std::list<std::pair<int, int>>& nMoveList) :
      gameObjects(nGameObjects), moveList(nMoveList),
      actionQueue(), usingAi(true){

}
CombatAi::CombatAi(const CombatAi& other) : gameObjects(other.gameObjects), moveList(other.moveList), actionQueue(other.actionQueue), usingAi(other.usingAi)
{}
CombatAi& CombatAi::operator=(const CombatAi& rhs){
  actionQueue = (rhs.actionQueue);
  usingAi = (rhs.usingAi);
  return *this;
}
CombatAi::~CombatAi(){

}
void CombatAi::buildActionQueue(int currentUnit){
  //Find enemy to move towards and attack
  int enemyUnit;
  enemyUnit = findClosestEnemy(currentUnit);
  
  //Find path to enemy from moveTiles
  int endX = gameObjects[enemyUnit]->getX();
  int endY = gameObjects[enemyUnit]->getY();
  
  findClosestTile(currentUnit, enemyUnit, &endX, &endY);//sets the endX & endY

  findPath(currentUnit, endX, endY);
  
  //If within range of an enemy, select attack
  if(canAttack(currentUnit, enemyUnit, endX, endY)){
    //Select target
    selectAttack(enemyUnit, endX, endY);
  }else{  //Else defend
    selectDefend();
  }
  
}
int CombatAi::findClosestEnemy(int currentUnit){
  int closestDist = 128;
  int closestUnit = 0;
  
  int size = gameObjects.size();
  for(int i = 0; i < size; i++){
    if(gameObjects[currentUnit]->getPlayer() != gameObjects[i]->getPlayer()){
      int d = abs(gameObjects[currentUnit]->getX() - gameObjects[i]->getX()) +
            abs(gameObjects[currentUnit]->getY() - gameObjects[i]->getY());
      if(d < closestDist){
        closestDist = d;
        closestUnit = i;
      }
    }
  }
  
  return closestUnit;
}
void CombatAi::findClosestTile(int currentUnit, int enemyUnit, int* endX, int* endY){
  int shortestDist = 128;

  for(auto it : moveList){
    if(shortestDist > abs(it.first - gameObjects[enemyUnit]->getX()) +
          abs(it.second - gameObjects[enemyUnit]->getY())){
      bool occupied = false;
      for(auto itoo : gameObjects){
        if(itoo->getX() == it.first && itoo->getY() == it.second)
          occupied = true;
      }
      if(!occupied){
        shortestDist = abs(it.first - gameObjects[enemyUnit]->getX()) +
          abs(it.second - gameObjects[enemyUnit]->getY());
        *endX = it.first;
        *endY = it.second;
        
      }
    }
  }
  
}
void CombatAi::findPath(int currentUnit, int endX, int endY){
  int startX = gameObjects[currentUnit]->getX();
  int startY = gameObjects[currentUnit]->getY();
  
  //A* search
  auto cmp = [](auto lhs, auto rhs){return lhs.first > rhs.first;};
  std::priority_queue<std::pair<int, std::pair<int, int>>,
        std::vector<std::pair<int, std::pair<int, int>>>,
        decltype(cmp)> myPriorityQ(cmp);
  
  std::map<std::pair<int, int>, int> costSoFar;
  std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
  
  
  myPriorityQ.push(std::make_pair(0, std::make_pair(startX, startY)));
  cameFrom[myPriorityQ.top().second] = std::make_pair(startX, startY);
  costSoFar[myPriorityQ.top().second] = 0;
  
  std::pair<int, std::pair<int, int>> node;
  while(!myPriorityQ.empty()){
    node = myPriorityQ.top();
    myPriorityQ.pop();
    
    if(node.second.first == endX && node.second.second == endY)
      break; //constuctPath();
  
    //make neighbors
    std::vector<std::pair<int, int>> neighbors;
    
    for(auto it : moveList){
      if(node.second.first+1 == it.first && node.second.second == it.second)
        neighbors.push_back(std::make_pair(it.first, it.second));
      if(node.second.first-1 == it.first && node.second.second == it.second)
        neighbors.push_back(std::make_pair(it.first, it.second));
      if(node.second.first == it.first && node.second.second+1 == it.second)
        neighbors.push_back(std::make_pair(it.first, it.second));
      if(node.second.first == it.first && node.second.second-1 == it.second)
        neighbors.push_back(std::make_pair(it.first, it.second));
    }
    
    
    for(auto next : neighbors){
      int newCost = costSoFar[node.second] + 1;
      if(costSoFar.find(next) == costSoFar.end() || newCost < costSoFar[next]){
        costSoFar[next] = newCost;
        int priority = newCost + (abs(endX - next.first) + abs(endY - next.second));
        myPriorityQ.push(std::make_pair(priority, next));
        cameFrom[next] = node.second;
      }
    }
  }
  
  //
  std::vector<int> thePath;
  std::pair<int, int> current = std::make_pair(endX, endY);
  while(current.first != startX || current.second != startY){
    if(cameFrom[current].second < current.second){       //up (opposite of down
      thePath.push_back(SDL_SCANCODE_S);
    }else if(cameFrom[current].second > current.second){ //down (opposite of up)
      thePath.push_back(SDL_SCANCODE_W);
    }else if(cameFrom[current].first < current.first){  //right (opposite of left)
      thePath.push_back(SDL_SCANCODE_D);
    }else if(cameFrom[current].first > current.first){  //left (opposite of right)
      thePath.push_back(SDL_SCANCODE_A);
    }
    current = cameFrom[current];
    
  }
  
  //
  while(!thePath.empty()){
    int queueMe = thePath.back();
    thePath.erase(thePath.begin() + thePath.size() - 1);
    actionQueue.push(queueMe);
  }
  actionQueue.push(SDL_SCANCODE_J); //select here to stay
}

bool CombatAi::canAttack(int currentUnit, int enemyUnit, int endX, int endY){
  int range = gameObjects[currentUnit]->actionRange(ACTION_ID::ATTACK);
  return 0 <= range - abs((gameObjects[enemyUnit]->getX() +
        gameObjects[enemyUnit]->getY()) - (endX + endY));
}
void CombatAi::selectAttack(int enemyUnit, int endX, int endY){
  actionQueue.push(SDL_SCANCODE_W); //move action menu to attack
  actionQueue.push(SDL_SCANCODE_J); //select attack
  
  //move cursor to the target
  while((endX != gameObjects[enemyUnit]->getX()) ||
        (endY != gameObjects[enemyUnit]->getY())){
    if(endX > gameObjects[enemyUnit]->getX()){
      actionQueue.push(SDL_SCANCODE_A); //move left
      endX--;
    }
    else if(endX < gameObjects[enemyUnit]->getX()){
      actionQueue.push(SDL_SCANCODE_D); //move right
      endX++;
    }
    if(endY > gameObjects[enemyUnit]->getY()){
      actionQueue.push(SDL_SCANCODE_W); //move up
      endY--;
    }
    else if(endY < gameObjects[enemyUnit]->getY()){
      actionQueue.push(SDL_SCANCODE_S); //move down
      endY++;
    }
  }
  
  actionQueue.push(SDL_SCANCODE_J); //select enemy
}
void CombatAi::selectDefend(){
  actionQueue.push(SDL_SCANCODE_S); //move action menu to defend
  actionQueue.push(SDL_SCANCODE_J); //select defend
}
int CombatAi::getAction(int currentUnit){
  if(actionQueue.empty())
    buildActionQueue(currentUnit);
  int scancode = actionQueue.front();
  actionQueue.pop();
  return scancode;
}
void CombatAi::resetActions(){
  while(!actionQueue.empty())
    actionQueue.pop();
}
bool CombatAi::getUsingAi(){
  return usingAi;
}
void CombatAi::toggleAi(){
  usingAi = !usingAi;
}
