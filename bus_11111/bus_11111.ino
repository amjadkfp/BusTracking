#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, -1);
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(3, 0); // The serial connection to the GPS device
const char * ssid = "AMJAD";
const char * password = "123456789";
float latitude, longitude;
String lat_str, lng_str;
String latarray[20];
String lngarray[20];
unsigned int i = 0;
unsigned long Interval = 13000;
unsigned long previousTime = 0;

void displayPrint(String text, unsigned int line = 1) {
  display.setCursor(0, line * 10);
  display.println(text);
  display.display();
}

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  Serial.begin(115200);
  ss.begin(9600);
  delay(2000); // Pause for 2 seconds
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    displayPrint("Connecting to WIFI...", 2);
    displayPrint(ssid, 3);
    displayPrint("OXOX", 4);
    delay(500);
    display.clearDisplay();
    displayPrint("Connecting to WIFI...", 2);
    displayPrint(ssid, 3);
    displayPrint("XOXO", 4);
    delay(500);
  }
  display.clearDisplay();
  displayPrint("WiFi Connected!", 2);
}

void checkandreadGPS() {
  unsigned long currentTime = millis();
  if (gps.encode(ss.read())) {
    if (currentTime - previousTime >= Interval) {
      if (gps.location.isValid()) {
        //Serial.print("Getting Data");
        latitude = gps.location.lat();
        lat_str = String(latitude, 6);
        longitude = gps.location.lng();
        lng_str = String(longitude, 6);
        latarray[i] = lat_str;
        lngarray[i] = lng_str;
        i++;
        Serial.print(i);
        if (i >= 20)
          // {
          i = 0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array
        // } 
        Serial.println("Latitude:");
        Serial.println(latarray[i]);
        Serial.println("Longitude:");
        Serial.println(lngarray[i]);
        Serial.println();
        display.setCursor(0, 20);
        display.println("Lat:");
        display.setCursor(27, 20);
        display.println(lat_str);
        display.setCursor(0, 40);
        display.println("Lng:");
        display.setCursor(27, 40);
        display.println(lng_str);
        display.display();
      }
      previousTime = currentTime;
    }
    //}

  }
}

void loop() {

  display.clearDisplay();
  if(WiFi.status() == WL_CONNECTED) {
    displayPrint("WiFi : Connected", 2);
    } else {
    displayPrint("WiFi Disconnected!", 2);
    displayPrint("== Turn on WiFi ==" ,3);
    displayPrint("SSID:" + String(ssid), 4);
    displayPrint("PWD:" + String(password) , 5);
    return ;
  }
  
  
// get configurations from api. 
  while (ss.available() > 0)
    checkandreadGPS();
  // post cordinated to api.
  // check wifi connection.
}
