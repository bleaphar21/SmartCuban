/**
authors @Brantley Leaphart and Alexander Stewart 
Purpose: This class is responsible for orchastrating the 
board to board communication and sending information over to 
the ios app 
*/
#include "color_vector.h"
#include "color_calculator.h"
//#include "fastpwm.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>
#include <Wire.h>

//Used for the clock functionality
unsigned long startMillis = 0;
unsigned long currentMillis = 0;

int pixels = 12;

//Simple enums representing the functionality modes 
enum MODE {
  color_clock,
  manual,
  real_time
};

MODE currentMode = manual;
int readTemp = 0;
int red = 0;
int green = 0;
int blue = 0;
int currentHour = 0;

MODE prevCurrentMode = manual;
int prevReadTemp = 0;
int prevRed = 0;
int prevGreen = 0;
int prevBlue = 0;

bool readyToSend = false;

// color calc obj
ColorCalculator calc = ColorCalculator();

#define SLAVE_ADDR 9
#define ANSWERSIZE 2

// code to set up bluetooth service and characteristics...
BLEService cubanLinkService("ac4f4e48-c67a-4a85-b98f-385a7bc1fe59"); // our bt service
BLEIntCharacteristic modeChar("862b4730-806e-4133-a965-b4d7dc14b68b", BLERead | BLEWrite | BLENotify | BLEIndicate); // charateristic to tell when mode changes.
BLEIntCharacteristic temperatureCelsius("1101", BLERead | BLEWrite | BLENotify | BLEIndicate);
BLEIntCharacteristic redValue("2101", BLERead | BLEWrite | BLENotify | BLEIndicate);
BLEIntCharacteristic greenValue("3101", BLERead | BLEWrite | BLENotify | BLEIndicate);
BLEIntCharacteristic blueValue("4101", BLERead | BLEWrite | BLENotify | BLEIndicate);

//The arduino nano ble 33
BLEDevice mainCentralDevice;


#define SLAVE_ADDR 9
#define ANSWERSIZE 2

void initStrip() {
  ColorVector c = ColorVector(0,0,0,0);
  for (int i = 0; i < pixels; i++) {
    pushColorForPixel(i, c);
    delay(100);
  }
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  // setting up ble services.
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!!");
    while (1);
  }
  // making service available
  BLE.setLocalName("CubanLink");
  BLE.setAdvertisedService(cubanLinkService);

  //Adding the characteristics that the nano should look for
  cubanLinkService.addCharacteristic(modeChar);
  cubanLinkService.addCharacteristic(temperatureCelsius);
  cubanLinkService.addCharacteristic(redValue);
  cubanLinkService.addCharacteristic(greenValue);
  cubanLinkService.addCharacteristic(blueValue);
  BLE.addService(cubanLinkService);
  BLE.advertise(); // bluetooth device is now active...
  Serial.println("Bluetooth device active!!");

  Wire.begin(); // join i2c bus (address optional for master)
}

/*
 * The main loop containing all of the executable code 
 */
void loop()
{
  if (mainCentralDevice == NULL) {
    Serial.println("It's null");
    mainCentralDevice = BLE.central();
  }

  if (mainCentralDevice.connected()) { // a bluetooth is connected...
    // read values from the characteristics
    byte rt = temperatureCelsius.value();
    readTemp = (int)rt;
    byte pre_x = modeChar.value();
    int x = (int)pre_x;
    switch (x) {
      case 1:
        currentMode = real_time;
        break;
      case 2:
        currentMode = manual;
        break;
      case 3:
        currentMode = color_clock;
        break;
    }
    byte v = redValue.value();
    byte y = greenValue.value();
    byte z = blueValue.value();

  //Turning the bytes into ints that can be used 
    red = (int)v; 
    green = (int)y;
    blue = (int)z;
  }

//Buffer functions 
  if (currentMode != prevCurrentMode) {
    if (currentMode == color_clock) {
      startMillis = millis();
      currentMillis = millis();
      initStrip();
      currentHour = 0;
      ColorVector newColor = calcColorBasedOnTemp(readTemp);
      pushColorForPixel(0, newColor);
    }
  }

  if (currentMode != prevCurrentMode ||
      readTemp != prevReadTemp ||
      red != prevRed ||
      green != prevGreen ||
      blue != prevBlue) {

    prevCurrentMode = currentMode;
    prevReadTemp = readTemp;
    prevRed = red;
    prevBlue = blue;
    prevGreen = green;
    operateMode();
  }

  currentMillis = millis();
  if (currentMillis - startMillis >= 3600000) {
    startMillis = millis();
    currentHour++;
    ColorVector newColor = calcColorBasedOnTemp(readTemp);
    pushColorForPixel(currentHour, newColor);
  }

}

/**
   setColorforPixel will set the color for the specified pixel.
   This function might store the current color the pixel in some data structure.
*/
void pushColorForPixel(int pixel, ColorVector c) {
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #4
  Wire.write(pixel);        // sends five bytes
  Wire.write(c.r);          // sends one byte
  Wire.write(c.g);
  Wire.write(c.b);
  Wire.endTransmission();    // stop transmitting
}

ColorVector calcColorBasedOnTemp(float currentTemp) {
  calc.populateWeatherColors(currentTemp, red, green, blue);

  return ColorVector(red, green, blue, 0);
}

/**
   currentWeatherColor will handle constantly updating the lights of the chain based on the weather.
   read from the sensors.
*/
void currentWeatherColor() {
  Serial.println("current_color_weather");
  ColorVector newColor = calcColorBasedOnTemp(readTemp);

  for (int i = 0; i < pixels; i++) {
    delay(100);
    pushColorForPixel(i, newColor);
    //delay(10000);
  }

}

/**
   function to set the manual color...
*/
void setManualColor() {
  ColorVector c = ColorVector(red, green, blue, 0);

  for (int i = 0; i < pixels; i++) {
    pushColorForPixel(i, c);
    delay(100);
  }
}

void operateMode() {
  switch (currentMode) {
    case color_clock:
      Serial.println("cc");
      break;
    case manual:
      Serial.println("m");
      setManualColor();
      break;
    case real_time:
      Serial.println("r_t");
      currentWeatherColor();
      break;
    default:
      Serial.println("def");
      currentWeatherColor();
      break;
  }
}
