#include <Adafruit_Arcada.h> // include for screen
#include <Bounce2.h> //https://github.com/thomasfredericks/Bounce2
#include <LinkedList.h> ////https://github.com/ivanseidel/LinkedList

struct Point {
  int x;
  int y;
};

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

//#define DEBUG

#define BUTTON_A 5
#define BUTTON_B 11

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240

#define BOARD_X 0
#define BOARD_Y 0
#define BOARD_WIDTH 240
#define BOARD_HEIGHT 208
#define CELL_SIZE 8

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3


#define LOOP_DELAY 20
#define FRAME_DELAY 1000
#define FPS 10

const double MSPF = FRAME_DELAY / FPS; // 16.66667ms

const int HEADER_WIDTH = SCREEN_WIDTH;
const int HEADER_HEIGHT = SCREEN_HEIGHT - BOARD_HEIGHT;

const int MIN_X_INDEX = 0;
const int MIN_Y_INDEX = HEADER_HEIGHT / CELL_SIZE;

const int MAX_X_INDEX = BOARD_WIDTH / CELL_SIZE;
const int MAX_Y_INDEX = BOARD_HEIGHT / CELL_SIZE;


double inputDelayCounter = 0.;
int inputDelayMax = 120;

int xPos = MAX_X_INDEX / 2;
int yPos = MAX_Y_INDEX / 2;
int dir = UP;
int score = 0;
bool gameOver = false;
int board[MAX_X_INDEX * MAX_Y_INDEX];

Adafruit_Arcada arcada;
LinkedList<int> snake = LinkedList<int>();
Bounce2::Button buttonA;
Bounce2::Button buttonB;

unsigned long lastTime;
double dt;


void setup() {
#ifdef DEBUG
   Serial.begin(9600);
#endif

  randomSeed(millis());

  Serial.println("CLUE SNAKE Starting");

  buttonA.attach(BUTTON_A, INPUT);
  buttonA.interval(5);
  buttonA.setPressedState(LOW);
  
  buttonB.attach(BUTTON_B, INPUT);
  buttonB.interval(5);
  buttonB.setPressedState(LOW);
  
  if (!arcada.arcadaBegin()) {
    Serial.print("Failed to begin");
    while (1);
  }
  
  arcada.displayBegin();
  arcada.display->setCursor(0, 0);
  arcada.display->setTextSize(2);

   // Turn on backlight
  arcada.setBacklight(255);

  int startIndex = pointToIndex({xPos, yPos});
  snake.add(startIndex);

  // init board
  int appleIndex = getRandomIndex();
  while(appleIndex == startIndex) {
    appleIndex = getRandomIndex();
  }
  for(int i = 0; i < MAX_X_INDEX * MAX_Y_INDEX; i++) {
    if(i == appleIndex) {
      board[i] = 1;
    } else if(i == startIndex) {
      board[i] = 2;
    } else {
      board[i] = 0;
    }
  }

  // draw initial items
  fillCell({xPos, yPos}, ARCADA_WHITE);
  drawApple();
}

void moveSnake() {
  int headIndex = snake.get(snake.size() - 1);
  Point head = indexToPoint(headIndex);
  
  int curX = head.x;
  int curY = head.y;
  int newY = 0;
  int newX = 0;
  
  switch(dir) {
    case UP:
      newX = curX;
      newY = curY - 1;
      if(newY < 0)
        gameOver = true;
      break;
    case RIGHT:
      newX = curX + 1;
      newY = curY;
      if(newX > MAX_X_INDEX)
        gameOver = true;
      break;
    case DOWN:
      newX = curX;
      newY = curY + 1;
      if(newY > MAX_Y_INDEX)
        gameOver = true;
      break;
    case LEFT:
      newX = curX - 1;
      newY = curY;
      if(newX < 0)
        gameOver = true;
      break;
  }

  // we always move the head
  fillCell({newX, newY}, ARCADA_WHITE); // render new head
  int index = pointToIndex({newX, newY});
  snake.add(index);
  
  if(board[index] == 1) {
    // we ate so we increase score and skip removing tail
    score++;
    setNewApple();
  } else if(board[index] == 2) {
    // we hit ourself! game over
    gameOver = true;
    return;
  } else {
    // we just moved - get rid of tail
    int tailIndex = snake.shift();                // grab first element
    Point tail = indexToPoint(tailIndex);
    fillCell(tail, ARCADA_BLACK);                 // clear from screen
    board[tailIndex] = 0;                         // remove tail from board
  }
  
  board[index] = 2;                               // put snake on board
}

void drawApple() {
  for(int i = 0; i < MAX_X_INDEX * MAX_Y_INDEX; i++) {
    if(board[i] == 1) {
      fillCell(indexToPoint(i), ARCADA_RED);
    }
  }
}

void fillCell(Point p, int color) {
  arcada.display->fillRect(p.x * CELL_SIZE, p.y * CELL_SIZE + HEADER_HEIGHT, CELL_SIZE, CELL_SIZE, color);
}

void setNewApple() {
  int ix = getRandomIndex();
  while(board[ix] != 0 && ix <= MIN_Y_INDEX) {
    ix = getRandomIndex();
  }
  board[ix] = 1;
}

int getRandomIndex() {
  int firstAppleX = random(0, MAX_X_INDEX);
  int firstAppleY = random(0, MAX_Y_INDEX);
  return pointToIndex({firstAppleX, firstAppleY});
}

int pointToIndex(Point p) {
  return p.y * MAX_X_INDEX + p.x;
}

Point indexToPoint(int index) {
   return { index % MAX_X_INDEX, index / MAX_X_INDEX };
}

void drawHeader() {
  arcada.display->fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT + CELL_SIZE, ARCADA_BLACK);
  arcada.display->drawRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT + CELL_SIZE, ARCADA_BLUE);
  arcada.display->setCursor(8, 12);
  if(gameOver) {
    arcada.display->printf("GAME OVER! SCORE: %d", score);
    return;
  }
  arcada.display->printf("SCORE: %d", score);
}

int frame = 0;
bool render = false;

void loop() {

  if(gameOver) {
    return;
  }

  buttonA.update();
  buttonB.update();

  if(buttonA.pressed()) {
    if(dir == UP || dir == DOWN)
      dir = LEFT;
    else
      dir = UP;
  } else if (buttonB.pressed()) {
    if(dir == UP || dir == DOWN)
      dir = RIGHT;
    else 
      dir = DOWN;
  }
  
  frame += 1;
  if(frame >= MSPF) {
    drawApple();
    moveSnake();
    drawHeader();
    //render = true;
    frame = 0;
  }
  delay(1);
}
