// Include the necessary libraries
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

// Set up variables to each BNO055. Include unique address for each chip.
Adafruit_BNO055 bnoA = Adafruit_BNO055(-1, 0x29);
Adafruit_BNO055 bnoB = Adafruit_BNO055(-1, 0x28);

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

// Create empty arrays to index quaternion values into.
float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};
float q_conj_[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_mult[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_conj[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float q_tempMult[4] = {0.0f, 0.0f, 0.0f, 0.0f};

// Create empty arrays for each BNO055.
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
  Serial.begin(9600);             // Begin Serial communication
  HC12.begin(9600);               // Serial port to HC12
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  // Initialize the sensor A
  if(!bnoA.begin())
  {
    // Error handling if nothing is returned
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);  // Wait 1 second
    
  bnoA.setExtCrystalUse(true);  // Used for initializing bnoA variable

  // Initialize the sensor B
  if(!bnoB.begin())
  {
    // Error handling if nothing is returned
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);  // Wait 1 second
    
  bnoB.setExtCrystalUse(true);  // Used for initializing bnoA variable
}
 
void loop(void) 
{
  // Create a new sensor event for sensor A
  sensors_event_t eventA; 
  bnoA.getEvent(&eventA);
  
  imu::Quaternion quatA = bnoA.getQuat();  // Get the quaternion

  // Get each element of the quaternion
  float w = (quatA.w());
  float x = (quatA.x());
  float y = (quatA.y());
  float z = (quatA.z());

  // Create a new sensor event for sensor B
  sensors_event_t eventB; 
  bnoB.getEvent(&eventB);

  
  imu::Quaternion quatB = bnoB.getQuat();  // Get the quaternion

  // Get each element of the quaternion
  float wB = (quatB.w());
  float xB = (quatB.x());
  float yB = (quatB.y());
  float zB = (quatB.z());

  if(millis() - start > 200) {    // While time is less that 200 milliseconds
    int count = 0;    // Initialize count
    float sum[3] = {0.0, 0.0, 0.0};   // Set empty array to fill with final position
    now = millis();   // Set now variable to current time 
    
    while(millis() - now < 500) {   // While time is less that 500 milliseconds
      q[0] = w; q[1] = x; q[2] = y; q[3] = z;    // Set initial quaternion points for sensor A
      quatConj(q);   // Call quatConj function on the current quaternion
      q_conj_[0] = q_conj[0]; q_conj_[1] = q_conj[1]; q_conj_[2] = q_conj[2]; q_conj_[3] = q_conj[3];  // Update quaternion
      
      float lu = 0.25;    // Set length of upper arm
      float armCoords[4] = {0.0f, lu, 0.0f, 0.0f};    // Use arm length to initialize quaternion once in T-pose
      
      quatMult(q, armCoords);   // Call quatMult function
      // Update the quaternion after completing the matrix multiplication
      q_tempMult[0] = q_mult[0]; q_tempMult[1] = q_mult[1]; q_tempMult[2] = q_mult[2]; q_tempMult[3] = q_mult[3];
      // Multiply the current quaternion by the conjugate
      quatMult(q_tempMult, q_conj_);  // Creates final elbow positions
      float elbowPos[4] = {q_mult[0], q_mult[1], q_mult[2], q_mult[3]};    // Set elbow position to single variable   
      
      qB[0] = wB; qB[1] = xB; qB[2] = yB; qB[3] = zB;  // Set initial quaternion points for sensor B
      quatConj(qB);   // Call quatConj function on the current quaternion
      q_conj_B[0] = q_conj[0]; q_conj_B[1] = q_conj[1]; q_conj_B[2] = q_conj[2]; q_conj_B[3] = q_conj[3];  // Update quaternion
      
      float lu1 = 0.25;    // Set length of forearm
      float armCoordsB[4] = {0.0f, lu1, 0.0f, 0.0f};    // Use arm length to initialize quaternion once in T-pose
      
      quatMult(qB, armCoordsB);  // Call quatMult function
      // Update the quaternion after completing the matrix multiplication
      q_tempMultB[0] = q_mult[0]; q_tempMultB[1] = q_mult[1]; q_tempMultB[2] = q_mult[2]; q_tempMultB[3] = q_mult[3];
      // Multiply the current quaternion by the conjugate
      quatMult(q_tempMultB, q_conj_B);
      // Calculates wrist position with respect to elbow
      float elbowPosB[4] = {q_mult[0], q_mult[1], q_mult[2], q_mult[3]};    // Set wrist position to single variable

      // Use vector addition to find the location of the wrist with respect to the shoulder.
      // Use specific scaling to improve position on UR-5 robot.
      // Eg. increase z axis to prevent robot from hitting table.
      sum[0] += -elbowPosB[1] - elbowPos[1];
      sum[1] += -elbowPosB[2] - elbowPos[2];
      sum[2] += (elbowPosB[3] + elbowPos[3]) + 0.50;
      count += 1;
    }

    float average[3] = {sum[0]/float(count), sum[1]/float(count), sum[2]/float(count)};  // Divide position by count

    // Print all position locations to the HC12 to be sent to opposite Serial monitor.
    HC12.print(average[0]); HC12.print(",");
    HC12.print(average[1]); HC12.print(",");
    HC12.print(average[2]); HC12.print("!");
    
    while (HC12.available()) {        // If HC-12 has data
      Serial.write(HC12.read());      // Send the data to Serial monitor
    }
    
    while (Serial.available()) {      // If Serial monitor has data
      HC12.write(Serial.read());      // Send that data to HC-12
    }

    radio.begin();
    radio.openWritingPipe(address); // start writing
    radio.setPALevel(RF24_PA_MIN);
  
    const char outtext[] = "o"; // writing
    radio.write(&outtext, sizeof(outtext));
    Serial.print("Outgoing text: ");
    Serial.println(outtext);

  }
}

// Function to perform quaternion multiplication when working with calculating final arm position.
void quatMult(float* q1, float* q2) {
  q_mult[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
  q_mult[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
  q_mult[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
  q_mult[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];
}

// Function to perform quaternion conjugation when working with calculating final arm position.
void quatConj(float* q1) {
  q_conj[0] = q1[0];
  q_conj[1] = -q1[1];
  q_conj[2] = -q1[2];
  q_conj[3] = -q1[3];
}
