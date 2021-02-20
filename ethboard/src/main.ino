#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <ArduinoJson.h>
#import "config.h"

#include <WiFi.h>
#include <HTTPClient.h>

LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

const char *ssid = "allthethings";
const char *password = "thisisathingz";

#define LED_PIN SCK
#define NUMPIXELS 4

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

#define KEY1 33
#define KEY2 13
#define KEY3 26
#define KEY4 12
#define KEY5 32
#define KEY6 25
#define KEY7 27
#define KEY8 14
#define KEY9 21

// taken from
#define KEY_SPACE 0x2C
#define KEY_SHIFT 0xE1
#define KEY_TAB 0x2B

const int keys[] = {
    KEY1,
    KEY2,
    KEY3,
    KEY4,
    KEY5,
    KEY6,
    KEY7,
    KEY8,
    KEY9,
};

int show = -1;

struct coin
{
  int indx = -1;
  String name = " loading ";
  String value = " loading ";
};

coin FirstCoin;
coin SecondCoin;
coin ThirdCoin;

DynamicJsonDocument doc(1024);

void setup()
{

  Serial.begin(115200);
  Serial.println("wifi lcd");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

  HTTPClient http;
  http.begin(CONFIG_URL_UNIT);
  int httpCode = http.GET();
  if (httpCode == 200)
  {
    String payload = http.getString();
    Serial.print("has doc: ");
    Serial.println(payload);
    deserializeJson(doc, payload);
  } else {
    Serial.println("no response");
  }

  pixels.begin();
  pixels.fill(pixels.Color(4, 0, 0));

  int error;


  for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
    pinMode(keys[i], INPUT_PULLUP);
  }

  Serial.println("LCD...");

  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);



  if (error == 0)
  {
    Serial.println(": LCD found.");
    show = 0;
    lcd.begin(16, 2); // initialize the lcd
  }
  else
  {
    Serial.println(": LCD not found.");
  } // if



  lcd.setBacklight(200);
  lcd.home();
  lcd.clear();
  lcd.print("~ numbermatic ~");
  lcd.setCursor(0, 1);
  lcd.print("~ ~ ~ ~ ~ ~ ~");

} // setup()

#define WHEN_PRESSED(key) \
  if (isPressed(key))     \
  {                       \
  }

bool isPressed(short key)
{
  return !digitalRead(key);
}

void whenPressed(short key, void (*run)())
{
  if (digitalRead(key))
  {
    return;
  }
  pixels.fill(pixels.Color(20, 20, 20), 0, 4);
  pixels.show();
  run();
  int counter = 0;
  while (counter++ < 60)
  {
    if (digitalRead(key))
    {
      break;
    }
    delay(1);
  }
  pixels.fill(pixels.Color(0, 0, 0), 0, 4);
  pixels.show();
}

void renderQuote(int id) {
  lcd.home();
  lcd.clear();
  String quoteName = doc["quotes"][id]["name"];
  const char* quoteValue = doc["quotes"][id]["quote"];
  lcd.print(quoteName);
  lcd.setCursor(0, 1);
  lcd.print(quoteValue);
}

void Key1Handler()
{
  renderQuote(0);
}

void Key2Handler()
{
  renderQuote(1);
}

void Key3Handler()
{
  renderQuote(2);
}

void Key4Handler()
{
  renderQuote(3);
}

void Key5Handler()
{
  // pass

  // step 1 activate keys
  // step 2 tab to the mute button
  // step 3 press the mute button
  // step 4 leave screen focus
  pixels.fill(pixels.Color(200, 0, 0));
  pixels.show();
}

void Key6Handler()
{
  renderQuote(4);
}

void Key7Handler()
{
  renderQuote(5);
}

void Key8Handler()
{
  renderQuote(6);
}

void Key9Handler()
{
  renderQuote(7);
}

void loop()
{
  pixels.fill(pixels.Color(4, 4, 4), 0, 4);
  pixels.show();

  whenPressed(KEY1, Key1Handler);
  whenPressed(KEY2, Key2Handler);
  whenPressed(KEY3, Key3Handler);
  whenPressed(KEY4, Key4Handler);
  whenPressed(KEY5, Key5Handler);
  whenPressed(KEY6, Key6Handler);
  whenPressed(KEY7, Key7Handler);
  whenPressed(KEY8, Key8Handler);
  whenPressed(KEY9, Key9Handler);
} // loop()
