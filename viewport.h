#ifndef VIEWPORT__H
#define VIEWPORT__H
#include "drawable.h"
#include "gamedata.h"
#include "hud.h"
#include "unit.h"

class Viewport {
public:
  static Viewport& getInstance();
  void draw();
  void update(Uint32 ticks);

  Vector2f getPosition() const { return position; }
  float getX() const  { return position[0]; }
  void  setX(float x) { position[0] = x; }
  float getY() const  { return position[1]; }
  void  setY(float y) { position[1] = y; }
  void setWidth(int w) { worldWidth = w; }
  void setHeight(int h) { worldHeight = h; }
  int getPlayer(){ return currentPlayer;}
  void setPlayer(int nPlayer){currentPlayer = nPlayer;}
  Unit* getUnitInfo(){return unitInfo;}
  void setUnitInfo(Unit* nUnitInfo){unitInfo = nUnitInfo;}
  int getWinCond(){return winCond;}
  void setWinCond(int wc){winCond = wc;}
  void setObjectToTrack(const Drawable *obj);
  const Drawable* getObjectToTrack() const { return objectToTrack; } 
  void toggleHud(std::string name);
  void toggleHelp();
  void toggleFps();
  
  void openActionMenu(const Unit& nUnit);
  void closeActionMenu();
  void navigateActionMenu(direction d);
  int selectActionMenu();
  std::string getActionName();
  
  void scale();
  
  
private:
  const Gamedata& gdata;
  Vector2f position;
  int worldWidth;
  int worldHeight;
  int viewWidth;
  int viewHeight;
  int objWidth;
  int objHeight;
  
  //needs to know these from scene
  int currentPlayer;
  Unit* unitInfo;
  int winCond;
  //scene will update these
  
  const Drawable *objectToTrack;
  Hud hud;

  Viewport();
  Viewport(const Viewport&);
  Viewport& operator=(const Viewport&);
};
#endif
