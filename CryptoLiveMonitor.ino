#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Correct I2C pins for ESP32-C3 Super Mini
#define I2C_SDA 8
#define I2C_SCL 9      

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WiFi credentials
const char* ssid = "Hasini";
const char* password = "Esp32Dev";

// API URL for 4 coins
const String apiURL =
  "https://api.coingecko.com/api/v3/simple/price?ids="
  "bitcoin,ethereum,solana,binancecoin"
  "&vs_currencies=usd";

// Arrays for names and JSON keys
String coinNames[4] = {"BTC", "ETH", "SOL", "BNB"};
String jsonKeys[4]  = {"bitcoin", "ethereum", "solana", "binancecoin"};

StaticJsonDocument<1024> doc;

// -------------------------------
// 40 Tech Jokes + Quotes (No Emojis)
// -------------------------------
String jokes[] = {

  
  "WiFi went for\na smoke break",
  "API busy\nwatching anime",
  "Packets ran\naway",
  "Network said:\nTry again later",
  "404: Brain\nnot found",
  "ESP32 is shy\ntoday",
  "Ping lost\nthe pong",
  "Crypto gods\nsaid no",
  "Debugging...\nplease wait",
  "Router is\nmood off",

  // +30 new ones
  "I swear I\nsent the packet",
  "WiFi wants a\ncoffee break",
  "Router said:\nNot today",
  "BRB...\ncompiling life",
  "Network ran\ngarbage cleanup",
  "Your packets\nare buffering",
  "Trying again...\nhopefully",
  "Server said:\nGo outside",
  "Brain has a\nsegmentation fault",
  "WiFi playing\nhide and seek",
  "Request decided\nto quit",
  "404: Motivation\nnot found",
  "Debug mode:\nActivated",
  "AI says:\nRetry human",
  "Router entered\nairplane mode",
  "Whales broke\nthe API",
  "Patience level:\nMinimum",
  "Bits escaped\nthe matrix",
  "ESP32 says:\nI tried",
  "Next packet will\nwork... maybe",
  "TCP handshake\nleft hanging",
  "WiFi dropped my\nfeelings too",
  "Behind every bug\nis a feature",
  "Works on my\nmachine",
  "Tech rule:\nRetry solves most",
  "Data is the\nnew oil",
  "It depends...\nalways",
  "Make it work.\nThen optimize",
  "Simple is deep.\nComplex is noise",
  "Failure teaches\nfaster than wins"
};

int jokeCount = sizeof(jokes) / sizeof(jokes[0]);

void setup() {
  Serial.begin(115200);
  randomSeed(esp_random()); // random jokes

  // Initialize I2C for OLED
  Wire.begin(I2C_SDA, I2C_SCL);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  HTTPClient http;
  http.begin(apiURL);
  int code = http.GET();

  display.clearDisplay();

  if (code == 200) {
    String body = http.getString();

    if (deserializeJson(doc, body) == DeserializationError::Ok) {

      for (int i = 0; i < 4; i++) {
        float price = doc[jsonKeys[i]]["usd"];

        display.setCursor(0, i * 14);
        display.print(coinNames[i]);
        display.print(": $");
        display.print(price);
      }

    } else {
      display.setCursor(0, 0);
      display.println("JSON Error");
    }

  } else {
    // ------------- RANDOM JOKE DISPLAY -------------
    int r = random(0, jokeCount);

    display.setCursor(0, 0);
    display.println("Oops!");

    display.setCursor(0, 14);
    display.println(jokes[r]);

    Serial.println("HTTP Error -> Joke: " + jokes[r]);
  }

  display.display();
  http.end();

  delay(5000); // Refresh every 5 sec
}
