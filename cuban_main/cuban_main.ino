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
 *      Shit that needs to get done:
 *        1. We need to have the three modes and figure out how to put them onto the LEDs
 *          - Color clock:
 *            How are we going to schedule the lights to individually light up based on the hour? Do we need the timing stuff from cuckoo?
 *            Send stuff timely from the app?
 *          - Manual Mode: Mode when the lights are controllable, and changes the colors by some RGB value
 *          - Real Time Weather Color: Changes the LED colors based on the weather read by the Arduino sensors.
 *        2. Function to handle turning celcius temp into a color.
 *      
 */
#include "color_vector.h"
#include "color_calculator.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

// Number of pixels that we have.
int pixels = 24;

enum MODE {
  color_clock,
  manual,
  real_time
};

MODE currentMode = real_time;

#define SLAVE_ADDR 9
#define ANSWERSIZE 2




// inputs: number of pixels, pin number out, color ordering
Adafruit_NeoPixel strip(pixels, 11, NEO_GRB); //  + NEO_KHZ800

// color calc obj
ColorCalculator calc = ColorCalculator();

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
//  Serial.println("this hoe working");
  pinMode(LED_BUILTIN, OUTPUT);

  // INITING THE BOARD AS THE LISTENER
  Wire.begin(SLAVE_ADDR);

  //Wire.onRequest(postData);

  Wire.onReceive(pullData);

  strip.begin();
  //initStrip();
  strip.show(); // Initialize all pixels to 'off'
}

// the loop function runs over and over again forever
void loop() {
  strip.show();
}

/**
 * setColorforPixel will set the color for the specified pixel. 
 * This function might store the current color the pixel in some data structure.
 */
void setColorForPixel(int pixel, ColorVector c) {
//  Serial.println(pixel);
//  Serial.println(c.r);
//  Serial.println(c.g);
//  Serial.println(c.b);
  
  strip.setPixelColor(pixel, c.g, c.r, c.b);
}

/**
 * This function will pull data back from the App.
 * I don't know how this will work lol.
 * 
 * Order that should be sent and read in:
 *  the led number being updated
 *  red value
 *  green value
 *  blue value
 */
void pullData(int howMany) {
  // pulling data while the connection is open
  String arr[16];
  int i = 0;
  
  //Serial.println("Wire Availability: " + Wire.available());
  while(Wire.available()) {
    Serial.println("receveji");
     arr[i] = (String)Wire.read();
     i++;
  }

  for(int j = 0; j < 4; j++) {
    Serial.println("value at index " + (String)j + ": " + arr[j]);
  }

  
  int pixelNum = arr[0].toInt();
  int red = arr[1].toInt();
  int green = arr[2].toInt();
  int blue = arr[3].toInt();
//  Serial.println("pixel: " + pixelNum);
//  Serial.println("r:" + red);
//  Serial.println("g:" + green);
//  Serial.println("b:" + blue);

  // putting the color for the pixel
  setColorForPixel(pixelNum, ColorVector(red, green, blue, 0));
}

/**
 * A function that will handle posting all relevant data needed for the IOS application.
 * Maybe will create class that will pack all info into, and this function
 * will take in this claass and will send to app over bluetooth/wifi...
 */
void postData(){}
