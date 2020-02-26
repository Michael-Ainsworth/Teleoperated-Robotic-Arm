//These are the pins for the DC Motor Control through the H-Bridge
int Enable1 = 13;
int Input1 = 5;
int Input2 = 4;

char oldintext = "o";
char haptics[] = "ooo";

float thumb; 
float indexFinger; 
float middleFinger;

//Imports and initialization for the RF transceiver 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Initialize Pins to their respective mode
  pinMode(Enable1, OUTPUT); 
  pinMode(Input1, OUTPUT);
  pinMode(Input2, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  /*
  analogWrite(Enable1, 200);
  digitalWrite(Input1, LOW);
  digitalWrite(Input2, HIGH);
  delay(400);

  analogWrite(Enable1, 0);
  digitalWrite(Input1, LOW);
  digitalWrite(Input2, LOW);
  delay(2000);
  
  analogWrite(Enable1, 200);
  digitalWrite(Input1, HIGH);
  digitalWrite(Input2, LOW);
  delay(800);

  analogWrite(Enable1, 0);
  digitalWrite(Input1, LOW);
  digitalWrite(Input2, LOW);
  delay(2000);
  */

  //Initializes the RF Module
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Begins RF Listening for a message
  radio.openReadingPipe(0, address);
  radio.startListening();

  delay(10);

  //This loop opens and closes the hand based on the character received 
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    //Serial.print(text);
    //Serial.println(oldintext);
    if (text[0] == 'o' && oldintext == 'c') {
      //Once the character 'o' is received, the motor will spin for 0.5 seconds to open
      //the hand and then the motor is turned off
      analogWrite(Enable1, 200);
      digitalWrite(Input1, HIGH);
      digitalWrite(Input2, LOW);
      delay(500);
      analogWrite(Enable1, 0);
      digitalWrite(Input1, LOW);
      digitalWrite(Input2, LOW);
      oldintext = 'o';
    } else if (text[0] == 'c' && oldintext == 'o') {
      //Once the character 'c' is received, the motor will spin for 0.25 seconds to open
      //the hand and then the motor is turned off
      analogWrite(Enable1, 200);
      digitalWrite(Input1, LOW);
      digitalWrite(Input2, HIGH);
      delay(250);
      analogWrite(Enable1, 0);
      digitalWrite(Input1, LOW);
      digitalWrite(Input2, LOW);
      oldintext = 'c';
    } else {
      oldintext = text[0];
    }
  }

  radio.stopListening();

  //This sets the pins to high (5V) in order to read the voltage for each of the taxels 
  //The analogRead function reads this voltage
  digitalWrite(11, HIGH);
  float val = analogRead(A0);
  float val1 = analogRead(A1);
  float val2 = analogRead(A2);
  float val3 = analogRead(A3);
  float val4 = analogRead(A4);
  float val5 = analogRead(A5);
  Serial.print(val);
  Serial.print(",");
  Serial.print(val1);
  Serial.print(",");
  Serial.print(val2);
  Serial.print(",");
  Serial.print(val3);
  Serial.print(",");
  Serial.print(val4);
  Serial.print(",");
  Serial.print(val5);
  Serial.print(",");
  digitalWrite(11, LOW);

  //This sets the pins to high (5V) in order to read the voltage for each of the taxels 
  //The analogRead function reads this voltage
  digitalWrite(12, HIGH);
  float val6 = analogRead(A0);
  float val7 = analogRead(A1);
  float val8 = analogRead(A2);
  float val9 = analogRead(A3);
  float val10 = analogRead(A4);
  float val11 = analogRead(A5);
  Serial.print(val6);
  Serial.print(",");
  Serial.print(val7);
  Serial.print(",");
  Serial.print(val8);
  Serial.print(",");
  Serial.print(val9);
  Serial.print(",");
  Serial.print(val10);
  Serial.print(",");
  Serial.print(val11);
  Serial.print(",");
  digitalWrite(12, LOW);

  //This sets the pins to high (5V) in order to read the voltage for each of the taxels 
  //The analogRead function reads this voltage
  digitalWrite(10, HIGH);
  float val12 = analogRead(A0);
  float val13 = analogRead(A1);
  float val14 = analogRead(A2);
  float val15 = analogRead(A3);
  float val16 = analogRead(A4);
  float val17 = analogRead(A5);
  Serial.print(val12);
  Serial.print(",");
  Serial.print(val13);
  Serial.print(",");
  Serial.print(val14);
  Serial.print(",");
  Serial.print(val15);
  Serial.print(",");
  Serial.print(val16);
  Serial.print(",");
  Serial.println(val17);
  digitalWrite(10, LOW);

  thumb = (val4 + val5 + val10 + val11 + val16 + val17)/6;
  indexFinger = (val2 + val3 + val8 + val9 + val14 + val15)/6;
  middleFinger = (val + val1 + val6 + val7 + val12 + val13)/6;

  if (thumb > 344 ) {
    haptics[0] = 'o';
  } else if (thumb < 344 && thumb >= 342) {
    haptics[0] = 's';
  } else if (thumb < 342 && thumb >= 341) {
    haptics[0] = 'm';
  } else {
    haptics[0] = 'h';
  }

  if (indexFinger < 20 ) {
    haptics[1] = 'o';
  } else if (indexFinger < 35 && indexFinger >= 20) {
    haptics[1] = 's';
  } else if (indexFinger < 50 && indexFinger >= 35) {
    haptics[1] = 'm';
  } else {
    haptics[1] = 'h';
  }

  if (middleFinger < 185) {
    haptics[2] = 'o';
  } else if (middleFinger < 200 && middleFinger >= 185) {
    haptics[2] = 's';
  } else if (middleFinger < 250 && middleFinger >= 200) {
    haptics[2] = 'm';
  } else {
    haptics[2] = 'h';
  }
  
  radio.openWritingPipe(address); //start writing
  //char outtext[] = ""; //writing
  radio.write(&haptics, sizeof(haptics));

  //Serial.println(haptics);
  //Serial.println(thumb);
  //Serial.println(indexFinger);
  //Serial.println(middleFinger);
  
}
