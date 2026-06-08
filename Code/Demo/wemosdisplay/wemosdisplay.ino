#include <Arduino.h>
#include <TM1637Display.h>
#include "LedControl.h"
#include "binary.h"
// TM1637
#define TM_CLK D1
#define TM_DIO D2

// MAX7219
#define MAX_DIN D7
#define MAX_CLK D5
#define MAX_CS D8
#define MAX_count 1
unsigned long delaytime = 1000;
TM1637Display segdisplay(TM_CLK, TM_DIO);
LedControl matrixdisplay = LedControl(D7, D5, D8, 1);


byte hf[8] = { B00111100, B01000010, B10100101, B10000001, B10100101, B10011001, B01000010, B00111100 };
// neutral face
byte nf[8] = { B00111100, B01000010, B10100101, B10000001, B10111101, B10000001, B01000010, B00111100 };
// sad face
byte sf[8] = { B00111100, B01000010, B10100101, B10000001, B10011001, B10100101, B01000010, B00111100 };


void setup() {
  for (int address = 0; address < MAX_count; address++) {
    matrixdisplay.shutdown(address, false);
    matrixdisplay.setIntensity(address, 8);
    matrixdisplay.clearDisplay(address);
  }
  segdisplay.setBrightness(0x0f);
}
void drawFaces() {
  // Display sad face
  matrixdisplay.setRow(0, 0, sf[0]);
  matrixdisplay.setRow(0, 1, sf[1]);
  matrixdisplay.setRow(0, 2, sf[2]);
  matrixdisplay.setRow(0, 3, sf[3]);
  matrixdisplay.setRow(0, 4, sf[4]);
  matrixdisplay.setRow(0, 5, sf[5]);
  matrixdisplay.setRow(0, 6, sf[6]);
  matrixdisplay.setRow(0, 7, sf[7]);
  delay(delaytime);

  // Display neutral face
  matrixdisplay.setRow(0, 0, nf[0]);
  matrixdisplay.setRow(0, 1, nf[1]);
  matrixdisplay.setRow(0, 2, nf[2]);
  matrixdisplay.setRow(0, 3, nf[3]);
  matrixdisplay.setRow(0, 4, nf[4]);
  matrixdisplay.setRow(0, 5, nf[5]);
  matrixdisplay.setRow(0, 6, nf[6]);
  matrixdisplay.setRow(0, 7, nf[7]);
  delay(delaytime);

  // Display happy face
  matrixdisplay.setRow(0, 0, hf[0]);
  matrixdisplay.setRow(0, 1, hf[1]);
  matrixdisplay.setRow(0, 2, hf[2]);
  matrixdisplay.setRow(0, 3, hf[3]);
  matrixdisplay.setRow(0, 4, hf[4]);
  matrixdisplay.setRow(0, 5, hf[5]);
  matrixdisplay.setRow(0, 6, hf[6]);
  matrixdisplay.setRow(0, 7, hf[7]);
  delay(delaytime);
}

void loop() {
  // True of false geeft aan of er leading zero's aanwezig zijn of niet.
  segdisplay.showNumberDec(123, true);
  delay(delaytime);
  segdisplay.showNumberDec(123, false);
  delay(delaytime);
  segdisplay.showNumberDec(0000, false);
  delay(delaytime);
  drawFaces();

  delay(delaytime);
}