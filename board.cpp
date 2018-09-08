#include <fstream>
#include <sstream>
#include "board.h"
#include "gamedata.h"

Board::Board(std::string fileName, int nWidth, int nHeight) : width(nWidth), height(nHeight),
        tiles(), viewX(0.0), viewY(0.0), view(Viewport::getInstance()) {
  std::ifstream file(fileName);
  if(!file.is_open()){
    std::cout << "Error Board::Board; file not found." << std::endl;
    std::exit(1);
  }
  //read in and initialize map file
  tiles.reserve(width * height);
  for(int j = 0; j < height; j++){
    for(int i = 0; i < width; i++){
      int t;
      file >> t;
      std::string name = "tile_list/tile_"+(std::stringstream() << t).str();
      tiles.push_back(new Tile(Gamedata::getInstance().getXmlStr(name),
            4, //This type is a center tile, hardcoded for now
            (i * 16), (j * 16))); //size of tiles hardcoded for now
    }
  }
  file.close();
  
  Viewport::getInstance().setWidth(width * 16);
  Viewport::getInstance().setHeight(height * 16);
}
Board::Board(const Board& other) : width(other.width), height(other.height), tiles(other.tiles),
      viewX(other.viewX), viewY(other.viewY), view(other.view)
{}
Board::~Board(){
  for(std::vector<Tile*>::iterator it = tiles.begin();
        *it != tiles.back(); it++){
    delete *it;
  }
  tiles.clear();
  Viewport::getInstance().setWidth(0);
  Viewport::getInstance().setHeight(0);
}
Board& Board::operator=(const Board& other){
  width = other.width;
  height = other.height;
  tiles = other.tiles;
  viewX = other.viewX;
  viewY = other.viewY;
  return *this;
}
void Board::draw(){
  for(auto& it : tiles)
    it->draw();
    
}
void Board::update(Uint32 ticks){

}
int Board::getCollision(int z){
  if(z < 0 || z > width * height)
    return 0;
  return tiles[z]->getCollision();
}
int Board::getCollision(int x, int y){
  if(x < 0 || x >= width || y < 0 || y >= height)
    return 0;
  return tiles[(y * width) + x]->getCollision(); //possible error
}
