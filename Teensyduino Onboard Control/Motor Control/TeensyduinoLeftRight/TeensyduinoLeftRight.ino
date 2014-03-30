/* Written by Jonathan Holland - 42658326,
   with reference to Sweep by Barragan (the arduino servo library example) */


#include <Servo.h> 
 
Servo myservo;  // create servo object
int pos = 0;    // servo position
int left;

 
void setup() 
{ 
  myservo.attach(9);  /* Initiate servo object on pin9 (one of the pins 
                      tested to work on teensy 3.1 with the servo.h library */
  pinMode(8, INPUT); // Set up pin 8 as the button press input
  left = 0;
} 

void turnLeft() {
  for(pos = 0; pos < 90; pos += 1) /* in steps of 1 degree (The larger this is, the smoother it will be 
                                    as the steps are dependant on clock calcs - defaulting to 16MHz). 
                                    Further C code could change this to the clock speed of the teensy @72MHz 
                                    
                                    A benefit of this however is being able to stop at more precise angles,
                                    which is very important in turning                                      */
                                    
  {                                   
    myservo.write(pos);              // rotate the servo
    delay(15);                       /* This delay before reversing the direction is problematic in the actual
                                      turning as we want a more accurate implementation of seeing when the servo stops.
                                      
                                      This might be difficult with the closed loop hobby dagu 8g servo,
                                      but a solution is to open the motor and connect the pot as an input to the teensy*/
  } 
  
}

void turnRight() {
  for(pos = 90; pos>=1; pos-=1)     // reverse direction 
  {                                
    myservo.write(pos);              // rotate the servo
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
 
void loop() 
{ 
  // IF the button is pressed, turn left  
  if((digitalRead(8)==HIGH)&&(left==0)) {
    turnLeft();
    left = 1;
  }
  // IF the button is pressed AND it has already turned left, turn right
  if((digitalRead(8)==HIGH)&&(left==1)) {
    turnRight();
    left = 0;
  }
} 


