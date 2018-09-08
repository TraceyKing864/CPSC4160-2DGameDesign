#include "ioMod.h"
#include "vector2f.h"
#include "renderContext.h"

/* Provided code for GoF singleton
ImageFactory* ImageFactory::getInstance() {
  if ( !instance ) instance = new ImageFactory;
  return instance;
}
*/
// My code for Meyer's singleton pattern
ImageFactory& ImageFactory::getInstance(){
  static ImageFactory instance;
  return instance;
}
//

ImageFactory::~ImageFactory() {
  std::cout << "Deleting images in Factory" << std::endl;
  
  /* Original code provided:
  // Free single image containers
  for(auto& si : surfaces) SDL_FreeSurface(si.second);
  for(auto& ti : textures) SDL_DestroyTexture(ti.second);
  for(auto& fi : images  ) {
    std::cout << "deleting " << fi.first << std::endl;
    delete fi.second;
  }
  */
  
  // My code with while loops without the use of auto
  std::map<std::string, SDL_Surface*>::iterator si = surfaces.begin();
  while(si != surfaces.end()){
    SDL_FreeSurface(si->second);
    si++;
  }
  std::map<std::string, SDL_Texture*>::iterator ti = textures.begin();
  while(ti != textures.end()){
    SDL_DestroyTexture(ti->second);
    ti++;
  }
  std::map<std::string, Image*>::iterator fi = images.begin();
  while(fi != images.end()){
    std::cout << "Deleting " << fi->first << std::endl;
    delete fi->second;
    fi++;
  }
  //

  // Free multi-image containers
  for ( auto& surfaces : multiSurfaces ) {
    for (unsigned int i = 0; i < surfaces.second.size(); ++i) {
      SDL_FreeSurface( surfaces.second[i] );
    }
  }
  for ( auto& textures : multiTextures ) {
    for (unsigned int i = 0; i < textures.second.size(); ++i) {
      SDL_DestroyTexture( textures.second[i] );
    }
  }

  for ( auto& images : multiImages ) {
    std::cout << "Deleting " << images.first << std::endl;
    for (unsigned int i = 0; i < images.second.size(); ++i) {
      delete images.second[i];
    }
  }
}

Image* ImageFactory::getImage(const std::string& name) {
    std::map<std::string, Image*>::const_iterator it = images.find(name); 
  if ( it == images.end() ) {
    SDL_Surface * const surface =
      IoMod::getInstance().readSurface( gdata.getXmlStr(name+"/file"));
    bool transparency = gdata.getXmlBool(name+"/transparency");
    if ( transparency ) {
      int keyColor = SDL_MapRGBA(surface->format, 255, 0, 255, 255);
      SDL_SetColorKey(surface, SDL_TRUE, keyColor);
    }
    surfaces[name] = surface;
    Image * const image =new Image(surface);
    images[name] = image;
    return image;
  }
  else {
    return it->second;
  }
}

std::vector<Image*> ImageFactory::getImages(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Image*> >::const_iterator 
    pos = multiImages.find(name); 
  if ( pos != multiImages.end() ) {
    return pos->second;
  }

  IoMod& iomod = IoMod::getInstance();
  RenderContext* renderContext  = RenderContext::getInstance();
  std::string sheetPath = gdata.getXmlStr(name+"/file");
  SDL_Surface* spriteSurface = iomod.readSurface(sheetPath);
  
  bool transparency;
  if(gdata.checkTag(name+"/transparency"))
    transparency = gdata.getXmlBool(name+"/transparency");
  else
    transparency = gdata.getXmlBool("sprite_default/transparency");
  

  // It wasn't in the map, so we have to make the vector of Images:
  unsigned cols;
  if(gdata.checkTag(name+"/frames"))
    cols = gdata.getXmlInt(name+"/frames");
  else if(gdata.checkTag(name+"/cols"))
    cols = gdata.getXmlInt(name+"/cols");
  else
    cols = spriteSurface->w / gdata.getXmlInt("sprite_default/imageWidth");
  
  unsigned rows;
  if(gdata.checkTag(name+"/variations"))
    rows = gdata.getXmlInt(name+"/variations");
  else if(gdata.checkTag(name+"/rows"))
    rows = gdata.getXmlInt(name+"/rows");
  else
    rows = spriteSurface->h / gdata.getXmlInt("sprite_default/imageHeight");
  
  int width;
  int height;

  if(  gdata.checkTag(name+"/imageWidth") 
    && gdata.checkTag(name+"/imageHeight") ){
    width  = gdata.getXmlInt(name+"/imageWidth");
    height = gdata.getXmlInt(name+"/imageHeight");
  }else{
    width = gdata.getXmlInt("sprite_default/imageWidth");
    height = gdata.getXmlInt("sprite_default/imageHeight");
  }

  std::vector<Image*> images;
  std::vector<SDL_Surface*> surfaces;
  std::vector<SDL_Texture*> textures;
  images.reserve(cols * rows);
  surfaces.reserve(cols * rows);
  textures.reserve(cols * rows);

  SpriteSheet sheet(spriteSurface,width,height);

  for (unsigned j = 0; j < rows; ++j){
    for (unsigned i = 0; i < cols; ++i) {
      SDL_Surface* surface = sheet[i + (j*cols)];
      if ( transparency ) {
        int keyColor = SDL_MapRGBA(spriteSurface->format, 255, 0, 255, 255);
        SDL_SetColorKey(surface, SDL_TRUE, keyColor);
      }
      SDL_Texture* texture =
        SDL_CreateTextureFromSurface(renderContext->getRenderer(),surface);
      surfaces.push_back( surface );
      textures.push_back( texture );
      images.push_back( new Image(surface) );
    }
  }
  
  multiSurfaces[name] = surfaces;
  multiTextures[name] = textures;
  multiImages[name] = images;
  return images;
}
