#include <Servo.h>

Servo myservo;
int pos=30;

void setup() {
  myservo.attach(9);
  pinMode(8,INPUT);
}


void loop() {
 if(digitalRead(8)==HIGH) {
   for(pos=30; pos<=160; pos+=5) { 
     myservo.write(pos);
     delay(75);
   }
   myservo.write(30);
 }
}
