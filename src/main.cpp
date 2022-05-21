#include <TFT_eSPI.h>       
#include "strings.h" 
#include "font.h"
#include "intro.h"
#include "intromessa.h"
#include "outro.h"
#include "collision_map.h"
#include "bottom_layer.h"
#include "background.h"
#include "top_layer.h"
#include "cristobal.h"
#include "carol.h"
#include "swimmer.h"
#include "vecina.h"
#include "dog.h"
#include "fire.h"
#include <SPI.h>



TFT_eSPI tft = TFT_eSPI();  
TFT_eSprite frameBuffer = TFT_eSprite(&tft); 

#define UP_PIN 13
#define DOWN_PIN 12
#define LEFT_PIN 2
#define RIGHT_PIN 15
#define MID_PIN 27
#define SET_PIN 26
#define RST_PIN 25

bool upButtonState;
bool downButtonState;
bool leftButtonState;
bool rightButtonState;
bool midButtonState;
bool setButtonState;
bool rstButtonState;
bool lastMidButtonState;
bool lastSetButtonState;
bool lastRstButtonState;

uint8_t scene = 0;
uint8_t animationCounter = 0;

#define TILESIZE 32
#define WIDTH 240
#define HEIGHT 135
#define MAPWIDTH 50
#define MAPHEIGHT 50
#define MESSAGEWIDTH 20



unsigned long lastTimeDisplayed = 0;

unsigned long framePeriod = 100;

unsigned long lastSceneDisplayed = 0;
unsigned long scenePeriod = 1000;

unsigned long lastCharacterDisplayed = 0;
uint8_t characterDelay = 25;
unsigned long lastTimeDebugged = 0;


unsigned long lastDebounceCheck = 0;
uint8_t debounceDelay = 200;

int xBackgroundOffset = 0;
int yBackgroundOffset = 0;


int playerMoveSpeed = 2;
bool showMessage = 0;
String currentMessage = TEXT_2;
int phraseStart = 0;
int phraseEnd = 0;
int characterIndex = 0;
int performActionFlag = 0;
int level = 0;

typedef struct {
  int x; //for x-coordinate on the screen itself (0-95)
  int y; //for y-coordinate on the screen itself (0-63)
  int width; //width of sprite
  int height; //height of sprite
  const unsigned int * bitmap; //a pointer to the bitmap currently in use
  byte animationLength;
  int xMap; //for x-coordinate on map grid
  int yMap; //for y-coordinate on map grid
  int xTarget;
  int yTarget;
  bool moving;
  byte direction;
} sprite;

//Flags
#define SHOO_DOGS 1
#define RECEIVE_MATCHES 2
#define LIT_FIRE 3
#define MOVE_CAR 4
#define TAKE_PICTURE 5
#define FINALE 6

#define EMPTY_TILE 0
#define DOGS 3
#define NEIGHBOUR 4
#define CAR 7
#define WATER 8
#define TREKKING 9
#define WATERBOTTLE 102
#define PEGS 124
#define PAN 96
#define EGG 101
#define GUITAR 97
#define SOCK 95
#define SNEACKER 88
#define TABLET 89
#define TABLE 82
#define MUSHROOM 103
#define KEY 126
#define SHORTS 125
#define FIRECAMP 127
#define SWIMMER 10
#define SIGN 69

//inventory
bool matches = 0;
bool keys = 0;
bool pegs = 0;
byte socks = 0;
byte sneackers = 0;
bool tablet = 0;
bool shorts = 0;
bool waterbottle = 0;
byte eggs = 0;
bool mushroom = 0;

sprite carol = {96, 64, 32, 32, carolSprite,3, 27, 10, -864, -320, 0, 0};
sprite cristobal = {128, 64, 32, 32, cristobalSprite, 3, 29, 13, -800, -352, 0, 2};
sprite swimmer = {0, 9, 32, 32, swimmerSprite, 3, 12, 44, 0, 0, 0, 0};
sprite vecina = {0, 0, 32, 32, vecinaSprite, 3, 38, 8, 0,  0, 1};
sprite dog1 = {0, 0, 32, 32, dogSprite, 3, 44, 16, 0, 0, 1};
sprite dog2 = {0, 0, 32, 32, dogSprite, 3, 45, 17, 0, 0, 1};
sprite fire = {0, 0, 32, 32, fireSprite, 3, 0, 0, 0, 0, 1};

sprite * spriteList[6] = {
  &cristobal,
  &vecina,
  &dog1,
  &dog2,
  &fire,
  &swimmer,
};
sprite * players[2] = {
  &carol,
  &cristobal
};

int * backgroundLayers[1][2500]={
  *bottomLayer,
};
bool currentPlayer = 0;

void initPlayer(){
  xBackgroundOffset = players[currentPlayer]->xTarget;
  yBackgroundOffset = players[currentPlayer]->yTarget;
  spriteList[0] = players[!currentPlayer];
}

void loadItem(int id, int x, int y){
    collisionMap[y][x] = id;
    topLayer[y][x] = id;
}

void moveCar(){
  topLayer[10][24] =  topLayer[9][28];
  topLayer[10][25] =  topLayer[9][29];
  topLayer[11][24] =  topLayer[10][28];
  topLayer[11][25] =  topLayer[10][29];
  topLayer[9][28]=  collisionMap[9][28]=0;
  topLayer[9][29]=  collisionMap[9][29]=0;
  topLayer[10][28]=  collisionMap[10][28]=0;
  topLayer[10][29]=  collisionMap[10][29]=0;
  collisionMap[10][24]=0;
  collisionMap[10][25]=0;
  collisionMap[11][24]=0;
  collisionMap[11][25]=0;
  players[1]->xMap=24;
  players[1]->yMap=12;
}

void moveDogs(){
  dog1.xMap = 33;
  dog1.yMap = 10;
  dog2.xMap = 37;
  dog2.yMap = 10;
}
void litFire(){
  fire.xMap = 27;
  fire.yMap = 12;
}
void setTent(){
  loadItem(93,24,12);
  loadItem(94,25,12);
  cristobal.xTarget=-20*TILESIZE;
  cristobal.yTarget=-11*TILESIZE;
}
void moveNeighbour(){
  vecina.xMap = 36;
  vecina.yMap = 13;
  vecina.direction = random(4);
}
void takePicture(){
  swimmer.xMap = 5;
  swimmer.yMap = 36;
  swimmer.direction=3;
  cristobal.xTarget=-1*TILESIZE;
  cristobal.yTarget=-32*TILESIZE;
  carol.xTarget=-3*TILESIZE;
  carol.yTarget=-32*TILESIZE;
  cristobal.xMap=5;
  cristobal.yMap=34;
  carol.xMap=6;
  carol.yMap=34;
  initPlayer();
}





void performAction(){
  switch(performActionFlag) {
    case SHOO_DOGS:
      moveDogs();
      level=3;
      break;
    case RECEIVE_MATCHES:
      matches = 1;
      moveNeighbour();
      level=4;
      break;
    case MOVE_CAR:
      moveCar();
      level=7;
      break;
    case LIT_FIRE:
      litFire();
      loadItem(KEY, 47,26);
      level=5;
      break;
    case TAKE_PICTURE:
      takePicture();
      level=10;
      lastSceneDisplayed=millis();
      scene++;
      break;
    case FINALE:
      scene=3;
  }
  performActionFlag = 0;
}
void loadPhrase()
{
  if (phraseEnd==-1){
    phraseStart = 0;
    phraseEnd = 0;
    showMessage = 0;
    if (performActionFlag){
      performAction();
    }
    return;
  }
  uint8_t accWidth = 0;
  characterIndex =0;
  uint8_t lastWordIndex = phraseEnd;
  while (( phraseStart + accWidth <= currentMessage.length())){
    phraseEnd = currentMessage.indexOf(' ', phraseStart + accWidth+1); 
    if (currentMessage.substring(lastWordIndex,phraseEnd).indexOf('\n',1)>0){
      phraseEnd = lastWordIndex + currentMessage.substring(lastWordIndex,phraseEnd).indexOf('\n',1);
      return;
    }
    if (accWidth + (phraseEnd - lastWordIndex) >= MESSAGEWIDTH) {
        return;
    }
    accWidth += phraseEnd - lastWordIndex;
    lastWordIndex = phraseEnd;
    }
  }

void displayMessage(){
   phraseStart = phraseEnd;
   showMessage = 1;
   loadPhrase();  
}


void prepareFinale(){
  currentMessage = TEXT_46;
  displayMessage();
  cristobal.xTarget=-24*TILESIZE;
  cristobal.yTarget=-10*TILESIZE;
  carol.xTarget=-24*TILESIZE;
  carol.yTarget=-11*TILESIZE;
  cristobal.xMap=28;
  cristobal.yMap=12;
  carol.xMap=27;
  carol.yMap=13;
  initPlayer();
  loadItem(GUITAR, 24, 15);

}



void checkEvents(){
  int collisionTile = 0;
  int collisionTileX;
  int collisionTileY;
   if (collisionMap[(players[currentPlayer]->yMap - 1+(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(players[currentPlayer]->xMap + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  > level && !upButtonState ) { //NORTH
    collisionTileX=(players[currentPlayer]->xMap + (MAPWIDTH-1)) % (MAPWIDTH-1);
    collisionTileY=(players[currentPlayer]->yMap - 1+(MAPHEIGHT-1)) % (MAPHEIGHT-1);
    collisionTile = collisionMap[collisionTileY][collisionTileX];

  } 
  if (collisionMap[(players[currentPlayer]->yMap + 1 +(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(players[currentPlayer]->xMap + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  > level && !downButtonState) { //SOUTH
    collisionTileX=(players[currentPlayer]->xMap + (MAPWIDTH-1)) % (MAPWIDTH-1);
    collisionTileY=(players[currentPlayer]->yMap + 1 +(MAPHEIGHT-1)) % (MAPHEIGHT-1);
    collisionTile = collisionMap[collisionTileY][collisionTileX];
  } 
  if (collisionMap[(players[currentPlayer]->yMap +(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(players[currentPlayer]->xMap - 1 + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  > level && !leftButtonState) { //EAST
    collisionTileX=(players[currentPlayer]->xMap - 1 + (MAPWIDTH-1)) % (MAPWIDTH-1);
    collisionTileY=(players[currentPlayer]->yMap +(MAPHEIGHT-1)) % (MAPHEIGHT-1);
    collisionTile = collisionMap[collisionTileY][collisionTileX];
  } 
  if (collisionMap[(players[currentPlayer]->yMap +(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(players[currentPlayer]->xMap + 1 + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  > level && !rightButtonState) { //WEST
      collisionTileX=(players[currentPlayer]->xMap + 1 + (MAPWIDTH-1)) % (MAPWIDTH-1);
    collisionTileY=(players[currentPlayer]->yMap +(MAPHEIGHT-1)) % (MAPHEIGHT-1);
    collisionTile = collisionMap[collisionTileY][collisionTileX];
  } 
  switch(collisionTile) {
    case PAN:
      // code block
      if (!showMessage){
        if (level==0){
          if (currentPlayer==0){
              currentMessage = TEXT_6;
              displayMessage();
              loadItem(0,28,32);
              level=1;
          } 
        }
      }
    break;
    case TREKKING:
      // code block
      if (!showMessage){     
          if (currentPlayer==0){
            if (sneackers>=2){
              currentMessage = TEXT_41;
              displayMessage();
            }else{
              currentMessage = TEXT_18;
              displayMessage();
              if (sneackers<1){
                loadItem(SNEACKER,3,6);
                loadItem(SNEACKER,31,3);
              }
            }
          } 
           else{
             if (waterbottle){
              currentMessage = TEXT_42;
              displayMessage();
             } else {
              currentMessage = TEXT_17;
              displayMessage();
            
              loadItem(WATERBOTTLE,2,19);
             }
          } 
      }
    break;
    case KEY:
      // code block
      if (!showMessage){
          if (currentPlayer==1){
              currentMessage = TEXT_36;
              displayMessage();
              loadItem(0,47,26);
              keys=1;
          } 
      }
    break;
    case SIGN:
      // code block
      if (!showMessage){
              currentMessage = TEXT_48;
              displayMessage();
      }
    break;
    case TABLE:
      // code block
      if (!showMessage){
          if (currentPlayer){
              currentMessage = TEXT_3;
              displayMessage();
          } else {
            currentMessage = TEXT_2;
              displayMessage();
          }
      }
    break;
    case WATERBOTTLE:
      // code block
      if (!showMessage){
          if (currentPlayer==1){
              currentMessage = TEXT_37;
              displayMessage();
              loadItem(EMPTY_TILE,2,19);
              waterbottle=1;
          } 
      }
    break;
    case WATER:
      // code block
      if (!showMessage){
        if (level==7){
          if (currentPlayer==0){
            if (socks>=2){
              currentMessage = TEXT_41;
              displayMessage();
            }else {
              currentMessage = TEXT_15;
              displayMessage();
              loadItem(SOCK,43,33);
              loadItem(SOCK,34,13);
            }
          } else {
            if (shorts){
              currentMessage = TEXT_42;
              displayMessage();
            } else {
              currentMessage = TEXT_16;
              displayMessage();
              loadItem(SHORTS,33,34);
            }
          }
        }
      }
    break;
    case FIRECAMP:
      // code block
      if (!showMessage){
         if (level<1){
            if (currentPlayer==0){
              currentMessage = TEXT_29;
              displayMessage();
            }  else {
              currentMessage = TEXT_30;
              displayMessage();
            }

        } 
        if (level==1){
          if (currentPlayer==0){
              currentMessage = TEXT_7;
              displayMessage();
              level=2;
          } 
        } 
        if (level==4){
          if (currentPlayer==0){
              currentMessage = TEXT_11;
              displayMessage();
              performActionFlag = LIT_FIRE;
          } 
        } 
        if (pegs&&currentPlayer==1){
          if (level<5){
            currentMessage = TEXT_33;
            displayMessage();
          } else {
            currentMessage = TEXT_32;
            setTent();
            level=6;
            displayMessage();
            pegs=0;
          }
        }
      }
    break;
    case PEGS:
      // code block
      if (!showMessage){
          if (currentPlayer==1){
              currentMessage = TEXT_28;
              displayMessage();
              pegs = 1;
              loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
          } 
      }
    break;
    case TABLET:
      // code block
      if (!showMessage){
        currentMessage = TEXT_39;
        displayMessage();
        tablet = 1;
        loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
      }
    break;
    case GUITAR:
      // code block
      if (!showMessage){
        currentMessage = TEXT_47;
        displayMessage();
        performActionFlag=FINALE;
        loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
      }
    break;
    case SWIMMER:
      // code block
      if (!showMessage){
          if (currentPlayer==0){
            if (tablet){
              currentMessage = TEXT_40;
              displayMessage();
              performActionFlag = TAKE_PICTURE;
            }else {
              currentMessage = TEXT_20;
              displayMessage();
              loadItem(TABLET,2,22);
            }
          } 
          else {
            currentMessage = TEXT_12;
            displayMessage();
          }
      }
    break;
    case DOGS:
      // code block
      if (!showMessage){
         if (level<2){
            currentMessage = TEXT_31;
            phraseStart = phraseEnd;
            showMessage = 1;
            loadPhrase();   
         }
        if (level==2){
          if (currentPlayer==0){
              currentMessage = TEXT_8;
              displayMessage();
          } else {
              currentMessage = TEXT_9;
              displayMessage();
              performActionFlag = SHOO_DOGS;
          }
        }
      }
      break;
    case NEIGHBOUR:
      // code block
      if (!showMessage){
        if (level==3){
          if (currentPlayer==0){
              currentMessage = TEXT_10;
              displayMessage();
              performActionFlag = RECEIVE_MATCHES;
          } else {
              currentMessage = TEXT_12;
              displayMessage();         
          }
        }
      }
      break;
      case SOCK:
      // code block
      if (!showMessage){
          if (currentPlayer==0){
              socks++;
              currentMessage = String(socks) + TEXT_34;
               loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
               displayMessage();
              
          }
      }
      case EGG:
      // code block
      if (!showMessage){
          if (currentPlayer){
              eggs++;
              currentMessage = String(eggs) + TEXT_45;
              loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
              displayMessage();
          }
      }
      case SNEACKER:
      // code block
      if (!showMessage){
          if (currentPlayer==0){
              sneackers++;
              currentMessage = String(sneackers) + TEXT_38;
               loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
               displayMessage();   
          }
      }
      case SHORTS:
      // code block
      if (!showMessage){
          if (currentPlayer){
              currentMessage = TEXT_35;
              shorts=1;
              displayMessage();
              loadItem(EMPTY_TILE, 33,34);
          }
      }
      case MUSHROOM:
      // code block
      if (!showMessage){
          if (currentPlayer){
              currentMessage = TEXT_44;
              displayMessage();
              loadItem(EGG, 18,36);
              loadItem(EGG, 16,35);
              loadItem(EGG, 30,40);
              loadItem(EGG, 18,31);
              loadItem(EGG, 12,22);
              loadItem(EGG, 2,15);
              loadItem(EGG, 36,27);
              loadItem(EGG, 43,2);
              loadItem(EGG, 46,41);
              loadItem(EGG, 22,21);
              loadItem(EMPTY_TILE,collisionTileX,collisionTileY);
              level++;
          }
      }
      break;
      case CAR:
      // code block
      if (!showMessage){
        if (level==6){
          if (!keys){
            if (currentPlayer==0){
                currentMessage = TEXT_26;
                phraseStart = phraseEnd;
                showMessage = 1;
                loadPhrase();   
            } else {
                currentMessage = TEXT_25;
                phraseStart = phraseEnd;
                showMessage = 1;
                loadPhrase();          
            }
          } else {
             if (currentPlayer==0){
                currentMessage = TEXT_26;
                phraseStart = phraseEnd;
                showMessage = 1;
                loadPhrase();   
            } else {
                currentMessage = TEXT_27;
                phraseStart = phraseEnd;
                showMessage = 1;
                loadPhrase();    
                performActionFlag = MOVE_CAR;  
            }
          } 
        }
      }
      break;
    default:
      // code block
      return;
  }
  if (socks>=2 &&shorts&&level<8){
    level=8;
  }
  if (waterbottle && sneackers>=2 && level <9){
    level=9;
  }
  if (eggs>=10){
      prepareFinale();
  }
}


void checkButtons(){
  
upButtonState =  digitalRead(UP_PIN);
downButtonState =  digitalRead(DOWN_PIN);
leftButtonState =  digitalRead(LEFT_PIN);
rightButtonState =  digitalRead(RIGHT_PIN);
midButtonState =  digitalRead(MID_PIN);
setButtonState =  digitalRead(SET_PIN);
rstButtonState =  digitalRead(RST_PIN); 
}



void draw(){
  
  for (int y = 0; y < HEIGHT; y++) {
      if (showMessage&&y>98){
              if (millis()>lastCharacterDisplayed+characterDelay){
               characterIndex++;
               lastCharacterDisplayed = millis();
              }
              frameBuffer.drawString(currentMessage.substring(phraseStart, min(phraseStart+characterIndex, phraseEnd)), 6, 112);
                for (int x = 0; x < WIDTH; x++) {
                    frameBuffer.drawPixel(x,y, 0x8869);
                }
      } else {
        int yTileNum = (y - yBackgroundOffset) / TILESIZE;   
        yTileNum =(yTileNum+(MAPHEIGHT-1)) % (MAPHEIGHT-1);
        int yTileOffset = (y - yBackgroundOffset) % TILESIZE;  
        yTileOffset =(yTileOffset+(MAPHEIGHT-1)) % (MAPHEIGHT-1);
        int yBitmapOffset = yTileOffset * TILESIZE*8;
        for (int x = 0; x < WIDTH; x++) {
          int xTileNum = (x - xBackgroundOffset) / TILESIZE;
          xTileNum =(xTileNum+(MAPWIDTH-1)) % (MAPWIDTH-1);
          int xTileOffset = (x - xBackgroundOffset) % TILESIZE;      
          int blTile = bottomLayer[yTileNum][xTileNum];
          const unsigned int * blBitmap = 0;
          blBitmap = &bgSprites[(blTile%8+animationCounter*(blTile>=104))*(TILESIZE)+(blTile/8)*TILESIZE*TILESIZE*8] + yBitmapOffset; 
          int tlTile = topLayer[yTileNum][xTileNum];
          const unsigned int * tlBitmap = 0;
          tlBitmap = &bgSprites[(tlTile%8+animationCounter*(tlTile>=104&&tlTile<=123))*(TILESIZE)+(tlTile/8)*TILESIZE*TILESIZE*8] + yBitmapOffset; 
          if (tlTile && tlBitmap[xTileOffset]){
            frameBuffer.drawPixel(x,y, tlBitmap[xTileOffset]);
          } else {
              frameBuffer.drawPixel(x,y, blBitmap[xTileOffset]);
          }
        
    }


    for (int spriteIndex = 0; spriteIndex < * (&spriteList + 1) - spriteList; spriteIndex++) {
      sprite *cs = spriteList[spriteIndex];
      int relativeY = (cs->yMap + yBackgroundOffset /TILESIZE)*TILESIZE;
      int relativeX = (cs->xMap + xBackgroundOffset /TILESIZE)*TILESIZE;

        if (players[currentPlayer]->yTarget < yBackgroundOffset) { //SOUTH
            relativeY+=abs(players[currentPlayer]->yTarget-yBackgroundOffset)-TILESIZE; //correct
        }
        if (players[currentPlayer]->yTarget > yBackgroundOffset) { //NORTH
            relativeY-=abs(players[currentPlayer]->yTarget-yBackgroundOffset);
        }
        if (players[currentPlayer]->xTarget < xBackgroundOffset) { //SOUTH
            relativeX+=abs(players[currentPlayer]->xTarget-xBackgroundOffset)-TILESIZE; //correct
        }
        if (players[currentPlayer]->xTarget > xBackgroundOffset) { //NORTH
            relativeX-=abs(players[currentPlayer]->xTarget-xBackgroundOffset);
        }
      if (y >= relativeY && y < relativeY + cs->height) {
        int endX = relativeX + cs->width;
        if (relativeX < WIDTH && endX > 0) {
          int xBitmapOffset = (cs->direction*(cs->width)*(cs->height)*cs->animationLength)+cs->moving*(animationCounter*cs->width);
          int xStart = 0;
          if (relativeX < 0) {
            xBitmapOffset -= relativeX;
          }
          if (relativeX > 0) {
            xStart = relativeX;
          }
          int yBitmapOffset = (y - relativeY) * (cs->width)*cs->animationLength;
          for (int x = xStart; x < endX; x++) {
            unsigned int color = cs->bitmap[xBitmapOffset + yBitmapOffset++];
            if (color){
              frameBuffer.drawPixel(x,y, color);      
            }
          }
        }
      }

    }
    sprite *player =players[currentPlayer];
      if (y >= player->y && y < player->y + player->height) {
        int endX = player->x + player->width;
        if (player->x < WIDTH && endX > 0) {
          int xBitmapOffset = (player->direction*(player->width)*(player->height)*player->animationLength)+player->moving*(animationCounter*player->width);
          int xStart = 0;
          if (player->x < 0) {
            xBitmapOffset -= player->x;
          }
          if (player->x > 0) {
            xStart = player->x;
          }
          int yBitmapOffset = (y - player->y) * (player->width)*player->animationLength;
          for (int x = xStart; x < endX; x++) {
            unsigned int color = player->bitmap[xBitmapOffset + yBitmapOffset++];
            if (color){
              frameBuffer.drawPixel(x,y, color);      
            }
          }
        }
      }
   


      }
  }  
  
    
}



void movePlayer() {

 sprite *player=players[currentPlayer];

  if(collisionMap[player->yMap][player->xMap]==WATER){
     playerMoveSpeed=1;
      if (currentPlayer){
          player->bitmap = cristobalBathingSuitSprite;
      }else {
         player->bitmap = carolBathingSuiteSprite;
      }
  } else{
     playerMoveSpeed=4;
       if (currentPlayer){
          player->bitmap = cristobalSprite;
      }else {
         player->bitmap = carolSprite;
      }
  }
  if (player->yTarget < yBackgroundOffset) { //SOUTH
    yBackgroundOffset -= playerMoveSpeed; //player will move based on set value of playerMoveSpeed
    player->direction = 0; //Set to a different number for each direction for sprite updating
  }
  if (player->xTarget > xBackgroundOffset) { //WEST
    xBackgroundOffset += playerMoveSpeed;
    player->direction = 1;
  }
  if (player->xTarget < xBackgroundOffset) { //EAST
    xBackgroundOffset -= playerMoveSpeed;
    player->direction = 2;
  }
  if (player->yTarget > yBackgroundOffset) { //NORTH
    yBackgroundOffset += playerMoveSpeed;
    player->direction = 3;
  }

  if (player->xTarget == xBackgroundOffset && player->yTarget == yBackgroundOffset) { //Checks if player is at target coordinate
    player->moving = false; //If the target is reached, player->moving is set to false
    player->yMap = (player->y - yBackgroundOffset) / TILESIZE;
    player->xMap = (player->x - xBackgroundOffset) / TILESIZE;
  }


  if (collisionMap[(player->yMap - 1+(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(player->xMap + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  <= level && !upButtonState && !player->moving) { //NORTH
    player->moving = true;
    player->yTarget += TILESIZE;
  } 
  if (collisionMap[(player->yMap + 1 +(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(player->xMap + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  <= level && !downButtonState && !player->moving) { //SOUTH
    player->moving = true;
    player->yTarget -= TILESIZE;
  } 
  if (collisionMap[(player->yMap +(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(player->xMap - 1 + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  <= level && !leftButtonState && !player->moving) { //EAST
    player->moving = true;
    player->xTarget += TILESIZE;
  } 
  if (collisionMap[(player->yMap +(MAPHEIGHT-1)) % (MAPHEIGHT-1)][(player->xMap + 1 + (MAPWIDTH-1)) % (MAPWIDTH-1)] 
  <= level && !rightButtonState && !player->moving) { //WEST
    player->moving = true;
    player->xTarget -= TILESIZE;
  } 
/***********************************************************************************/

/*
  These lines can be implemented for troubleshooting and making sure that
  the player's coordinates on the map are correct.
*/
if (millis()>lastTimeDebugged+1000){
  /*Serial.println("Player 1");
  Serial.print("XMap: "); Serial.println(players[0]->xMap);
  Serial.print("YMap: "); Serial.println(players[0]->yMap);
  Serial.print("X: "); Serial.println(players[0]->x);
  Serial.print("Y: "); Serial.println(players[0]->y);
  Serial.println("Player 2");
  Serial.print("XMap: "); Serial.println(players[1]->xMap);
  Serial.print("YMap: "); Serial.println(players[1]->yMap);
  Serial.print("X: "); Serial.println(players[1]->x);
  Serial.print("Y: "); Serial.println(players[1]->y);
  Serial.println("Background offset");
  Serial.print("X: "); Serial.println(xBackgroundOffset);
  Serial.print("Y: "); Serial.println(yBackgroundOffset);
  */
  Serial.print("ps: ");Serial.println(phraseStart);
  Serial.print("pe: ");Serial.println(phraseEnd);
  lastTimeDebugged = millis();
}
}

void game(){

  draw();
  if (millis()> lastDebounceCheck + debounceDelay){
   if (lastRstButtonState!=rstButtonState){
      if (!rstButtonState){
          playerMoveSpeed = !(playerMoveSpeed-1) + 1;
      }
     lastRstButtonState = rstButtonState;
  }
  }
  if (millis()> lastDebounceCheck + debounceDelay){
   if (lastSetButtonState!=setButtonState){
      if (!setButtonState){
          currentPlayer=!currentPlayer;
          initPlayer();
      }
     lastSetButtonState = setButtonState;
   }
 }
  if (millis()> lastDebounceCheck + debounceDelay){
   if (lastMidButtonState!=midButtonState){
      if (!midButtonState){
        if (showMessage){
          phraseStart = phraseEnd;
          loadPhrase();   
        }   
      }
     lastMidButtonState = midButtonState;
   }
 }
 if (!showMessage){
    movePlayer();
  }
  checkEvents();
}

void intro(){
  frameBuffer.pushImage(0,0,240,135, introAnimation[animationCounter/2]);
  if ((millis()> lastSceneDisplayed + scenePeriod)&&!midButtonState){
    scene++;
    lastSetButtonState = setButtonState;
  }
}
void intromessa(){
  frameBuffer.pushImage(0,0,240,135, intromessaBitmap);
   if ((millis()> lastSceneDisplayed + scenePeriod&&!midButtonState)){
    currentMessage = TEXT_43;
    displayMessage();
    loadItem(MUSHROOM,24,42);
    scene--;
    lastSetButtonState = setButtonState;
  }
}


void outro(){
  frameBuffer.pushImage(0,0,240,135, outroAnimation[animationCounter]);
}

void setup() {

  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(MID_PIN, INPUT_PULLUP);
  pinMode(SET_PIN, INPUT_PULLUP);
  pinMode(RST_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  frameBuffer.setFreeFont(&Slackey_Regular_12);
  frameBuffer.createSprite(WIDTH, HEIGHT);
  frameBuffer.fillSprite(TFT_WHITE);
  frameBuffer.setTextColor(0xFF3D);
  frameBuffer.setSwapBytes(true);
  initPlayer();
  loadItem(PAN, 28,32);
  loadItem(PEGS,34, 3);
  lastSceneDisplayed = millis();
  currentMessage = TEXT_1;
  displayMessage();
}


void loop() {
  switch (scene) {
  case 0:
    intro();
    break;
  case 1:
    game();
    break;
  case 2:
    intromessa();
    break;
  case 3:
    outro();
    break;
}
  if (millis() > lastTimeDisplayed + framePeriod){
    frameBuffer.pushSprite(0,0);
    animationCounter = (animationCounter + 4 ) % 3;
    lastTimeDisplayed = millis();
  }
    checkButtons();
  
}
