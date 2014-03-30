/* This is a base connection test for SENDING data TO the pc on the
Teensy 3.1 using the UART TTL Serial1 section onboard */

// Jonathan Holland
// 42658326
// METR4810

int bump = 4; // Use the bump sensor to trigger sending information

// Serial 1 is by default sending on pin 1
HardwareSerial Uart = HardwareSerial();

void setup() {
  Uart.begin(115200);
  
}

void loop() {
  if(bump==HIGH) {
    Uart.write(1); // write transfers a byte at a time
    delay(15);
  }
  else {
    Uart.write(0);
    delay(15);
  }

