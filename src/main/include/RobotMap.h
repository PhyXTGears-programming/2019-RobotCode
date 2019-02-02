/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
**/


// Only define this macro if you're testing on the 1721 bot.
#define PROTOBOT


// Motor controller pins
#ifdef PROTOBOT
// pin numbers for PWM motor controllers
#   define kRightFrontMotor    0
#   define kRightBackMotor     1
#   define kLeftFrontMotor     2
#   define kLeftBackMotor      3

// pin numbers for wheel encoders
// protobot didn't have encoders
#   define kLeftEncoderPin0    0
#   define kLeftEncoderPin1    1
#   define kRightEncoderPin0   2
#   define kRightEncoderPin1   3
#else
// CAN ID numbers for CAN Bus TalonSRXs
#   define kLeftFrontMotor     1
#   define kLeftBackMotor      7
#   define kRightFrontMotor    2
#   define kRightBackMotor     4

// pin numbers for wheel encoders
#   define kLeftEncoderPin0    0
#   define kLeftEncoderPin1    1
#   define kRightEncoderPin0   2
#   define kRightEncoderPin1   3
#endif
