#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif12pt7b.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

const int BUTTON_PIN = 2;
const double GRAVITY = 0.4, JUMP_POWER = -2.3;
const int PLAYER_SPEED = 2, PIPE_WIDTH = 5;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool buttonLocked = false;
int pipes[3] = { -1, -42, -84 };
double playerVelocity = 0;
int playerY = 32;

void setup() {
  //setup oled
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(200);
  display.clearDisplay();
  //setup button
  pinMode(BUTTON_PIN, INPUT);
  playIntroSeq();
}

void playIntroSeq() {
  display.setFont(&FreeSerif12pt7b);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(25, 32);
  display.clearDisplay();
  display.println("READY");
  display.display();
  delay(1400);
}

void loop() {
  display.clearDisplay();
  calculatePlayerY();
  drawPlayer();
  calculatePipes();
  if (testCollision()) {
    die();
  }
  display.display();
}

bool testCollision(){
  for (int i = 0; i < 3; i++) {
    int pipex = pipes[i];
    if (pipex < SCREEN_WIDTH/2 && pipex > (SCREEN_WIDTH/2) - 5)  {
      if (playerY < 15 || playerY > 49){
        return true;
      }
    }
  }
  return false;
}

void die() {
    display.setCursor(25, 32);
    display.println("womp :3");
    display.display();
    delay(2000);
    playerY = 32;
    playerVelocity = 0;
} 

void calculatePipes() {
  for (int i = 0; i < 3; i++) {
    int pipex = pipes[i];
    pipex += PLAYER_SPEED;
    if (pipex > -1) {                               //for the first pipes
      pipex = pipex % (SCREEN_WIDTH);  //so that they repeat themselves
    }
    drawPipe(pipex);
    pipes[i] = pipex;
  }
}

void calculatePlayerY() {

  if (digitalRead(BUTTON_PIN) == 1 && !buttonLocked) {
    playerVelocity = JUMP_POWER;
    buttonLocked = true;
  } else if (digitalRead(BUTTON_PIN) == 0) {
    buttonLocked = false;
  }
  Serial.println(playerVelocity);
  playerVelocity += GRAVITY;
  playerY += playerVelocity;
}

void drawPlayer() {
  display.fillCircle(64, playerY, 3, WHITE);
}

void drawPipe(int x) {
  display.fillRect(x, 49, PIPE_WIDTH, 15, WHITE);
  display.fillRect(x, 0, PIPE_WIDTH, 15, WHITE);
}