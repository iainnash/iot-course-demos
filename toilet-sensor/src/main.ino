#include <M5StickC.h>
#include "config.h"

// accelerometer data
float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
// end data

AdafruitIO_Feed *seatUp = io.feed("seat-up");

// connects adafruit io
void connectIo()
{
  Serial.begin(115200);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void setupDisplay()
{
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("!!toilettime!!");
}

void setup()
{
  connectIo();
  setupDisplay();
  // init position sensor
  M5.MPU6886.Init();
  // set button as input
  pinMode(39, INPUT);
}

int transitions = 0;
long isUpMillis = -1;
void loop()
{
  // put your main code here, to run repeatedly:
  M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.MPU6886.getAccelData(&accX, &accY, &accZ);

  if (digitalRead(37))
  {
    // publish cleaned
    // set cleaned date
  }

  if (accX * 1000 > 500)
  {
    if (isUpMillis == -1)
    {
      // set flag and timer
      isUpMillis = millis();
      // count times moved up
      transitions++;
    }
  }
  else
  {
    isUpMillis = -1;
  }

  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextSize(2);
  if (isUpMillis != -1)
  {
    // up for more than 20 minutes
    int upDuration = (millis() - isUpMillis) / 1000;
    M5.Lcd.printf("Up for: \n%dm %ds", (int)upDuration / 60, (int)(upDuration % 60));
    if (upDuration % 60 == 0)
    {
      seatUp->save(upDuration / 60);
    }
  }
  else
  {
    M5.Lcd.printf("down :)");
  }

  // pressed down, reset uses counter
  if (!digitalRead(39))
  {
    transitions = 0;
  }

  M5.Lcd.setCursor(400, 40);
  M5.Lcd.printf("uses: %d", transitions);

  delay(1000);
}
