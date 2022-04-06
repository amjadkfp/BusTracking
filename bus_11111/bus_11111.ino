#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(2, 0); // The serial connection to the GPS device
const char* ssid = "optimus";
const char* password = "pass1234";
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;
//int numreadings = 10;
String latarray[20];
String lngarray[20];
//String current_latarray[10], current_lngarray[10], previous_latarray[10], previous_lngarray[10] ;
unsigned int i = 0;
const unsigned long Interval = 13000;
unsigned long previousTime = 0;
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  delay(2000); // Pause for 2 seconds
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}
void loop()
{
  unsigned long currentTime = millis();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    { 
     if (currentTime - previousTime >= Interval) { 
      if (gps.location.isValid())
      {
        //Serial.print("Getting Data");
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        latarray[i] = lat_str;
        lngarray[i]= lng_str;
        i++;
        Serial.print(i);
        if (i>=20)
       // {
          i=0; //reset to beginning of array, so you don't try to save readings outside of the bounds of the array
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
