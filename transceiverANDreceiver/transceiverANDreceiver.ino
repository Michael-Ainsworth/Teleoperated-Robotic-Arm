#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  radio.begin();
  radio.openWritingPipe(address); //start writing
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  const char outtext[] = "o"; //writing
  radio.write(&outtext, sizeof(outtext));
  Serial.print("Outgoing text: ");
  Serial.println(outtext);

  delay(5000);

  const char outtext2[] = "c"; //writing
  radio.write(&outtext2, sizeof(outtext2));
  Serial.print("Outgoing text: ");
  Serial.println(outtext2);

  delay(5000);

  radio.openReadingPipe(0, address); //start reading
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  if (radio.available()) {// reading
    char intext[32] = "";
    radio.read(&intext, sizeof(intext));
    Serial.print("Received text: ");
    Serial.println(intext);
  }

  delay(100);
  
  

}
