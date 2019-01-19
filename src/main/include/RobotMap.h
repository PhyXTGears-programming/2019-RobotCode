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
#   define kLeftFrontMotor     0
#   define kLeftBackMotor      1
#   define kRightFrontMotor    2
#   define kRightBackMotor     3

#   define kLeftEncoderPin0    0
#   define kLeftEncoderPin1    1
#   define kRightEncoderPin0   2
#   define kRightEncoderPin1   3
#else
#   define kFrontLeftMotor     0
#   define kBackLeftMotor      1
#   define kFrontRightMotor    2
#   define kBackRightMotor     3

#   define kLeftEncoderPin0    0
#   define kLeftEncoderPin1    1
#   define kRightEncoderPin0   2
#   define kRightEncoderPin1   3
#endif

// If you are using multiple modules, make sure to define both the port
// number and the module. For example you with a rangefinder:
// constexpr int kRangeFinderPort = 1;
// constexpr int kRangeFinderModule = 1;
