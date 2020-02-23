// Run code on Linux side to receive arm tracking
// Device 2
#include <SoftwareSerial.h>
SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

String readBuffer = "";

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
}
void loop() {
  HC12.listen();
  char help = 'a';
  while (HC12.available()) {        // If HC-12 has data
    help = HC12.read();

    if(help == '!') {
      break;
    }
    
    readBuffer+=help;
  }

  if(help == '!'){
    Serial.println(readBuffer);
    readBuffer = "";
  }
  
  while (Serial.available()) {      // If Serial monitor has data
    HC12.write(Serial.read());      // Send that data to HC-12
  }
  
}
