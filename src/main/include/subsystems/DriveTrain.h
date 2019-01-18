/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/ADXRS450_Gyro.h>
#include <frc/Encoder.h>
#include <frc/PWMTalonSRX.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/commands/Subsystem.h>
#include <frc/drive/DifferentialDrive.h>

#include "RobotMap.h"

class DriveTrain : public frc::Subsystem {
    public:
        DriveTrain();
        void InitDefaultCommand() override;

    private:
        // Motors
        frc::PWMTalonSRX m_motorLeft1 {kLeftMotor1};
        frc::PWMTalonSRX m_motorLeft2 {kLeftMotor2};
        frc::SpeedControllerGroup m_motorLeftSet {m_motorLeft1, m_motorLeft2};

        frc::PWMTalonSRX m_motorRight1 {kRightMotor1};
        frc::PWMTalonSRX m_motorRight2 {kRightMotor2};
        frc::SpeedControllerGroup m_motorRightSet {m_motorRight1, m_motorRight2};

        frc::DifferentialDrive m_robotDrive {m_motorLeftSet, m_motorRightSet};

        // Encoders (third arg reverses direction)
        frc::Encoder m_encoderLeft  {kLeftMotor1, kLeftMotor2, false, frc::CounterBase::k4X};
        frc::Encoder m_encoderRight {kRightMotor1, kRightMotor2, true, frc::CounterBase::k4X};
        frc::ADXRS450_Gyro m_gyro {};
};
