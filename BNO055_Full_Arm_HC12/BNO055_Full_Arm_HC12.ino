#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

#include <SoftwareSerial.h>
SoftwareSerial HC12(9, 10); // HC-12 TX Pin, HC-12 RX Pin

Adafruit_BNO055 bnoA = Adafruit_BNO055(-1, 0x29);
Adafruit_BNO055 bnoB = Adafruit_BNO055(-1, 0x28);

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float q_conj_[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_mult[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_conj[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_tempMult[4] = {0.0f, 0.0f, 0.0f, 0.0f};

float qB[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float q_conj_B[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_multB[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_conjB[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_tempMultB[4] = {0.0f, 0.0f, 0.0f, 0.0f};

uint32_t now = 0;
uint32_t start = 0;
uint32_t test = 0;

void setup(void) 
{
  Serial.begin(9600);
  HC12.begin(9600);               // Serial port to HC12
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bnoA.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bnoA.setExtCrystalUse(true);

  /* Initialise the sensor */
  if(!bnoB.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bnoB.setExtCrystalUse(true);
}
 
void loop(void) 
{
  /* Get a new sensor event */ 
  sensors_event_t eventA; 
  bnoA.getEvent(&eventA);
  
  imu::Quaternion quatA = bnoA.getQuat();

  float w = (quatA.w());
  float x = (quatA.x());
  float y = (quatA.y());
  float z = (quatA.z());

  sensors_event_t eventB; 
  bnoB.getEvent(&eventB);

  
  imu::Quaternion quatB = bnoB.getQuat();
 
  float wB = (quatB.w());
  float xB = (quatB.x());
  float yB = (quatB.y());
  float zB = (quatB.z());

  if(millis() - start > 200) {
    int count = 0;
    float sum[3] = {0.0, 0.0, 0.0};
    now = millis();
    
    while(millis() - now < 500) {
      q[0] = w; q[1] = x; q[2] = y; q[3] = z;
      quatConj(q);
      q_conj_[0] = q_conj[0]; q_conj_[1] = q_conj[1]; q_conj_[2] = q_conj[2]; q_conj_[3] = q_conj[3];
      
      float lu = 0.25;
      float armCoords[4] = {0.0f, lu, 0.0f, 0.0f};
      
      quatMult(q, armCoords);
      q_tempMult[0] = q_mult[0]; q_tempMult[1] = q_mult[1]; q_tempMult[2] = q_mult[2]; q_tempMult[3] = q_mult[3];
      quatMult(q_tempMult, q_conj_);
      float elbowPos[4] = {q_mult[0], q_mult[1], q_mult[2], q_mult[3]};    
      
      qB[0] = wB; qB[1] = xB; qB[2] = yB; qB[3] = zB;
      quatConj(qB);
      q_conj_B[0] = q_conj[0]; q_conj_B[1] = q_conj[1]; q_conj_B[2] = q_conj[2]; q_conj_B[3] = q_conj[3];
      
      float lu1 = 0.25;
      float armCoordsB[4] = {0.0f, lu1, 0.0f, 0.0f};
      
      quatMult(qB, armCoordsB);
      q_tempMultB[0] = q_mult[0]; q_tempMultB[1] = q_mult[1]; q_tempMultB[2] = q_mult[2]; q_tempMultB[3] = q_mult[3];
      quatMult(q_tempMultB, q_conj_B);
      float elbowPosB[4] = {q_mult[0], q_mult[1], q_mult[2], q_mult[3]};
      
      sum[0] += -elbowPosB[1] - elbowPos[1];
      sum[1] += -elbowPosB[2] - elbowPos[2];
      sum[2] += (elbowPosB[3] + elbowPos[3]) + 0.50;
      count += 1;
    }

    float average[3] = {sum[0]/float(count), sum[1]/float(count), sum[2]/float(count)};

    HC12.print(average[0]); HC12.print(",");
    HC12.print(average[1]); HC12.print(",");
    HC12.print(average[2]); HC12.print("!");
    
    while (HC12.available()) {        // If HC-12 has data
      Serial.write(HC12.read());      // Send the data to Serial monitor
    }
    
    while (Serial.available()) {      // If Serial monitor has data
      HC12.write(Serial.read());   // Send that data to HC-12
    }

    radio.begin();
    radio.openWritingPipe(address); //start writing
    radio.setPALevel(RF24_PA_MIN);
  
    const char outtext[] = "o"; //writing
    radio.write(&outtext, sizeof(outtext));
    Serial.print("Outgoing text: ");
    Serial.println(outtext);

  }
}


void quatMult(float* q1, float* q2) {
  q_mult[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
  q_mult[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
  q_mult[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
  q_mult[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];
}


void quatConj(float* q1) {
  q_conj[0] = q1[0];
  q_conj[1] = -q1[1];
  q_conj[2] = -q1[2];
  q_conj[3] = -q1[3];
}
