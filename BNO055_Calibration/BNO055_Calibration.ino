/* For use of this program, observe the Serial Monitor output. There will be three
 * calibration readings for each BNO055 sensor: Magnetometer, Gyroscope, and Accelerometer.
 * Initially, all the readings will read 0. When all readings reach 3, the sensors are fully
 * calibrated.
 * 
 * To calibrate the gyroscope, simple lay the sensors on a flat surface and wait for a few
 * seconds. To calibrate the magnetometer, move each sensor in a figure-8 motion until 
 * a 3 is displayed. To calibrate the accelerometer, rotate each BNO055 approximately 45 degrees
 * along any coordinate axis, pausing after each rotation. Continue until reading displays a 3.
 * 
 * Total calibration process takes 1-2 minutes.
 */



// Include necessary libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line bettylow (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x29);
Adafruit_BNO055 bnoB = Adafruit_BNO055(-1, 0x28);


/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;  // set variables for each sensor for the first BNO055
  system = gyro = accel = mag = 0;  // initialize system to 0
  bno.getCalibration(&system, &gyro, &accel, &mag);  // Start calibration process

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);


  uint8_t gyroB, accelB, magB;  // Initialize variables for the second BNO055
  system = gyroB = accelB = magB = 0;  // initialize system to 0
  bnoB.getCalibration(&system, &gyroB, &accelB, &magB);  // Start calibration process

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyroB, DEC);
  Serial.print(" A:");
  Serial.print(accelB, DEC);
  Serial.print(" M:");
  Serial.print(magB, DEC);
}



void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  bno.setExtCrystalUse(true);



  if(!bnoB.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Optional: Display current status */
//  displaySensorStatus();

  bnoB.setExtCrystalUse(true);
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  /* Display the floating point data */
//  Serial.print("X: ");
//  Serial.print(event.orientation.x, 4);
//  Serial.print("\tY: ");
//  Serial.print(event.orientation.y, 4);
//  Serial.print("\tZ: ");
//  Serial.print(event.orientation.z, 4);

  /* Optional: Display calibration status */
  displayCalStatus();

  sensors_event_t eventB;
  bnoB.getEvent(&eventB);

  /* Optional: Display sensor status (debug only) */
  //displaySensorStatus();

  /* New line for the next sample */
  Serial.println("");

  /* Wait the specified delay before requesting nex data */
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
