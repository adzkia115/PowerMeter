#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

int lcdColumns = 20;
int lcdRows = 4;
int ledPin = 2;
int pirPin = 36;
bool ledState = LOW;
bool pirStat = LOW; 


String chat_id2 = "5430299098";

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

// Replace with your network credentials
const char* ssid = "Android 14";
const char* password = "mintamulu";
WiFiClientSecure client;


// Initialize Telegram BOT
#define BOTtoken "7574239427:AAFDOeEdJWiBzxBaYAt3rY5dAOuaAJ4K_6A"  // your Bot Token (Get from Botfather)
#define CHAT_ID "5430299098"
UniversalTelegramBot bot(BOTtoken, client);




// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  Serial.println(F("DHT11 Start"));
  dht.begin();


    // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}


void loop() 
{
  pirStat = digitalRead(pirPin);
  if (pirStat == HIGH) 
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("Hey I got you!!!");
  }
  else 
  {
    digitalWrite(ledPin, LOW);
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();         // Read temperature as Celsius (the default)
  float t = dht.readTemperature();      // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println(f);
  
  lcd.setCursor(0, 0);  // set cursor to first column, first row
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("'C");
  
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(0,2);
  lcd.print("Human: ");
  lcd.print(pirStat);

  delay(5000);
  lcd.clear(); 

  if (t > 35)
  {
    Serial.println("Send OH");
    // bot.sendMessage(chat_id2, "OverHeat", "");
    delay(500);
  }
}