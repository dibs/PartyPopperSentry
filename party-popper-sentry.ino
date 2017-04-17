// Basic motion detector and servo sketch with two LED's
// denfordberriman@gmail.com
// https://github.com/dibs/PartyPopperSentry
// MIT license

// LIBRARIES
#include <Servo.h>

// VARS

// the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30; //20

// create servo object to control a servo
Servo myservo;

// initialise servo position var
int pos0 = 10;
int pos1 = 180;
int pirPin    = 3;        // PIR pin
int servoPin  = 9;        // Servo pin
int led1Pin    = 13;      // LED pin
int led2Pin    = 12;      // LED pin
int val = 0;              // variable for reading the pin status
boolean led1On = false;   // use for traking LED
boolean led2On = false;   // use for traking LED
boolean movement = false; // have we detected motion
boolean fired = false;    // has it fired yet?
// PREPARE
void setup() 
{
  Serial.begin(9600); // Enable serial comms
  
  pinMode(pirPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  digitalWrite(pirPin, LOW); // needed?
  
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object 
  //set servo to 10, 0 seems a bit out or reach
  myservo.write(pos0);
  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    for (int j = 0; j < 10; j++) {
      if (led1On) {
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2Pin, HIGH);
        led1On = false;
      } else {
        digitalWrite(led1Pin, HIGH);
        digitalWrite(led2Pin, LOW);
        led1On = true;
      }
      delay(100);
    }
  }
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  led1On = false;
  led2On = false;
}


void loop() 
{
  if (!fired) {
    //motion detector mode
    if(digitalRead(pirPin) == HIGH){
      if (movement == false) {
        digitalWrite(led2Pin, HIGH);   //the led visualizes the sensors output pin state
        //digitalWrite(swordPin, HIGH);  //unsheath the sword
        Serial.println("---");
        Serial.println("motion detected");
        
        myservo.write(pos1);
        delay(500);
        myservo.write(pos0);
        delay(300);
  //      myservo.write(pos1);
  //      delay(300);
  //      myservo.write(pos0);
  //      delay(300);
  //      myservo.write(pos1);
  //      delay(300);
  //      myservo.write(pos0);
  //      delay(300);
  //      myservo.write(pos1);
  //      delay(300);
  //      myservo.write(pos0);
  //      delay(300);
        movement = true;
        fired = true;
      }
    }
    if(digitalRead(pirPin) == LOW && movement == true){
      digitalWrite(led2Pin, LOW);  //the led visualizes the sensors output pin state
      // digitalWrite(swordPin, LOW); //sheath the sword
      Serial.println("clear");
      myservo.write(pos0);
      movement = false;
    }
  } else {
    // Idle when done firing, led's let you know it's done.
    if (led1On) {
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, HIGH);
      led1On = false;
    } else {
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, LOW);
      led1On = true;
    }
    delay(1000);
  }
}
