#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ----------------------------------
// USER SETTINGS
// ----------------------------------
const char* ssid = "Hasini";
const char* password = "Esp32Dev";

String city = "Hyderabad";
String country = "INDIA";

// Get your API key: https://openweathermap.org/api
String apiKey = "8ac36ae605e5fa47dfa4c159e9dc63be";

// ----------------------------------
int screenIndex = 0;  // auto-scroll index

// ----------------------------------
void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);  // SDA = D20, SCL = D22
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // WiFi Connect
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("WiFi Connected!");
  display.display();
  delay(1000);
}

// ----------------------------------
// Fetch WEATHER + AQI
// ----------------------------------
bool getWeather(float &temp, float &hum, int &aqi) {
  HTTPClient http;

  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + country + "&appid=" + apiKey + "&units=metric";
  http.begin(url);

  int httpCode = http.GET();
  if (httpCode != 200) {
    return false;
  }

  String payload = http.getString();
  http.end();

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, payload);

  temp = doc["main"]["temp"];
  hum  = doc["main"]["humidity"];

  // Now get AQI from OpenWeather Air Pollution API
  float lat = doc["coord"]["lat"];
  float lon = doc["coord"]["lon"];

  String aqiURL = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" 
                    + String(lat) + "&lon=" + String(lon) + "&appid=" + apiKey;

  http.begin(aqiURL);
  httpCode = http.GET();
  if (httpCode != 200) {
    return false;
  }

  String aqiPayload = http.getString();
  http.end();

  DynamicJsonDocument aqiDoc(1024);
  deserializeJson(aqiDoc, aqiPayload);

  aqi = aqiDoc["list"][0]["main"]["aqi"];   // 1–5 scale

  return true;
}

// ----------------------------------
// DISPLAY SCREENS
// ----------------------------------
void showScreen(String title, String line1, String line2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(title);
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  display.setCursor(0, 20);
  display.println(line1);

  display.setCursor(0, 38);
  display.println(line2);

  display.display();
}

// ----------------------------------
void loop() {
  float temp, humidity;
  int aqi;

  if (!getWeather(temp, humidity, aqi)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Error fetching data");
    display.display();
    delay(5000);
    return;
  }

  // AUTO SCROLL SCREENS EVERY 5 SECONDS
  if (screenIndex == 0) {
    showScreen(" WEATHER ", 
               "Temp: " + String(temp) + " C", 
               "Humidity: " + String(humidity) + "%");
  }

  else if (screenIndex == 1) {
    showScreen(" AIR QUALITY ", 
               "AQI Level: " + String(aqi), 
               aqi==1?"Good":
               aqi==2?"Fair":
               aqi==3?"Moderate":
               aqi==4?"Poor":"Very Poor");
  }

  else if (screenIndex == 2) {
    showScreen(" LOCATION ", 
               city + ", " + country, 
               "WiFi OK ✓");
  }

  screenIndex++;
  if (screenIndex > 2) screenIndex = 0;

  delay(5000);  // auto scroll every 5 sec
}
