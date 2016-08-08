/*
Quick Quiz
Easy Setup
No Solder Quiz-Like buzzer game for 1-4 people!
Can all get plugged right in to a simple Arduino uno!
and 1-wire buzzers to anything capacitively touched.

 Cap Touch Version by Andy Quitmeyer 2015 20 dec

Based off a modded version of:
  Quiz-O-Tron 3000
  By Roy Rabey
  Version 1.0 6-DEC-2010
*/

#include <ADCTouch.h>

//Setup all the pins
//Defaults to a 4 player game, could do 6 on an Arduino Uno
int maxPins = 4;                    // Max number of pin sets (1 in and 1 out for each contestant button).

int outputPins[4] = {2, 4, 6, 8}; // The numbers of the LED pins.
int extraGrounds[4] = {3, 5, 7, 9}; // The numbers of the LED pins.
bool useExtraGroundsAsOutput = true; // set this to tie outputPin[n] to extraGrounds[n]

//buzzer makes a beep
int thebuzzer = 11;

int inputPins[4]  = {A0, A1, A2, A3};   // The numbers of the switch pins.
int refs[4] = {0, 0, 0, 0};
int qvalue[4] = {0, 0, 0, 0} ;
//trigger values
bool tvalue[4] = {false,false,false,false};


//  Some variables to control processing
unsigned long WinDelayTime = 4000;            // Number of milliseconds to wait while the buzz LEDs are on.

void buzz(int);                   // Function definition.


//
// Begin processing
//


void setup() {

  // The setup() function is performed once when the Arduino is powered up or reset.

pinMode(thebuzzer, OUTPUT);

  for (int i = 0; i < maxPins; i++) {
    // This tells the Arduino how the pins will be used.
    // Initialize the LED pins.
    pinMode(outputPins[i], OUTPUT);  // Make this an output pin.
    digitalWrite(outputPins[i],HIGH);
    
    pinMode(extraGrounds[i], OUTPUT);  // Make this an output pin.
    if (!useExtraGroundsAsOutput) {
      digitalWrite(extraGrounds[i], LOW);
    } else {
      digitalWrite(extraGrounds[i], HIGH);
    }
    //Initialize the cap touch for all the pins
    refs[i] = ADCTouch.read(inputPins[i], 500);
  }
  // Start the Serial console
  Serial.begin(9600);
  reset();
}



void loop() {

  // The loop() function is executed after the setup() function completes.
  // As the name implies the loop() function loops forever or until the Arduino is reset.

  int val = HIGH;  // Used to determine if an input pin's state has changed to LOW
  for (int p = 0; p < maxPins; p++) {

    captouchProcessor(p);

    // Read the state of each input pin.

    if (tvalue[p] == true) {
      // If a pin goes LOW then someone pressed a button. We have a buzz!
      buzz(p);  // Call the buzz() function with the number of the winning pin set.
    }
  }

  Serial.println();
}


void captouchProcessor(int p) {

  qvalue[p] = ADCTouch.read(inputPins[p], 10);


//Print out all the data
  Serial.print(p);            
  Serial.print(") ref: ");
  Serial.print(refs[p]);
  Serial.print(" raw: ");
  Serial.print(qvalue[p]);
  
    qvalue[p] -= refs[p];       //remove offset
  Serial.print(" val: ");
  Serial.print(qvalue[p]);             //return value

  if(qvalue[p]>130){
    tvalue[p]=true;
  }
   Serial.print(" t: ");
  Serial.print(tvalue[p]);             //return value
  
  Serial.print(" | \t");

}
void buzz(int p) {
  Serial.println();
  Serial.print("**** Buzz in from Player: ");
    Serial.println(p);
    
    // Set the associated output pin to HIGH thus sending power to the winning button's blinking LED circuit.
  digitalWrite(outputPins[p], HIGH);   // Turn the LEDs on
  if (useExtraGroundsAsOutput) {
   digitalWrite(extraGrounds[p], HIGH);
  }
 
 //Buzz the buzzer the amount of times for the player #
 for(int q=-1; q<p; q++){
   digitalWrite(thebuzzer, HIGH);
   delay(100);
  //Turn buzzer off real quick
  digitalWrite(thebuzzer, LOW);
     delay(30);
 }


  // Wait 5 seconds
  delay(WinDelayTime);
  // Set the associated output pin to LOW thus killing power to the winning button's blinking LED circuit.
  reset();
}

void reset(){

  for (int i = 0; i < maxPins; i++) {
   tvalue[i]=false;
   digitalWrite(outputPins[i], LOW);    // Turn the LEDs off
   if (useExtraGroundsAsOutput) {
    digitalWrite(extraGrounds[p], LOW);
  }
  }

}

