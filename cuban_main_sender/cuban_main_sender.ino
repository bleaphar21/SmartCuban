/**
 * 
 * add our explaination here..
 */
#include "color_vector.h"
#include "color_calculator.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>
#include <Wire.h>

int pixels = 24;

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

BLEDevice mainCentralDevice;


#define SLAVE_ADDR 9
#define ANSWERSIZE 2

void setup()
{
  Serial.begin(9600);
  while(!Serial);

  // setting up ble services.
  if(!BLE.begin()) {
    Serial.println("starting BLE failed!!");
    while(1);
  }
  // making service available
  BLE.setLocalName("CubanLink");
  BLE.setAdvertisedService(cubanLinkService);
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

void loop()
{
  delay(1000);
  if(mainCentralDevice == NULL) {
    Serial.println("It's null");
    mainCentralDevice = BLE.central();
  }

  if(mainCentralDevice.connected()) { // a bluetooth is connected...
    Serial.println("A device is connected...");
    // read values from the characteristics
    readTemp = temperatureCelsius.value();
    currentMode = (MODE)modeChar.value();
    red = redValue.value();
    green = greenValue.value();
    blue = blueValue.value();
    
    Serial.println("readTemp: " + readTemp);
    Serial.println("currentMode: " + currentMode);
    Serial.println("red: " + red);
    Serial.println("green: " + green);
    Serial.println("blue: " + blue);
    // might need one more characteristic that has the rgb values for manual mode...
    // pass values to diff functions based on the mode.
  }

  operateMode();
}

/**
 * setColorforPixel will set the color for the specified pixel. 
 * This function might store the current color the pixel in some data structure.
 */
void pushColorForPixel(int pixel, ColorVector c) {
//  Serial.println("Pixel: " + pixel);
//  Serial.println("Red: " + c.r);
//  Serial.println("Green: " + c.g);
//  Serial.println("Blue: " + c.b);
  Serial.println("This is the current color vector: ");
  Serial.println((String) c.r);
  Serial.println((String) c.g);
  Serial.println((String) c.b);
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #4
  Wire.write(pixel);        // sends five bytes
  Wire.write(c.r);          // sends one byte  
  Wire.write(c.g);
  Wire.write(c.b);
  Wire.endTransmission();    // stop transmitting
  delay(1000);
}

ColorVector calcColorBasedOnTemp(float currentTemp) {
  int red;
  int green;
  int blue;

  calc.populateWeatherColors(currentTemp, red, green, blue);

  return ColorVector(red, green, blue, 0);
}

/**
 * currentWeatherColor will handle constantly updating the lights of the chain based on the weather.
 * read from the sensors.
 */
void currentWeatherColor() {
  //Serial.println("current_color_weather");
  ColorVector newColor = calcColorBasedOnTemp(readTemp);

  for (int i = 0; i < pixels; i++) {
    pushColorForPixel(i, newColor);
  }
}

/**
 * function to set the manual color...
 */
void setManualColor() {
  ColorVector c = ColorVector(50, 50, 0, 0);

   for (int i = 0; i < pixels; i++) {
    pushColorForPixel(i, c);
  }
}

void operateMode() {
  //Serial.println("operating");
  switch(currentMode) {
    case color_clock:
      Serial.println("cc");
      // statements
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
