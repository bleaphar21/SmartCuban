/* Created by - Brantley Leaphart and Alex Stewart
 *  December 2019
 *  
 *  Important Notes- This sketch is specifically for the arduino
 *  responsible for controlling the color of the lights of the program.
 *  The arduino will recieve color information from the "parent arduino"
 *  handling recieving info from bluetooth and sensors.
 *  Please make sure to include the following programs
 *  into your arduino IDE in order to make changes and update the code:
 *    - Boards
 *      - Arduino Nano 33 Sense (tools -> board -> board manager)
 *      
 *    - Libraries
 *      - HTS221 (Temp and Humidity)
 *      - APDS9960 (Gestures)
 *      - Adafruit NeoPixels
 *      - ArduinoBLE
 *      
 *      
 */
#include "color_vector.h"
#include "color_calculator.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// Number of pixels that we have.
int pixels = 12;

byte busy = 0; 

//Simple enums to describe the type of functionality 
enum MODE {
  color_clock,
  manual,
  real_time
};

MODE currentMode = real_time;

#define SLAVE_ADDR 9 //address of the arduino uno
#define ANSWERSIZE 2


// inputs: number of pixels, pin number out, color ordering
Adafruit_NeoPixel strip(pixels, 11, NEO_RGB); //  + NEO_KHZ800

// color calc obj
ColorCalculator calc = ColorCalculator();

//Sets up the strip initially 
void initStrip() {
  ColorVector c = ColorVector(0,0,0,0);
  for (int i = 0; i < pixels; i++) {
    setColorForPixel(i, c);
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // INITING THE BOARD AS THE LISTENER
  Wire.begin(SLAVE_ADDR);

  //Giving information to the master board
  Wire.onRequest(postData);

  //How to process information from the master board
  Wire.onReceive(pullData);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// the loop function runs over and over again forever
void loop() {
  
}

/**
 * setColorforPixel will set the color for the specified pixel. 
 * This function might store the current color the pixel in some data structure.
 */
void setColorForPixel(int pixel, ColorVector c) {  
  strip.setPixelColor(pixel, c.g, c.r, c.b);
  strip.show();
}

/**
 * This function will pull data back from the App through 
 * the master board and interprets the information into bytes
 * that can be used to color the necklace
 * 
 */
void pullData(int howMany) {
  // pulling data while the connection is open
  busy = 1;
  String arr[16];
  int i = 0;
  while(Wire.available()) {
     arr[i] = (String)Wire.read();
     i++;
  }

  int pixelNum = arr[0].toInt();
  int red = arr[1].toInt();
  int green = arr[2].toInt();
  int blue = arr[3].toInt();

  // putting the color for the pixel
  setColorForPixel(pixelNum, ColorVector(red, green, blue, 0));
  busy = 0;
}

/**
 * A functionhandles posting all relevant data needed for the IOS application back to the master
 */
void postData() {
  Wire.write(busy);
}
