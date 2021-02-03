#include <Adafruit_Arcada.h> // include for screen

/***
 * 
 * Snake game
 * 
 * Start in center and go up
 * Apples must generate in a differnt qudrant?
 * 
 * Button A: Left Down Up
 * Button B: Right and Down
 * Reset   : Resets the game
 * 
 */

#define DEBUG

#define BUTTON_A 5
#define BUTTON_B 11

#define BOARD_X 0
#define BOARD_Y 0
#define BOARD_WIDTH 240
#define BOARD_HEIGHT 240
#define CELL_SIZE 8

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3


//#define LOOP_DELAY 200
#define FPS 60
const double MSPF = 1000 / FPS; // 16.66667ms
const int MAX_X_INDEX = BOARD_WIDTH / CELL_SIZE;
const int MAX_Y_INDEX = BOARD_HEIGHT / CELL_SIZE;

double inputDelayCounter = 0.;
int inputDelayMax = 120;

int xPos = 0;
int yPos = MAX_Y_INDEX / 2;
int dir = UP;
bool gameOver = false;
int board[MAX_X_INDEX * MAX_Y_INDEX];

Adafruit_Arcada arcada;

unsigned long lastTime;
double dt;


void setup() {
#ifdef DEBUG
   Serial.begin(115200);
#endif

  randomSeed(millis());

  Serial.println("CLUE SNAKE Starting");
/*
  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
  if (!arcada.arcadaBegin()) {
    Serial.print("Failed to begin");
    while (1);
  }
  
  arcada.displayBegin();
  arcada.display->setCursor(0, 0);
  arcada.display->setTextSize(3);

   // Turn on backlight
  arcada.setBacklight(255);

  snakeList.Add(1, 2);
  dumpSnake();
  snakeList.Add(3, 4);
  dumpSnake();
  snakeList.Add(5, 6);
  dumpSnake();
  snakeList.Remove(snakeList.Head());
  dumpSnake();
  snakeList.Add(7, 8);
  dumpSnake();



  

//  snakeList = new LinkedList();

  // init board
  int firstAppleX = random(0, MAX_X_INDEX);
  int firstAppleY = random(0, MAX_Y_INDEX);
  for(int i = 0; i < MAX_X_INDEX * MAX_Y_INDEX; i++) {
    int y = i % MAX_Y_INDEX;
    int x = i / MAX_X_INDEX;
    if(firstAppleX == x && firstAppleY == y) {
      board[i] = 1;
    } else {
      board[i] = 0;
    }
  }
  */
}



void moveSnake() {
  int newY = 0;
  int newX = 0;
  
  switch(dir) {
    case UP:
      newX = xPos;
      newY = yPos - 1;
      if(newY < 0)
        gameOver = true;
      break;
    case RIGHT:
      newX = xPos + 1;
      newY = yPos;
      if(newX > MAX_X_INDEX)
        gameOver = true;
      break;
    case DOWN:
      newX = xPos;
      newY = yPos + 1;
      if(newY > MAX_Y_INDEX)
        gameOver = true;
      break;
    case LEFT:
      newX = xPos - 1;
      newY = yPos;
      if(newX < 0)
        gameOver = true;
      break;
  }
  
  arcada.display->fillRect(newX * CELL_SIZE, newY * CELL_SIZE, CELL_SIZE, CELL_SIZE, ARCADA_WHITE);

  // if we don't eat, remove head so we move the snake. otherwise we're growing
  int index = newY * MAX_Y_INDEX + newX;
  if(board[index] != 1) {
    int tailX = 0;
    int tailY = 0;
    arcada.display->fillRect(tailX * CELL_SIZE-CELL_SIZE, tailY * CELL_SIZE, CELL_SIZE, CELL_SIZE, ARCADA_BLACK);
    
  } 
}

//void drawSnake() {
//
//  
//
//  
//  arcada.display->fillRect(xPos * CELL_SIZE-CELL_SIZE, yPos * CELL_SIZE, CELL_SIZE, CELL_SIZE, ARCADA_BLACK);
//  arcada.display->fillRect(xPos * CELL_SIZE, yPos * CELL_SIZE, CELL_SIZE, CELL_SIZE, ARCADA_WHITE);
//}



void loop() {

  return;
  
  delay(MSPF); // 60 frames per second

  //drawSnake();

  inputDelayCounter += MSPF;
  if(inputDelayCounter > inputDelayMax) {
    inputDelayCounter = 0;

    if(digitalRead(BUTTON_A) == 0) {
      if(dir == UP || dir == DOWN)
        dir = LEFT;
      else
        dir = UP;
    } else if (digitalRead(BUTTON_B) == 0) {
      if(dir == UP || dir == DOWN)
        dir = RIGHT;
      else 
        dir = DOWN;
    }
    moveSnake();
  }
  
  //arcada.display->fillScreen(ARCADA_BLACK);
  
  //delay(LOOP_DELAY);
}
