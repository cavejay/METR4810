/* This is a PWM test for RECEIVING data FROM the pc on the
Teensy 3.1 and then sending different PWM signals */

// Jonathan Holland
// 42658326
// METR4810

char inf; // initiate variable used to store received data
int onboardled = 13; // Use the onboard LED
int pwmPin = 9; // Set the Pin for PWM (back motor)

// Serial 1 is by default receiving on pin 0
HardwareSerial Uart = HardwareSerial();

void setup() {
  pinMode(onboardled, OUTPUT);
  
  pinMode(pwmPin, OUTPUT);
  // Set the frequency of the PWM output
  analogWriteFrequency(pwmPin, 2929);
  // This gives us 0-16383 instead of 0-255
  
  Uart.begin(115200); // rate of 115200bps
                        // Picking a fast speed here is important
                        // Error at 48Mhz @115200 is +0.04%  
  
  
}

void loop() {
  if(Uart.available()) { 
    inf = Uart.read(); // Read ANY incoming serial (not tied to an input pin in this example)
    
    if(inf=='0') { // *Specify the number as a character
      digitalWrite(onboardled, LOW);
      delay(15);     
    }
    
    else if(inf=='1') {
      digitalWrite(onboardled, HIGH);
      delay(15);  
    }
    
    else if(inf=='2') {
      analogWrite(pwmPin, 500);
      delay(15);
    }
    else if(inf=='3') {
      analogWrite(pwmPin, 8000);
      delay(15);
    }
    else if(inf=='4') {
      analogWrite(pwmPin, 15000);
      delay(15);
    }
    
    else {
     // Do nothing 
    }
  }
  
  
  
  
}
