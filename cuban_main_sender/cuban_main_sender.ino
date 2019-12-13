/**

   add our explaination here..
*/
#include "color_vector.h"
#include "color_calculator.h"
//#include "fastpwm.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>
#include <Wire.h>

#define SAMPLE_RATE 8000 //number of ticks in a second 
volatile unsigned long timer_count = 0; //Value of the timer

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

BLEDevice mainCentralDevice;


#define SLAVE_ADDR 9
#define ANSWERSIZE 2

//// This function is the Interrupt Service Routine (ISR), which is called
//// every time an interrupt occurs for
//ISR(TIMER1_COMPA_vect) {
//  timer_count++;
//  //28800000 represents the time in ticks for an hour
//  if ((timer_count % 28800000) == 0) {
//    currentHour++;
//    ColorVector newColor = calcColorBasedOnTemp(readTemp);
//    pushColorForPixel(0, newColor);
//  }
//}

//void init_timer(void) // Call this function from setup()
//{
//  noInterrupts(); // Disable all interrupts
//  // Clear Timer1 register configuration
//  TCCR1A = 0; TCCR1B = 0;
//  // Configure Timer1 for CTC mode (WGM = 0b0100)
//  bitSet(TCCR1B , WGM12);
//  // Disable prescaler
//  bitSet(TCCR1B , CS10);
//
//  // Set timer period, which is defined as the number of
//  // CPU clock cycles (1/16MHz) between interrupts - 1
//  OCR1A = (F_CPU / SAMPLE_RATE) - 1;
//
//  // Enable timer interrupts when timer count reaches value in OCR1A
//  bitSet(TIMSK1 , OCIE1A);
//
//  interrupts(); // Enable interrupts
//}

void setup()
{
  Serial.begin(9600);
  while (!Serial);

//  init_timer();
//  fastpwm_init();

  // setting up ble services.
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!!");
    while (1);
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
  //delay(1000);
  if (mainCentralDevice == NULL) {
    Serial.println("It's null");
    mainCentralDevice = BLE.central();
  }

  if (mainCentralDevice.connected()) { // a bluetooth is connected...
    //Serial.println("A device is connected...");
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

    red = (int)v;
    green = (int)y;
    blue = (int)z;

    //    Serial.print("readTemp: ");
    //    Serial.println(readTemp);
    //    Serial.print("currentMode: ");
    //    Serial.println(currentMode);
    //    Serial.print("red: ");
    //    Serial.println(v);
    //    Serial.print("green: ");
    //    Serial.println(y);
    //    Serial.print("blue: ");
    //    Serial.println(z);
    // might need one more characteristic that has the rgb values for manual mode...
    // pass values to diff functions based on the mode.
  }

  if (currentMode != prevCurrentMode ||
      readTemp != prevReadTemp ||
      red != prevRed ||
      green != prevGreen ||
      blue != prevBlue) {
//    if (currentMode == color_clock) {
//      timer_count = 0;
//      currentHour = 0;
//      ColorVector newColor = calcColorBasedOnTemp(readTemp);
//      pushColorForPixel(0, newColor);
//    }
    prevCurrentMode = currentMode;
    prevReadTemp = readTemp;
    prevRed = red;
    prevBlue = blue;
    prevGreen = green;
    operateMode();
  }

}

/**
   setColorforPixel will set the color for the specified pixel.
   This function might store the current color the pixel in some data structure.
*/
void pushColorForPixel(int pixel, ColorVector c) {
  //  Serial.println("Pixel: " + pixel);
  //  Serial.println("Red: " + c.r);
  //  Serial.println("Green: " + c.g);
  //  Serial.println("Blue: " + c.b);
  //  Serial.println("This is the current color vector: ");
  Serial.println((String) c.r);
  Serial.println((String) c.g);
  Serial.println((String) c.b);
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #4
  Wire.write(pixel);        // sends five bytes
  Wire.write(c.r);          // sends one byte
  Wire.write(c.g);
  Wire.write(c.b);
  Wire.endTransmission();    // stop transmitting
  //delay(1000);
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
    //    while(Wire.requestFrom(SLAVE_ADDR, 1)){
    //      Serial.println("we are waiting...");
    //    }
    //delay(5000);
  }
}

void operateMode() {
  //Serial.println("operating");
  switch (currentMode) {
    case color_clock:
      //Serial.println("cc");
      // statements
      break;
    case manual:
      Serial.println("m");
      setManualColor();
      break;
    case real_time:
      //Serial.println("r_t");
      currentWeatherColor();
      break;
    default:
      Serial.println("def");
      currentWeatherColor();
      break;
  }
}
