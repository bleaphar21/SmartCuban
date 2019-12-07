/* Created by - Brantley Leaphart and Alex Stewart
 *  December 2019
 *  
 *  Important Notes- Please make sure to include the following programs
 *  into your arduino IDE in order to make changes and update the code:
 *    - Boards
 *      - Arduino Nano 33 Sense (tools -> board -> board manager)
 *      
 *    - Libraries
 *      - HTS221 (Temp and Humidity)
 *      - APDS9960 (Gestures)
 *      -Adafruit NeoPixels
 *      
 *      
 *      Shit that needs to get done:
 *        1. We need to have the three modes and figure out how to put them onto the LEDs
 *          - Color clock:
 *            How are we going to schedule the lights to individually light up based on the hour? Do we need the timing stuff from cuckoo?
 *            Send stuff timely from the app?
 *          - Manual Mode: Mode when the lights are controllable, and changes the colors by some RGB value
 *          - Real Time Weather Color: Changes the LED colors based on the weather read by the Arduino sensors.
 *        2. Function to handle turning celcius temp into a color.
 *      
 * 
 */
#include "color_vector.h"
#include "color_calculator.h"
#include <Arduino_HTS221.h>
#include <Arduino_APDS9960.h>
#include <Adafruit_NeoPixel.h>

// Number of pixels that we have.
int pixels = 9;

// inputs: number of pixels, pin number out, color ordering
Adafruit_NeoPixel strip(pixels, 11, NEO_GRB); //  + NEO_KHZ800

// color calc obj
ColorCalculator calc = ColorCalculator();

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initializing sensors
  HTS.begin(); // initializing temp and humidity sensors
  APDS.begin(); // initializing gestures

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  
}

/**
 * getTemperature is a function that will return the current Temperature being read by the Arduino.
 * return - returns the float representing the current temperature. (in celcius)
 */
float getTemperature() {
  return HTS.readTemperature();
}

/**
 * getHumidity is a function that will return the current Humidity being read by the Arduino.
 * return - returns the float representing the current Humidity. (percentage)
 */
float getHumidity() {
  return HTS.readHumidity();
}

/**
 * getGesture is a function that will return the current Gesture being read by the Arduino.
 * return - returns the int representing the current Gesture. -1 if there is no available gesture.
 * Possible gestures are:
 *    GESTURE_UP - “up” gesture
 *    GESTURE_DOWN - “down” gesture
 *    GESTURE_LEFT - “left” gesture
 *    GESTURE_RIGHT - “right” gesture
 *    GESTURE_NONE - the gesture doesn’t match any of the above
 */
int getGesture() {
  if (APDS.gestureAvailable()) {
    // a gesture was detected
    return APDS.readGesture();
  }
  return -1;
}


/**
 * getSensorColors is a function that will return the current Colors being read by the Arduino.
 * return - returns a Color class that contains the red, green, blue, and alpha values from the sensor.
 * If the sensor is not available, returns a transparent color (0,0,0,0).
 */
ColorVector getSensorColors() {
  if (APDS.colorAvailable()) {
    int r, g, b, a;
    // read the color
    APDS.readColor(r, g, b, a);
    return ColorVector(r, g, b, a);
  }
  return ColorVector(0,0,0,0);
}

/**
 * setColorforPixel will set the color for the specified pixel. 
 * This function might store the current color the pixel in some data structure.
 */
void setColorForPixel(int pixel, ColorVector c) {
  strip.setPixelColor(pixel, c.g, c.r, c.b);
}

ColorVector calcColorBasedOnTemp(float currentTemp) {
  int red;
  int green;
  int blue;

  calc.populateWeatherColors(currentTemp, red, green, blue);

  return ColorVector(red, green, blue, 255);

  
}

/**
 * currentWeatherColor will handle constantly updating the lights of the chain based on the weaither.
 * 
 */
void currentWeatherColor() {
  float currentTemp = getTemperature();
  ColorVector newColor = calcColorBasedOnTemp(currentTemp);

  for (int i = 0; i < pixels; i++) {
    setColorForPixel(i, newColor);
  }
  
}


/**
 * This function will pull data back from the App.
 * I don't know how this will work lol.
 */
void pullData() {
  
}

/**
 * A function that will handle posting all relevant data needed for the IOS application.
 * Maybe will create class that will pack all info into, and this function
 * will take in this claass and will send to app over bluetooth/wifi...
 */
void postData(){}
