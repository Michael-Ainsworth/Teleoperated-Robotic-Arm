# Honors-Instrumentation

This project aims to create a teleoperated robotic arm for remote palpation using the UR5 and soft robotics.

Within this repository, we have the following files and their intended uses:

- BNO055_Calibration: Used to calubrate the two BNO055 chips located on the user's arm tracker.
- Full_Arm_Tracking_UR5End: Used for reading in positional values from the arm tracker after being sent through wireless                                   communication.
- Full_Arm_Tracking_UserEnd: Used for interpreting sensor values and calculating arm position. Arm coordinates are then sent                                to the UR5 end through wireless communication.
- HC_12_Setup_1: Sample code for demonstrating wireless communication using the HC 12. This file should be run through the                      first Serial port. Data is sent back and forth between both Serial monitors.
- HC_12_Setup_2: Sample code for demonstrating wireless communication using the HC 12. This file should be run through the                      first Serial port. Data is sent back and forth between both Serial monitors.
- RF_communication: Sample code to demonstrate RF communication.
- MATLAB_communication.m: Sample code for sending data from the Arduino to a MATLAB script.
- UR5_Control: Full control of the UR5. Data is captured by the arm tracker on the user end, sent to the UR5 end through                      wireless communication, and the sent to a MATLAB script using the MATLAB_communication code. From here, the                    positional arguements are translated to the UR5 using reverse kinematics to move the physical robot.
- inv_kin_test.m: 
- ur5FwdKin.m:
- ur5invKinControl.m:
- ur5_interface.m:
