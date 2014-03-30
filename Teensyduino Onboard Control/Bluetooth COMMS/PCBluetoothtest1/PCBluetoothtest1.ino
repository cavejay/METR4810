/* This is a base connection test for RECEIVING data FROM the pc on the
Teensy 3.1 using the UART TTL Serial1 section onboard */

// Jonathan Holland
// 42658326
// METR4810

char inf; // initiate variable used to store received data
int onboardled = 13; // Use the onboard LED

// Serial 1 is by default receiving on pin 0
HardwareSerial Uart = HardwareSerial();

void setup() {
  pinMode(onboardled, OUTPUT);
  
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
    
    else {
     // Do nothing 
    }
  }
  
  
  
  
}
