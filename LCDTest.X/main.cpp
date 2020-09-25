/* 
 * File:   main.cpp
 * Author: emilymcdonnell
 *
 * Created on September 22, 2020, 10:28 AM
 */

#define __dsPIC33FJ256GP710A__
#include <cstdlib>
#include "/Applications/microchip/mplabx/v5.40/packs/Microchip/MEC15xx_DFP/1.3.60/mec152x/xc32/include/xc.h"

using namespace std;
#include <iostream>
//#include <SPI.h>
//#include "Adafruit_GFX.h"
//#include "Adafruit_HX8357.h"
#include "/Applications/microchip/xc16/v1.60/support/dsPIC33F/h/p33Fxxxx.h"

// These are 'flexible' lines that can be changed
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// SoftSPI - note that on some processors this might be *faster* than hardware SPI!
//Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, MOSI, SCK, TFT_RST, MISO);
 

int main(int argc, char** argv) {
    cout << "Please work";
    SPI1CON1 = 0x057A;	// 0000 0000 0111 1111
	SPI1STATbits.SPIROV = 0;		// Clear Overflow bit
	SPI1STATbits.SPIEN = 1;	
//    Serial.begin(9600);
//    Serial.println("HX8357D Test!"); 
//
//  tft.begin();
//
//  // read diagnostics (optional but can help debug problems)
//  uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
//  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
//  x = tft.readcommand8(HX8357_RDMADCTL);
//  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
//  x = tft.readcommand8(HX8357_RDCOLMOD);
//  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
//  x = tft.readcommand8(HX8357_RDDIM);
//  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
//  x = tft.readcommand8(HX8357_RDDSDR);
//  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
//  
//  Serial.println(F("Benchmark                Time (microseconds)"));
//
//  tft.setRotation(1);
//
//  Serial.print(F("Text                     "));
//  Serial.println(testText());
//  delay(500);
//
//  Serial.print(F("Lines                    "));
//  Serial.println(testLines(HX8357_CYAN));
//  delay(500);
//
//  Serial.print(F("Rectangles (outline)     "));
//  Serial.println(testRects(HX8357_GREEN));
//  delay(500);
//
//  tft.fillScreen(HX8357_BLACK);
//  Serial.print(F("Circles (outline)        "));
//  Serial.println(testCircles(10, HX8357_RED));
//  delay(500);
//
//
//  Serial.print(F("Triangles (outline)      "));
//  Serial.println(testTriangles());
//  delay(500);
//
//  Serial.print(F("Triangles (filled)       "));
//  Serial.println(testFilledTriangles());
//  delay(500);
//
//
//  Serial.print(F("Rounded rects (outline)  "));
//  Serial.println(testRoundRects());
//  delay(500);
//
//  Serial.print(F("Rounded rects (filled)   "));
//  Serial.println(testFilledRoundRects());
//  delay(500);
//
//  Serial.println(F("Done!"));
    return 0;
}

