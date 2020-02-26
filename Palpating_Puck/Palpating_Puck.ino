void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Initialize Pins to their respective mode
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  //This sets the pins to high (5V) in order to read the voltage for each of the taxels 
  //The analogRead function reads this voltage
  digitalWrite(1, HIGH);
  float val = analogRead(A0);
  float val1 = analogRead(A1);
  float val2 = analogRead(A2);
  Serial.print(val);
  Serial.print(",");
  Serial.print(val1);
  Serial.print(",");
  Serial.print(val2);
  Serial.print(",");
  digitalWrite(1, LOW);

  //This sets the pins to high (5V) in order to read the voltage for each of the taxels 
  //The analogRead function reads this voltage
  digitalWrite(2, HIGH);
  float val3 = analogRead(A0);
  float val4 = analogRead(A1);
  float val5 = analogRead(A2);
  Serial.print(val3);
  Serial.print(",");
  Serial.print(val4);
  Serial.print(",");
  Serial.print(val5);
  Serial.print(",");
  digitalWrite(2, LOW);

  //This sets the pins to high (5V) in order to read the voltage for each of the taxels 
  //The analogRead function reads this voltage
  digitalWrite(3, HIGH);
  float val6 = analogRead(A0);
  float val7 = analogRead(A1);
  float val8 = analogRead(A2);
  Serial.print(val6);
  Serial.print(",");
  Serial.print(val7);
  Serial.print(",");
  Serial.print(val8);
  Serial.print(",");
  digitalWrite(3, LOW);

  //Temperature is sensed from the thermistor values and outputted to the Serial port 
  float tval = analogRead(A3);
  Serial.print(tval);
  Serial.print(",");
  float tval1 = analogRead(A4);
  Serial.println(tval1);
  delay(50);
  
}
