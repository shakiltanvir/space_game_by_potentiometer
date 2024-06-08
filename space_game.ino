#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Spaceship variables
int spaceshipX = SCREEN_WIDTH / 2;
int spaceshipY = SCREEN_HEIGHT - 10;
int spaceshipWidth = 10;
int spaceshipHeight = 5;

// Bullet variables
int bulletX;
int bulletY;
int bulletSpeed = 2;
boolean bulletFired = false;

// Alien invader variables
int alienX = random(0, SCREEN_WIDTH);
int alienY = 0;
int alienSpeed = 1;
int alienWidth = 10;
int alienHeight = 5;

// Game variables
int score = 0;
int lives = 3;

// Button and potentiometer pins
int buttonPin = 2; // Change to the pin you're using
int potentiometerPin = 15; // Change to the pin you're using

void setup() {
  
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  //display.begin(0x3C, 4, 5);
  display.display();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Space Invaders");
  display.display();
  delay(2000); // Display the title for 2 seconds

  // Initialize the button pin
  pinMode(buttonPin, INPUT);
}

void loop() {
  // Read the potentiometer value to move the spaceship
  int potValue = analogRead(potentiometerPin);
  spaceshipX = map(potValue, 0, 4095, 0, SCREEN_WIDTH - spaceshipWidth);

  // Check if the button is pressed to fire a bullet
  if (digitalRead(buttonPin) == HIGH && !bulletFired) {
    bulletX = spaceshipX + spaceshipWidth / 2;
    bulletY = spaceshipY;
    bulletFired = true;
  }

  // Move the bullet
  if (bulletFired) {
    bulletY -= bulletSpeed;
    if (bulletY < 0) {
      bulletFired = false;
    }
  }

  // Move the alien invader
  alienY += alienSpeed;
  if (alienY > SCREEN_HEIGHT) {
    alienX = random(0, SCREEN_WIDTH);
    alienY = 0;
  }

  // Check for collisions
  if (collide(bulletX, bulletY, 2, 2, alienX, alienY, alienWidth, alienHeight)) {
    score++;
    if(score%10==0)
    {
      bulletSpeed++;
      alienSpeed++;
    }
    alienX = random(0, SCREEN_WIDTH);
    alienY = 0;
    bulletFired = false;
  }

  if (collide(spaceshipX, spaceshipY, spaceshipWidth, spaceshipHeight, alienX, alienY, alienWidth, alienHeight)) {
    lives--;
    alienX = random(0, SCREEN_WIDTH);
    alienY = 0;
  }

  // Clear the display
  display.clearDisplay();

  // Draw game elements
  display.drawRect(spaceshipX, spaceshipY, spaceshipWidth, spaceshipHeight, SSD1306_WHITE);
  display.drawCircle(bulletX, bulletY, 2, SSD1306_WHITE);
  display.drawRect(alienX, alienY, alienWidth, alienHeight, SSD1306_WHITE);

  // Display score and lives
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
  display.setCursor(SCREEN_WIDTH - 50, 0);
  display.print("Lives: ");
  display.print(lives);

  display.display();

  // Check for game over
  if (lives <= 0) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, SCREEN_HEIGHT / 2 - 10);
    display.print("Game Over");
    display.display();
    delay(2000); // Display "Game Over" for 2 seconds
    resetGame();
  }

  delay(20); // Adjust the game speed by changing the delay
}

boolean collide(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
  return (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2);
}

void resetGame() {
  score = 0;
  lives = 3;
  bulletSpeed=1;
  alienSpeed=1;
  spaceshipX = SCREEN_WIDTH / 2;
  alienX = random(0, SCREEN_WIDTH);
  alienY = 0;
  display.clearDisplay();
}
