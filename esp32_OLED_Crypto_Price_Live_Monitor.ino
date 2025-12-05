#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// API URL
const String apiURL =
  "https://api.coingecko.com/api/v3/simple/price?ids="
  "bitcoin,ethereum,solana,binancecoin,grass,pi-network"
  "&vs_currencies=usd";

// Display names
String coinNames[6] = {"BTC", "ETH", "SOL", "BNB", "GRASS", "PI"};

// JSON keys from API
String jsonKeys[6] = {
  "bitcoin",
  "ethereum",
  "solana",
  "binancecoin",
  "grass",
  "pi-network"
};

StaticJsonDocument<1024> doc;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {

  HTTPClient http;
  http.begin(apiURL);
  int httpCode = http.GET();

  display.clearDisplay();

  if (httpCode == 200) {
    String payload = http.getString();

    if (deserializeJson(doc, payload) == DeserializationError::Ok) {

      for (int i = 0; i < 6; i++) {
        float price = doc[jsonKeys[i]]["usd"];

        // ---- Added Line Spacing ----
        display.setCursor(0, i * 11);  // 10px per line (clean spacing)

        display.print(coinNames[i]);
        display.print(": $");
        display.print(price);
      }

    } else {
      display.setCursor(0, 0);
      display.println("JSON parse error");
    }
  } else {
    display.setCursor(0, 0);
    display.println("API error");
  }

  display.display();
  http.end();

  delay(1000);  // update every 5 seconds
}
