

const int PWMpin = 9;


void setup() {
  pinMode(PWMpin, OUTPUT);
  
}



void loop() {
  analogWrite(PWMpin, 30);
  delay(15);
  analogWrite(PWMpin, 80);
  delay(15);
  analogWrite(PWMpin, 150);
  delay(40);  
  
}
