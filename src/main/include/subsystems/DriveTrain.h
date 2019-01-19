/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "RobotMap.h"

#include <frc/XboxController.h>

#include <frc/ADXRS450_Gyro.h>
#include <frc/Encoder.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/commands/Subsystem.h>
#include <frc/drive/DifferentialDrive.h>

#ifdef PROTOBOT
#include <frc/Victor.h>
#define MOTORTYPE frc::Victor
#else
#include <ctre/Pheonix.h>
#define MOTORTYPE WPI_TalonSRX 
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
        MOTORTYPE m_motorLeftFront  {kLeftFrontMotor};
        MOTORTYPE m_motorLeftBack   {kLeftBackMotor};
        MOTORTYPE m_motorRightFront {kRightFrontMotor};
        MOTORTYPE m_motorRightBack  {kRightBackMotor};

        // Group the motors into their sides and then combine them
        frc::SpeedControllerGroup m_leftMotors {m_motorLeftFront, m_motorLeftBack};
        frc::SpeedControllerGroup m_rightMotors {m_motorRightFront, m_motorRightBack};
        frc::DifferentialDrive m_robotDrive {m_leftMotors, m_rightMotors};

        // Encoders (third arg reverses direction)
        frc::Encoder m_encoderLeft  {kLeftEncoderPin0, kLeftEncoderPin1, false};
        frc::Encoder m_encoderRight {kRightEncoderPin0, kRightEncoderPin1, true};
        frc::ADXRS450_Gyro m_gyro {};
};
