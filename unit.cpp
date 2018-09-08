#include "unit.h"
#include "gamedata.h"
#include "sound.h"

int addVar(int input){  //adds variation into the mix
  return input + ((rand() % 2 ? 1 : -1) * (rand() % 5 ));
}

Unit::Unit(std::string nClass, std::string nName, int nPlayer, int nX, int nY) :
      characterClass(nClass),
      name(nName),
      sprite(new MultiSprite((nClass), (nX * 16), (nY * 16))),
player(nPlayer), partySprite(new FollowSprite((player == 0 ? "party_blue" : "party_red"), *sprite)), shadow(new FollowSprite("shadow", *sprite)),
      animation(NULL), projectile(NULL), explosion(NULL),
      atk(addVar(Gamedata::getInstance().getXmlInt(characterClass+"/atk"))),
      def(addVar(Gamedata::getInstance().getXmlInt(characterClass+"/def"))),
      mov(Gamedata::getInstance().getXmlInt(characterClass+"/mov")),
      hp(addVar(Gamedata::getInstance().getXmlInt(characterClass+"/hp"))),
      curhp(hp),
      mp(addVar(Gamedata::getInstance().getXmlInt(characterClass+"/mp"))),
      curmp(mp),
      speed(addVar(Gamedata::getInstance().getXmlInt(characterClass+"/speed"))),
      ability1(Gamedata::getInstance().getXmlInt(characterClass+"/ability1")),
      ability2(Gamedata::getInstance().getXmlInt(characterClass+"/ability2")),
      atkRange(Gamedata::getInstance().getXmlInt(characterClass+"/atkRange")),
      turnCounter(0),
      x(nX),
      y(nY),
      animating(0)
{}
Unit::Unit(const Unit& other) : characterClass(other.characterClass),
      name(other.name),
      sprite( new MultiSprite(*other.sprite)),
      player(other.player),
      partySprite( new FollowSprite((player == 0 ? "party_blue" : "party_red"), *sprite)),
      shadow( new FollowSprite("shadow", *sprite)),
      animation(NULL),
      projectile(NULL),
      explosion(NULL),
      atk(other.atk),
      def(other.def),
      mov(other.mov),
      hp(other.hp), curhp(other.curhp),
      mp(other.mp), curmp(other.curmp),
      speed(other.speed),
      ability1(other.ability1),
      ability2(other.ability2),
      atkRange(other.atkRange),
      turnCounter(other.turnCounter),
      x(other.x),
      y(other.y),
      animating(other.animating)
{}
Unit::~Unit(){
  delete sprite;
  delete partySprite;
  delete shadow;
  if(animation)
    delete animation;
  if(projectile)
    delete projectile;
  if(explosion)
    delete explosion;
}
Unit& Unit::operator=(const Unit& other){
  characterClass = other.characterClass;
  name = other.name;
  if(sprite){
    delete sprite;
  }
  sprite = new MultiSprite(*other.sprite);
  player = other.player;
  if(partySprite){
    delete partySprite;
  }
  partySprite = new FollowSprite((player == 0 ? "party_blue" : "party_red"), *sprite);
  if(shadow){
    delete shadow;
  }
  shadow = new FollowSprite("shadow", *sprite);
  if(animation)
    delete animation;
  animation = NULL;
  if(projectile)
    delete projectile;
  projectile = NULL;
  if(explosion)
    delete explosion;
  explosion = NULL;
  atk = other.atk;
  def = other.def;
  mov = other.mov;
  hp = other.hp;
  curhp = other.curhp;
  mp = other.mp;
  curmp = other.curmp;
  speed = other.speed;
  ability1 = other.ability1;
  ability2 = other.ability2;
  atkRange = other.atkRange;
  turnCounter = other.turnCounter;
  x = other.x;
  y = other.y;
  animating = other.animating;
  return *this;
}
bool Unit::operator==(const Unit& other){
  if(characterClass == other.characterClass &&
  name == other.name &&
  sprite == other.sprite &&
  player == other.player &&
  atk == other.atk &&
  def == other.def &&
  mov == other.mov &&
  hp == other.hp &&
  curhp == other.curhp &&
  mp == other.mp &&
  curmp == other.curmp &&
  speed == other.speed &&
  ability1 == other.ability1 &&
  ability2 == other.ability2 &&
  atkRange == other.atkRange &&
  turnCounter == other.turnCounter &&
  x == other.x &&
  y == other.y &&
  animating == other.animating)
    return true;
  else
    return false;
}
void Unit::draw() const{
  if(!explosion){
    shadow->draw();
    sprite->draw();
    partySprite->draw();
    if(animation)
      animation->draw();
    if(projectile)
      projectile->draw();
  }else{
    explosion->draw();
  }
}
void Unit::update(Uint32 ticks){
  //the reason for the animations being handled here is so that you can have
  //  extra effects happening depending on when an animation finishes
  if(animating){
    if(animating == -1){ //walking animation
      if((sprite->getVelocityX() == 0) && (sprite->getVelocityY() == 0)) //if 0 then done animating
        animating = 0;
    }else if(animating == -2){ //animating explosion
      if(explosion->getFinished())
        animating = 0;
    }else if(animating == ACTION_ID::ATTACK){
      if(animation){
        if(animation->isFinished()){
          SDLSound::getInstance()[1];
          animating = 0;
        }
      }
      if(projectile){
        if(projectile->isFinished()){
          SDLSound::getInstance()[1];
          animating = 0;
        }
      }
    }else if(animating == ACTION_ID::SHOUT){
      if(animation){
        if(animation->isFinished()){
          if(animation){
            animation->resetAnimation();
            delete animation;
            animation = NULL;
          }
          if(projectile){
            projectile->resetAnimation();
            delete projectile;
            projectile = NULL;
          }
          animating = 0;
        }
      }
    }else if(animating == ACTION_ID::SMASH){
      if(animation){
        if(animation->isFinished()){
          SDLSound::getInstance()[1];
          if(animation){
            animation->resetAnimation();
            delete animation;
            animation = NULL;
          }
          if(projectile){
            projectile->resetAnimation();
            delete projectile;
            projectile = NULL;
          }
          animating = 0;
        }
      }
    }else if(animating == ACTION_ID::FIREBALL){
      if(projectile){
        if(projectile->isFinished()){
          if(!animation){
            animation = new Animation("animation_fireball",
            (Gamedata::getInstance().getXmlBool("animation_fireball/atTarget") ?
                  projectile->getX() : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_fireball/atTarget") ?
                  projectile->getY() : (y * 16)));
          }
          if(animation->isFinished()){
            SDLSound::getInstance()[1];
            if(animation){
              animation->resetAnimation();
              delete animation;
              animation = NULL;
            }
            if(projectile){
              projectile->resetAnimation();
              delete projectile;
              projectile = NULL;
            }
            animating = 0;
          }
        }
      }
    }else if(animating == ACTION_ID::BLIZZARD){ //animating blizzard
      if(animation){
        if(animation->isFinished()){
          SDLSound::getInstance()[1];
          if(animation){
            animation->resetAnimation();
            delete animation;
            animation = NULL;
          }
          if(projectile){
            projectile->resetAnimation();
            delete projectile;
            projectile = NULL;
          }
          animating = 0;
        }
      }
    }else if(animating == ACTION_ID::TRAP){ //animating trap
      if(animation){
        if(animation->isFinished()){
          SDLSound::getInstance()[1];
          if(animation){
            animation->resetAnimation();
            delete animation;
            animation = NULL;
          }
          if(projectile){
            projectile->resetAnimation();
            delete projectile;
            projectile = NULL;
          }
          animating = 0;
        }
      }
    }else if(animating == ACTION_ID::AIM){ //animating aim
      if(animation){
        if(animation->isFinished()){
          if(!projectile){
            projectile = new Projectile("projectile_arrow",
            (x * 16), (y * 16),
            animation->getX(), animation->getY());
          }
          if(projectile->isFinished()){
            SDLSound::getInstance()[1];
            if(animation){
              animation->resetAnimation();
              delete animation;
              animation = NULL;
            }
            if(projectile){
              projectile->resetAnimation();
              delete projectile;
              projectile = NULL;
            }
            animating = 0;
          }
        }
      }
    }else if(animating == ACTION_ID::HEAL){ //animating heal
      if(animation){
        if(animation->isFinished()){
          if(animation){
              animation->resetAnimation();
              delete animation;
              animation = NULL;
          }
          if(projectile){
            projectile->resetAnimation();
            delete projectile;
            projectile = NULL;
          }
          animating = 0;
        }
      }
    }else if(animating == ACTION_ID::LIGHTNING){ //animating revive
      if(projectile){
        if(projectile->isFinished()){
          if(!animation){
            animation = new Animation("animation_lightning",
            (Gamedata::getInstance().getXmlBool("animation_lightning/atTarget") ?
                  projectile->getX() : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_lightning/atTarget") ?
                  projectile->getY() : (y * 16)));
          }
          if(animation->isFinished()){
            SDLSound::getInstance()[1];
            if(animation){
              animation->resetAnimation();
              delete animation;
              animation = NULL;
            }
            if(projectile){
              projectile->resetAnimation();
              delete projectile;
              projectile = NULL;
            }
            animating = 0;
          }
        }
      }
    }
    //other animations here ie (animating == x){}
  }
  sprite->update(ticks);
  shadow->update(ticks);
  partySprite->update(ticks);
  if(animation)
    animation->update(ticks);
  if(projectile)
    projectile->update(ticks);
  if(explosion)
    explosion->update(ticks);
}
MultiSprite* Unit::getSprite(){
  return sprite;
}

//common commands
void Unit::act(int action_id, Unit* target){
  switch(action_id){
    case ACTION_ID::ATTACK:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      if(Gamedata::getInstance().checkTag(characterClass + "/attackAnimation")){
        animation = new Animation(Gamedata::getInstance().getXmlStr(
              characterClass + "/attackAnimation"),
              (Gamedata::getInstance().getXmlBool(Gamedata::getInstance().getXmlStr(
                  characterClass + "/attackAnimation") + "/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
              (Gamedata::getInstance().getXmlBool(Gamedata::getInstance().getXmlStr(
                  characterClass + "/attackAnimation") + "/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
        if(target->getX() > x)
          animation->setVariation(0);
        else if(target->getX() < x)
          animation->setVariation(1);
        else if(target->getY() > y)
          animation->setVariation(0);
        else
          animation->setVariation(1);
      }
      if(Gamedata::getInstance().checkTag(characterClass + "/attackProjectile")){
        projectile = new Projectile(Gamedata::getInstance().getXmlStr(
              characterClass + "/attackProjectile"), (x * 16), (y * 16),
              (target->getX() * 16), (target->getY() * 16));
      }
      break;
    case ACTION_ID::SHOUT:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation; animation = NULL; animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile; projectile = NULL;
      }
      animation = new Animation("animation_shout",
            (Gamedata::getInstance().getXmlBool("animation_shout/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_shout/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
      break;
    case ACTION_ID::SMASH:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      animation = new Animation("animation_smash",
            (Gamedata::getInstance().getXmlBool("animation_smash/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_smash/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
      if(target->getX() > x)
        animation->setVariation(0);
      else if(target->getX() < x)
        animation->setVariation(1);
      else if(target->getY() > y)
        animation->setVariation(0);
      else
        animation->setVariation(1);
      break;
    case ACTION_ID::FIREBALL:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      projectile = new Projectile("projectile_fireball",
            (x * 16), (y * 16),
            (target->getX() * 16), (target->getY() * 16));
      break;
    case ACTION_ID::BLIZZARD:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      animation = new Animation("animation_blizzard",
            (Gamedata::getInstance().getXmlBool("animation_blizzard/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_blizzard/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
      break;
    case ACTION_ID::TRAP:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      animation = new Animation("animation_trap",
            (Gamedata::getInstance().getXmlBool("animation_trap/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_trap/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
      break;
    case ACTION_ID::AIM:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      animation = new Animation("animation_aim",
            (Gamedata::getInstance().getXmlBool("animation_aim/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_aim/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
      break;
    case ACTION_ID::LIGHTNING:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
        projectile = new Projectile("projectile_lightning",
            (x * 16), (y * 16),
            (target->getX() * 16), (target->getY() * 16));
      break;
    case ACTION_ID::HEAL:
      animating = action_id;
      if(animation){
        animation->resetAnimation();
        delete animation;
        animation = NULL;
      }
      if(projectile){
        projectile->resetAnimation();
        delete projectile;
        projectile = NULL;
      }
      animation = new Animation("animation_heal",
            (Gamedata::getInstance().getXmlBool("animation_heal/atTarget") ?
                  (target->getX() * 16) : (x * 16)),
            (Gamedata::getInstance().getXmlBool("animation_heal/atTarget") ?
                  (target->getY() * 16) : (y * 16)));
      break;
    default:
      break;
  }
}
int Unit::battleCalculations(int action_id, Unit* target, int* additionalEffect){
  switch(action_id){
    case ACTION_ID::ATTACK:
      return attack(target);
      break;
    case ACTION_ID::SHOUT:
      return shout();
      break;
    case ACTION_ID::SMASH:
      return smash(target);
      break;
    case ACTION_ID::FIREBALL:
      return fireball(target);
      break;
    case ACTION_ID::BLIZZARD:
      return blizzard(target);
      break;
    case ACTION_ID::TRAP:
      return trap(target);
      break;
    case ACTION_ID::AIM:
      return aim(target);
      break;
    case ACTION_ID::LIGHTNING:
      return lightning(target);
      break;
    case ACTION_ID::HEAL:
      return heal(target);
      break;
    default:
      return 1;
      break;
  }
}
int Unit::attack(Unit* target){
  int damage = (atk - target->getDef());
  if(damage < 0)
    damage = 0;
  target->setCurhp(target->getCurhp() - damage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
void Unit::move(direction d){
  if(animating) return;
  if(d == direction::up){
    y--;
    sprite->setVelocityY(-2.0);
  }else if(d == direction::down){
    y++;
    sprite->setVelocityY(2.0);
  }else if(d == direction::left){
    x--;
    sprite->setVelocityX(-2.0);
  }else if(d == direction::right){
    x++;
    sprite->setVelocityX(2.0);
  }
  if(Gamedata::getInstance().getXmlBool("coutmovement"))
    std::cout << name << " moved to: (" << x << ", " << y << ") (z-" << (y * 27 + x) << ") " << std::endl;
  sprite->setDirection(d);
  animating = -1;
}
int Unit::actionTargetType(int action_id){
  switch(action_id){
    case ACTION_ID::ATTACK:
      return Gamedata::getInstance().getXmlInt("action_attack/target");
      break;
    case ACTION_ID::DEFEND:
    default:
      return TARGET_TYPE::SELF;
      break;
    case ACTION_ID::SHOUT:
      return Gamedata::getInstance().getXmlInt("action_shout/target");
      break;
    case ACTION_ID::SMASH:
      return Gamedata::getInstance().getXmlInt("action_smash/target");
      break;
    case ACTION_ID::FIREBALL:
      return Gamedata::getInstance().getXmlInt("action_fireball/target");
      break;
        case ACTION_ID::BLIZZARD:
      return Gamedata::getInstance().getXmlInt("action_blizzard/target");
      break;
    case ACTION_ID::TRAP:
      return Gamedata::getInstance().getXmlInt("action_trap/target");
      break;
    case ACTION_ID::AIM:
      return Gamedata::getInstance().getXmlInt("action_aim/target");
      break;
        case ACTION_ID::HEAL:
      return Gamedata::getInstance().getXmlInt("action_heal/target");
      break;
    case ACTION_ID::LIGHTNING:
      return Gamedata::getInstance().getXmlInt("action_lightning/target");
      break;
  }
}
int Unit::actionRange(int action_id){
  switch(action_id){
    case ACTION_ID::ATTACK:
      return atkRange;
      break;
    case ACTION_ID::DEFEND:
    default:
      return 4;
      break;
    case ACTION_ID::SHOUT:
      return Gamedata::getInstance().getXmlInt("action_shout/range");
      break;
    case ACTION_ID::SMASH:
      return Gamedata::getInstance().getXmlInt("action_smash/range");
      break;
    case ACTION_ID::FIREBALL:
      return Gamedata::getInstance().getXmlInt("action_fireball/range");
      break;
    case ACTION_ID::BLIZZARD:
      return Gamedata::getInstance().getXmlInt("action_blizzard/range");
      break;
    case ACTION_ID::TRAP:
      return Gamedata::getInstance().getXmlInt("action_trap/range");
      break;
    case ACTION_ID::AIM:
      return Gamedata::getInstance().getXmlInt("action_aim/range");
      break;
    case ACTION_ID::HEAL:
      return Gamedata::getInstance().getXmlInt("action_heal/range");
      break;
    case ACTION_ID::LIGHTNING:
      return Gamedata::getInstance().getXmlInt("action_lightning/range");
      break;
  }
}
bool Unit::canTarget(int targetType, Unit* target){
  switch(targetType){
    case TARGET_TYPE::ALLY:
      if(target == NULL)
        return false;
      else if(player == target->player)
        return true;
      else
        return false;
      break;
    case TARGET_TYPE::ENEMY:
      if(target == NULL)
        return false;
      else if(player != target->player)
        return true;
      else
        return false;
      break;
    case TARGET_TYPE::BOTH:
      if(target == NULL)
        return false;
      else
        return true;
      break;
    case TARGET_TYPE::NEITHER:
      if(target == NULL)
        return true;
      else
        return false;
      break;
    case TARGET_TYPE::SELF:
      if(target == NULL)
        return false;
      else if(*this == *target)
        return true;
      else
        return false;
      break;
    default:
      return false;
      break;
  }
}
void Unit::deathAnimation(){
  if(explosion){
    delete explosion;
  }
  animating = -2;
  explosion = new ExplodingSprite(sprite->getImage(), sprite->getX(), sprite->getY());
}

//unique commands will be implemented in the last project
//  Warrior
int Unit::shout(){
  atk += atk / 2;
  return curhp;
}
int Unit::smash(Unit* target){
  int damage = ((atk * 1.3) - target->getDef());
  if(damage < 0)
    damage = 0;
  target->setCurhp(target->getCurhp() - damage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
//  Mage
int Unit::fireball(Unit* target){
  const int fireballDamage = 18;  //deals true damage
  target->setCurhp(target->getCurhp() - fireballDamage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
int Unit::blizzard(Unit* target){
  const int blizzardDamage = 12;  //deals true damage
  target->setCurhp(target->getCurhp() - blizzardDamage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
//  Archer
int Unit::trap(Unit* target){
  int damage = ((atk * 1.5) - target->getDef());
  if(damage < 0)
    damage = 0;
  target->setCurhp(target->getCurhp() - damage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
int Unit::aim(Unit* target){
  int damage = ((atk * 1.2) - target->getDef());
  if(damage < 0)
    damage = 0;
  target->setCurhp(target->getCurhp() - damage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
//  Paladin
int Unit::lightning(Unit* target){
  const int lightningDamage = 14;  //deals true damage
  target->setCurhp(target->getCurhp() - lightningDamage);
  if(target->getCurhp() < 0){
    target->setCurhp(0);
  }
  return target->getCurhp();
}
int Unit::heal(Unit* target){
  const int healAmount = 23;  //heals specific amount
  target->setCurhp(target->getCurhp() + healAmount);
  if(target->getCurhp() > target->getHp()){
    target->setCurhp(target->getHp());
  }
  return target->getCurhp();
}
