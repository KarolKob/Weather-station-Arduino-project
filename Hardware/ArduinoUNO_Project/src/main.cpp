#include <Arduino.h>
#include "DHT.h"
#include "rgb_lcd.h"

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 255;

int cnt = 0;

// The value indicates what will be displayed on the LCD
int temp_hum_light = 0; // temp - 0, hum - 1, light - 2

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);

  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin(); // initialize the sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // wait a few seconds between measurements.
  delay(10);

  // Value to display on the LCD
  String disp, uartStr;

  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(A3);

  uartStr = "  " + String(analogValue) + " ";

  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 10) {
    uartStr += "Dark ";
    disp = "Dark";
  } else if (analogValue < 300) {
    uartStr += "Dim ";
    disp = "Dim";
  } else if (analogValue < 500) {
    uartStr += "Light ";
    disp = "Light";
  } else if (analogValue < 700) {
    uartStr += "Bright ";
    disp = "Bright";
  } else {
    uartStr += "Very bright ";
    disp = "Very bright";
  }

  // read humidity
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  float tempC = dht.readTemperature();

  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    uartStr += "? ?\r";
  } else {
    uartStr += String(humi) + " " + String(tempC) + "\r";
  }

  // send data through UART
  Serial.print(uartStr);

  if (cnt == 100)
  {
    cnt = 0;

    // Set the display value
    if (temp_hum_light == 0)
    {
      disp = "Temp.: " + String(tempC) + "°C";
      temp_hum_light = 1;
    }
    else
    {
      if (temp_hum_light == 1)
      {
        disp = "Humidity: " + String(humi) + "%";
        temp_hum_light = 2;
      }
      else
      {
        temp_hum_light = 0;
      }
    }

    // clear the screen
    lcd.clear();

    // display each character to the LCD
    lcd.print(disp);
  }
  
  cnt++;

}