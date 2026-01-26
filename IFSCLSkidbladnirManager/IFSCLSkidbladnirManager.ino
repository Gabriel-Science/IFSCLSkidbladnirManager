/*
IFSCL Skidbladnir manager - by Gabriel-Science
An Arduino UNO R4 WiFi program for controlling efficiently the Skidbladnir in IFSCL in Replika missions.
Software under ODbL license.
*/

#include <Keyboard.h>
#include <RGBLed.h>
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;


// Pins' mapping
const int button1 = 12;
const int button2 = 11;
const int button3 = 9;
const int button4 = 8;
const int button5 = 7;
const int buttonMain = 2;
const int redPin = 6;
const int greenPin = 5;
const int bluePin = 3;

// LED matrix's dimensions
const int matrixWidth = 12;
const int matrixHeight = 8;

const int windowOpeningTime = 500;
const int minCommandTime = 25; // 150 if you want stability.
const int brightness = 10;

uint8_t frame[8][12] = {0};
RGBLed led(6, 5, 3, RGBLed::COMMON_CATHODE);

int mode = 0; // 0 is LW selection, 1 is going into VWorld, 2 is TSL, 3 is LW swapping, 4 is devirt and autopilot.
int LW[] = {0, 0, 0, 0, 0}; // Which LG is in mission (Aelita, Odd, Yumi, Ulrich, William)
int LWNumber = 0;
int translatedLW[] = {0, 0, 0, 0, 0};
int translatedLWNumber = 0;
int alreadyTranslatedLWNumber;
int dbqLW[] = {0, 0, 0, 0, 0};
String LWNames[] = {"Aelita", "Odd", "Yumi", "Ulrich", "William"}; // An array of the LW's names


byte LWColours[5][3] = { // The accent colour for each LW
  {255, 125, 125}, // Aelita
  {160, 32, 240}, // Odd
  {132, 27, 45}, // Yumi
  {255, 200, 0}, // Ulrich
  {80, 80, 150}, // William
};


void setup() {
  // put your setup code here, to run once:

  // Pins' mode
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(buttonMain, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  matrix.begin();
  Keyboard.begin(KeyboardLayout_de_DE);
}

void loop() {
  // put your main code here, to run repeatedly:  
  matrix.beginDraw();

  // MODE 0 : LW selction
  while (mode == 0) {
    if (digitalRead(buttonMain) == LOW) {
      mode += 1;
      break;
    }
    if (digitalRead(button1) == LOW) { // If the first button is pressed (Aelita)
      warriorSelection(0, button1);
    }
    if (digitalRead(button2) == LOW) { // If the second button is pressed (Odd)
      warriorSelection(1, button2);
    }
    if (digitalRead(button3) == LOW) { // If the third button is pressed (Yumi)
      warriorSelection(2, button3);
    }
    if (digitalRead(button4) == LOW) { // If the fourth button is pressed (Ulrich)
      warriorSelection(3, button4);
    }
    if (digitalRead(button5) == LOW) { // If the fifth button is pressed (William)
      warriorSelection(4, button5);
    }
    // matrix.renderBitmap(frame, matrixHeight, matrixWidth);
  }


  // MODE 1 : Enter VWorld
  while (mode == 1) {
    // Deactivating scanners 1 and 2.
    enterCommand(windowOpeningTime, "scanners");
    reuseCommand(minCommandTime, "1", 1);
    reuseCommand(minCommandTime, "off", 1);
    reuseCommand(minCommandTime, "2", 1);
    reuseCommand(minCommandTime, "off", 1);

    // Entering into the VWorld and going in front of tower 1.
    enterCommand(19500, "skid.do.entrer");
    enterCommand(14000, "skid.to.1");
    reuseCommand(6800, "do.aligner", 1);

    // Docking and tower activation
    reuseCommand(minCommandTime, "arrimer", 2);
    enterCommand(1300, "transm");
    reuseCommand(windowOpeningTime, "to.tskid", 1);
    reuseCommand(minCommandTime, "exe", 1);

    mode += 1;
  }

  
  // MODE 2 : TSL
  while (mode == 2) {
    enterCommand(minCommandTime, "sktrans.tsl");
    translatedLWNumber = ceil((float)LWNumber / 2);
    for (int i = 0; i < 5; i++) {
      if (LW[i] == 1) {
        alreadyTranslatedLWNumber += 1;
        warriorTranslation(i, 1);
      }      
      if (alreadyTranslatedLWNumber >= translatedLWNumber) {
        break;
      }
    }
    reuseCommand(1700, "exe", 1);
    if (LWNumber > translatedLWNumber) {
      reuseCommand(minCommandTime, "dbq", 1);
      reuseCommand(minCommandTime, "all", 1);
      allLED(0);
      reuseCommand(1700, "exe", 1);      
    }

    while (digitalRead(buttonMain) != LOW) { }
    mode += 1;
  }


  // MODE 3 : TSL swapping
  while (mode == 3) {
    if (LWNumber == translatedLWNumber) {
      mode += 1;
      break;
    }
    enterCommand(minCommandTime, "sktrans.ebq");
    allLED(0);
    reuseCommand(4000, "exe", 1);

    matrix.renderBitmap(frame, matrixHeight, matrixWidth);
    enterCommand(minCommandTime, "sktrans.dtsl");
    for (int i = 0; i < 5; i++) {
      if (translatedLW[i] == 1) {
        alreadyTranslatedLWNumber -= 1;
        warriorTranslation(i, 0);
        dbqLW[i] = 1;
      }  
    }
    translatedLWNumber = 5 -  translatedLWNumber;
    reuseCommand(1850, "exe", 1);
    
    reuseCommand(minCommandTime, "dbq", 1);
    reuseCommand(1700, "exe", 1);      

    reuseCommand(minCommandTime, "tsl", 1);
    reuseCommand(minCommandTime, "all", 1);
    allLED(0);
    reuseCommand(1700, "exe", 1);

    while (digitalRead(buttonMain) != LOW) { }
    mode += 1;
  }


  // MODE 4 : Devirt & autopilot
  while (mode == 4) {
    enterCommand(minCommandTime, "sktrans.dtsl");
    reuseCommand(minCommandTime, "all", 1);
    allLED(0);
    reuseCommand(1700, "exe", 1);
    translatedLWNumber = 0;
    alreadyTranslatedLWNumber = 0;
    
    enterCommand(minCommandTime, "skid.do.desarrimer");
    for (int i = 0; i < 5; i++) {
      frame[1][i] = 0;
      translatedLW[i] = 0;
    }
    matrix.renderBitmap(frame, matrixHeight, matrixWidth);
    enterCommand(minCommandTime, "scanners.1");
    reuseCommand(minCommandTime, "on", 1);
    reuseCommand(minCommandTime, "2", 1);
    reuseCommand(minCommandTime, "on", 1);

    enterCommand(3400, "devirt");
    for (int i = 0; i < 5; i++) {
      if (LW[i] == 1) {
        warriorDevirt(i);
      }
      if (i == 2 || i == 4) {
        reuseCommand(3000, "exe", 1);
      } 
    }
    enterCommand(minCommandTime, "skid.autopilotage.on");
    led.fadeIn(RGBLed::RED, 500, 1000);
    matrix.loadSequence(LEDMATRIX_ANIMATION_CHECK);
    matrix.play(true);
    while (true) { }
  }
}

//-------------------------------------------------------------------------//

// Custom functions

int warriorSelection(unsigned int selectedLW, unsigned int pinNumber) { // Manages the selection from mode 1.
  if (LW[selectedLW] == 0) {
    LW[selectedLW] = 1;
    LWNumber += 1;
  }else {
    LW[selectedLW] = 0;
    LWNumber -= 1;
  }
  frame[0][selectedLW] = LW[selectedLW];
  led.brightness(LWColours[selectedLW][0], LWColours[selectedLW][1], LWColours[selectedLW][2], brightness);
  while (digitalRead(pinNumber) == LOW) { }
  led.off();
  matrix.renderBitmap(frame, matrixHeight, matrixWidth);
  delay(120);
}


int warriorTranslation(unsigned int warrior, unsigned int isTranslation) { // Translates a LW. The warrior variable corresponds to the LWNames index.
  translatedLW[warrior] = isTranslation;
  String tslCommand = LWNames[warrior];
  
  frame[1][warrior] = isTranslation;
  led.brightness(LWColours[warrior][0], LWColours[warrior][1], LWColours[warrior][2], brightness);
  reuseCommand(500, tslCommand, 1);
  led.off();
  matrix.renderBitmap(frame, matrixHeight, matrixWidth);
}


int warriorDevirt(unsigned int warrior) {
  String devirtCommand = LWNames[warrior];

  frame[0][warrior] = 0;
  led.brightness(LWColours[warrior][0], LWColours[warrior][1], LWColours[warrior][2], brightness);
  reuseCommand(2000, devirtCommand, 1);
  led.off();
  matrix.renderBitmap(frame, matrixHeight, matrixWidth);
}


int allLED(unsigned int isTranslation) {
  for (int i = 0; i < 5; i++) {
    if (LW[i] == 1) {
      led.brightness(LWColours[i][0], LWColours[i][1], LWColours[i][2], brightness);
      if (isTranslation = 1) {
        frame[1][i] = 1;
      }
      delay(100);
    }
  }
  led.off();
}


int enterCommand(unsigned int timeBeforeNextAction, String command) { // timeBeforeNextAction corresponds to the time before the next ation in the code can be executed.
  digitalWrite(LED_BUILTIN, HIGH);
  Keyboard.println(command);
  digitalWrite(LED_BUILTIN, LOW);
  delay(timeBeforeNextAction+20);
}


int reuseCommand(unsigned int timeBeforeNextAction, String command, unsigned int howManyTabuations) {
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < howManyTabuations; i++) {
    Keyboard.write(KEY_TAB);
    delay(50);
  }
  enterCommand(timeBeforeNextAction, command);
}
