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

/**
 * !! ---------------------------------------------------------------------- !!
 *  Programmers, be sure to always check the wiring spreadsheet in Google Docs 
 * !! ---------------------------------------------------------------------- !!
**/

// Only define this macro if you're testing on the 1721 bot.
//#define PROTOBOT


// ROBORIO PINS
#define DIO_0 0
#define DIO_1 1
#define DIO_2 2
#define DIO_3 3
#define DIO_4 4
#define DIO_5 5
#define DIO_6 6
#define DIO_7 7
#define DIO_8 8
#define DIO_9 9

#define AIO_0 0
#define AIO_1 1
#define AIO_2 2
#define AIO_3 3

#define RELAY_0 0
#define RELAY_1 1
#define RELAY_2 2
#define RELAY_3 3

#define PWM_0 0
#define PWM_1 1
#define PWM_2 2
#define PWM_3 3
#define PWM_4 4
#define PWM_5 5
#define PWM_6 6
#define PWM_7 7
#define PWM_8 8
#define PWM_9 9

#define CAN_0 0
#define CAN_1 1
#define CAN_2 2
#define CAN_3 3
#define CAN_4 4
#define CAN_5 5
#define CAN_6 6
#define CAN_7 7
#define CAN_8 8
#define CAN_9 9

#define AIR_0 0
#define AIR_1 1
#define AIR_2 2
#define AIR_3 3
#define AIR_4 4
#define AIR_5 5

// Motor controller pins
#ifdef PROTOBOT
// Pin numbers for PWM motor controllers.
#   define kRightFrontMotor    PWM_0
#   define kRightBackMotor     PWM_1
#   define kLeftFrontMotor     PWM_2
#   define kLeftBackMotor      PWM_3

// Pin numbers for wheel encoders
// Protobot doesn't have encoders
#   define kLeftEncoderPin0    DIO_0
#   define kLeftEncoderPin1    DIO_1
#   define kRightEncoderPin0   DIO_2
#   define kRightEncoderPin1   DIO_3

// Cargo intake proximity sensor connected to DIO pin 4.
#   define kCargoSensor DIO_4

#else

// CAN IDs for CAN Bus TalonSRX's.
#   define kLeftFrontMotor     CAN_1
#   define kLeftBackMotor      CAN_7
#   define kRightFrontMotor    CAN_2
#   define kRightBackMotor     CAN_4

// Pin numbers for wheel encoders
#   define kLeftEncoderPin0    DIO_0
#   define kLeftEncoderPin1    DIO_1
#   define kRightEncoderPin0   DIO_2
#   define kRightEncoderPin1   DIO_3

// PCM CAN ID
#   define kPCM CAN_9

// Creeper Arm Climber constants
// TalonSRX CAN ID, Relay pin, and Solenoid PCM IDs
#   define kCreeperArmDrive  RELAY_0
#   define kCreeperArmHipot  AIO_1
#   define kCreeperArmRotate CAN_6
#   define kCreeperSolenoidAscend  AIR_0
#   define kCreeperSolenoidDescend AIR_1

// Cargo Intake constants
// Cargo intake proximity sensor connected to DIO pin 4.
#   define kCargoSensor DIO_4
// Bottom and top servos for the hatch grabbers
#   define kCargoHatchServoBottom PWM_0
#   define kCargoHatchServoTop    PWM_1
// Spike rlays for pickup rollers and ejector
#   define kCargoSpikeEjector RELAY_2
#   define kCargoSpikeRoller  RELAY_1
// TalonSRX CAN ID for rotating
#   define kCargoTalonRotate CAN_6
// TalonSRX CAN ID for rollers
#   define kCargoTalonRollers CAN_3
// Hipot for rotation position
#   define kCargoRotationSensor AIO_0

#endif
