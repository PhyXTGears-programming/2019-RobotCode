/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "RobotMap.h"


#include <frc/ADXRS450_Gyro.h>
#include <frc/Encoder.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/XboxController.h>
#include <frc/commands/Subsystem.h>
#include <frc/drive/DifferentialDrive.h>

#ifdef PROTOBOT
    // Our team's prototyping bot just uses regular Victor motor controllers.
#   include <frc/Victor.h>
#   define  MOTORTYPE frc::Victor
#else
    // The WPI_TalonSRX type provides compatibility with WPILib's drivetrain types.
#   include <ctre/Phoenix.h>
#   define  MOTORTYPE WPI_TalonSRX
#endif

#include "RobotMap.h"

class DriveTrain : public frc::Subsystem {
    public:
        DriveTrain();
        void InitDefaultCommand() override;
        void Drive(double left, double right);
        void Drive(frc::XboxController& driver);

    private:
        // Motors
        MOTORTYPE m_MotorRightFront {kRightFrontMotor};
        MOTORTYPE m_MotorRightBack  {kRightBackMotor};
        MOTORTYPE m_MotorLeftFront  {kLeftFrontMotor};
        MOTORTYPE m_MotorLeftBack   {kLeftBackMotor};

        // Group the motors into their sides and then combine them into the drivetrain
        frc::SpeedControllerGroup m_RightMotors {m_MotorRightFront, m_MotorRightBack};
        frc::SpeedControllerGroup m_LeftMotors {m_MotorLeftFront, m_MotorLeftBack};
        frc::DifferentialDrive m_RobotDrive {m_LeftMotors, m_RightMotors};

        // Encoders (reverse the right encoder)
        frc::Encoder m_EncoderRight {kRightEncoderPin0, kRightEncoderPin1, true};
        frc::Encoder m_EncoderLeft  {kLeftEncoderPin0, kLeftEncoderPin1, false};
        //frc::ADXRS450_Gyro m_Gyro {}; // onboard RoboRIO gyro
};
