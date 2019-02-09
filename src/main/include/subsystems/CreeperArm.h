/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/commands/Subsystem.h>

#include "RobotMap.h"
#ifndef PROTOBOT
#   include <ctre/Phoenix.h>
#   include <frc/AnalogPotentiometer.h>
#   include <frc/Relay.h>
#   include <frc/Solenoid.h>
#endif

class CreeperArm : public frc::Subsystem {
    public:
        CreeperArm();
        void InitDefaultCommand() override;

    // Protobot must not use any of this.
// #   ifndef PROTOBOT
//     // public:
//         // Creeper Arm rotation
//         double GetArmAngle();
//         // Creeper Arm rotational speed
//         void SetRotateSpeed(double spd);
//         // Creeper Arm toggle arm wheels
//         void SetRelay(bool on);

//         // Solenoid toggles (both independent)
//         void SetSolenoidAscend(bool on);
//         void SetSolenoidDescend(bool on);

//     private:
//         // This gets the rotational position of the Creeper Arm
//         // 10k Full-turn potentiometer, could need changes later
//         frc::AnalogPotentiometer m_ArmHipot {kCreeperArmHipot, 360, 0};

//         // The motor that Rotates the Creeper Arm
//         WPI_TalonSRX     m_ArmRotate  {kCreeperArmRotate};
//         frc::Relay       m_ArmDrive   {kCreeperArmDrive, frc::Relay::kForwardOnly};
//         frc::Solenoid    m_SolAscend  {kPCM, kCreeperSolenoidAscend};
//         frc::Solenoid    m_SolDescend {kPCM, kCreeperSolenoidDescend};
// #   endif
};
