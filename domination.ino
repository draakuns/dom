//*
Project mannheim TOP SECRET!
 */

//**************************INFO***************************************

// Designing a project
// http://forum.arduino.cc/index.php?topic=261445.0

// Revisar para los interrupts 
// delay() and millis() both rely on interrupts, they will not work while an ISR is running. delayMicroseconds()
// http://arduino.cc/en/Reference/AttachInterrupt
// http://gammon.com.au/interrupts

// Variables and constants that hold data connected with millis() should be defined as unsigned long
// const unsigned long ledOnMillis = 300;
// const unsigned long ledAbaseInterval = 500;
// const unsigned long ledBbaseInterval = 1000;
// if (millis() - prevMillis >= interval) {
//   // time has expired so do something
// }

// Start Serial communication
// Serial.begin(9600); (o 115200)
// Good practice: Echo program name
// Serial.println("Starting LessonF.ino");

// http://mattgemmell.com/2008/12/08/what-have-you-tried/

// Hint: Static strings can quickly gobble up SRAM. A simple technique to avoid that is to use the F( ) macro: 
// Serial.println (F("Welcome to my program!"));

// 11. Traps, tips, style guide and more reading.
// http://www.gammon.com.au/tips

// a collection of common beginner trouble spots: http://www.cprogramming.com/tutorial/common.html

// The C++ FAQ Lite: http://www.parashift.com/c++-faq-lite/

//**************************CODE***************************************

/*
Project mannheim TOP SECRET!
 */
#include "LedControl.h"

// Create a new max72XX LedControl for 1 device... 
LedControl pantalla=LedControl(12,11,10,1); 

// Timing constants
const int MAX_POINTS = 240;  // With 5sec interval means 20min of play
const unsigned long INTERVAL = 5000;  //  Time between checks for flag owner, in millis (sec * 1000)
const unsigned long MAX_GAMEPLAY = 1200000;  // Max gameplay time, in millis (min * 60 * 1000)
//const unsigned long BUTTON_PRESS = 3000;  //  Time that must be holded a button to change flag owner, in millis (sec * 1000)

// Pins used
//MAX leds 12
//MAX leds 11
//MAX leds 10
const byte B_TRIGGER = 9;
const byte R_TRIGGER = 8;


// Variable definition
byte R_POINTS;  // RED team current points
byte B_POINTS;  // BLUE team current points
unsigned long currentmillis;  // current time
unsigned long prevmillis;  // previous time
//unsigned long buttonmillis;  // button press time duration
boolean itsthetime;  // currentmillis - prevmillis >= INTERVAL, 1 equals to interval crossed
boolean endgame;  // Marks end of the game if true, starts blink mode
byte teamplus;  // Team to account: 0 = noone, 1 = red, 2 = blue
byte actualview;  // Team score last showed: 0 = time, 1 = red, 2 = blue

// Main setup
void setup() {                

  // initialize the pins and set everyone off (except if required)
  initialize();
  //Serial init if needed for debugging purposes
  Serial.begin(115200);

}

// LOOP PRINCIPAL
void loop() {

  // Update current time
  currentmillis = millis();

  // Check interval and set flag if boundary crossed, then update prevmillis to check interval next loop
  if ( (currentmillis - prevmillis) >= INTERVAL ) {
    itsthetime = 1;
    prevmillis = currentmillis;
  }

  // Only work every interval
  if ( itsthetime == true ) {

    //If MAX_GAMEPLAY or MAX_POINTS reached, set variable to blink later on
    if ( (currentmillis >= MAX_GAMEPLAY) || (B_POINTS == MAX_POINTS) || (R_POINTS == MAX_POINTS) ) {
      endgame = true;
      teamplus = 0;  //Stops pointing to let the screen show final stats
      AÑADIR AQUI EL MUESTRARIO DE TIEMPO, RPUNTOS, BPUNTOS en blinks
    }
    //Else, continue normal execution.
    else {
      increasepoints()  //Increases the right counter 
      shownextpoints();  //Show next team/time
      
    }
  }
  
  // Monitor pullup switches (outside of the interval-only working)
  if ( R_TRIGGER == LOW ) {
    teamplus = 1;
    actualview = 0;  //Forces actualview to display red counter next
  }
  else if ( B_TRIGGER == LOW) {
    teamplus = 2;
    actualview = 1;  //Forces actualview to display blue counter next
  }
}

// Initializes pins to type and required state.
void initialize() {

  //Pins init
  pinMode(R_TRIGGER,INPUT_PULLUP);	//Red switch
  pinMode(B_TRIGGER,INPUT_PULLUP);	//Blue switch

  // Powerup max72XX (0 is the dev number, if stacked could be 1,2...) and set intensity to half brightness.
  pantalla.shutdown(0,false);
  pantalla.setIntensity(0,8);
  pantalla.clearDisplay(0);	// Comment this line for debugging purposes, as it lit every led on display.

  // Initialize values to its start value
  B_POINTS = 0;
  R_POINTS = 0;
  prevmillis = 0;
  itsthetime = 0;
  endgame = false;
  teamplus = 0;
  actualview = 0;
}

// This function rotates and shows time-red-blue points (with 2 displays gets obsolete)
void shownextpoints() {
  switch (actualview) {
  case 0:
    actualview = 1;
    printNumber(R_POINTS);
    break;
  case 1:
    actualview = 2;
    printNumber(B_POINTS);
    break;
  case 2: 
    actualview = 0;
    printNumber(millis() / 2);
  }
}  

//Printnumber function from Arduino playground
// ¿¿¿¿¿¿¿¿¿How to implement "r" and "A" on fourth digit, or lit a LED for every team on dashboard
// And what happens if writing numbers lower than 100?
void printNumber(int v) {
  int ones;
  int tens;
  int hundreds;
  ////    boolean negative;	

  if(v < 0 || v > 999) 
    return;
  ////    if(v<0) {
  ////        negative=true;
  ////        v=v*-1;
  ////    }
  ones=v%10;
  v=v/10;
  tens=v%10;
  v=v/10;
  hundreds=v;
  ////    if(negative) {
  ////       //print character '-' in the leftmost column	
  ////       pantalla.setChar(0,3,'-',false);
  ////    }
  ////    else {
  ////       //print a blank in the sign column
  ////       pantalla.setChar(0,3,' ',false);
  ////    }
  //Now print the number digit by digit
  pantalla.setDigit(0,2,(byte)hundreds,false);
  pantalla.setDigit(0,1,(byte)tens,false);
  pantalla.setDigit(0,0,(byte)ones,false);
}
