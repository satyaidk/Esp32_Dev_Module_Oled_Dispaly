#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int faceIndex = 0;
unsigned long lastChange = 0;


// -----------------------------------------------------
// Replace drawArc → create arc using line segments
// -----------------------------------------------------
void drawSmileArc(int cx, int cy, int radius, int startAngle, int endAngle) {
  for (int angle = startAngle; angle < endAngle; angle += 5) {
    float rad1 = angle * PI / 180;
    float rad2 = (angle + 5) * PI / 180;

    int x1 = cx + radius * cos(rad1);
    int y1 = cy + radius * sin(rad1);
    int x2 = cx + radius * cos(rad2);
    int y2 = cy + radius * sin(rad2);

    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
}


// -----------------------------------------------------
// FACES
// -----------------------------------------------------

void faceHappy() {
  display.clearDisplay();
  display.fillCircle(40, 25, 6, SSD1306_WHITE);
  display.fillCircle(90, 25, 6, SSD1306_WHITE);

  drawSmileArc(65, 40, 20, 20, 160);  // smile arc

  display.display();
}

void faceCuteSmile() {
  display.clearDisplay();
  display.fillCircle(40, 25, 5, SSD1306_WHITE);
  display.fillCircle(90, 25, 5, SSD1306_WHITE);

  display.drawLine(50, 40, 80, 40, SSD1306_WHITE); // straight smile
  display.display();
}

void faceAngry() {
  display.clearDisplay();

  // Angry eyebrows
  display.drawLine(30, 15, 50, 25, SSD1306_WHITE);
  display.drawLine(80, 25, 100, 15, SSD1306_WHITE);

  display.fillCircle(40, 30, 5, SSD1306_WHITE);
  display.fillCircle(90, 30, 5, SSD1306_WHITE);

  drawSmileArc(65, 50, 18, 200, 340);  // frown arc

  display.display();
}

void faceSurprised() {
  display.clearDisplay();
  display.fillCircle(40, 25, 5, SSD1306_WHITE);
  display.fillCircle(90, 25, 5, SSD1306_WHITE);

  display.drawCircle(65, 45, 8, SSD1306_WHITE);  // O-mouth
  display.display();
}

void faceSleepy() {
  display.clearDisplay();

  display.drawLine(30, 25, 50, 25, SSD1306_WHITE);
  display.drawLine(80, 25, 100, 25, SSD1306_WHITE);

  display.drawCircle(65, 45, 6, SSD1306_WHITE);
  display.display();
}

void faceBlink() {
  display.clearDisplay();
  display.drawLine(35, 25, 45, 25, SSD1306_WHITE);
  display.drawLine(85, 25, 95, 25, SSD1306_WHITE);
  display.display();
}


// -----------------------------------------------------
// FACE SWITCHER
// -----------------------------------------------------
void showFace(int id) {
  switch (id) {
    case 0: faceHappy(); break;
    case 1: faceCuteSmile(); break;
    case 2: faceAngry(); break;
    case 3: faceSurprised(); break;
    case 4: faceSleepy(); break;
    case 5: faceBlink(); break;
  }
}


// -----------------------------------------------------
void setup() {
  Wire.begin(21, 22);  // SDA=21 (D20), SCL=22 (D22)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showFace(faceIndex);
}


// -----------------------------------------------------
void loop() {
  unsigned long now = millis();

  if (now - lastChange >= 10000) {  // change every 5 sec
    faceIndex++;
    if (faceIndex > 5) faceIndex = 0;

    showFace(faceIndex);
    lastChange = now;
  }
}
