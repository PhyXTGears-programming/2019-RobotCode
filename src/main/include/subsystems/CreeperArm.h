/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "RobotMap.h"

#include <frc/commands/Subsystem.h>

#ifndef PROTOBOT
#   include <ctre/Phoenix.h>
#   include <frc/Relay.h>
#   include <frc/Solenoid.h>
#endif

class CreeperArm : public frc::Subsystem {
    public:
        CreeperArm();
        void InitDefaultCommand() override;

    // Protobot must not use any of this.
#   ifndef PROTOBOT
    // public:
        // Creeper Arm rotational speed
        void SetRotateSpeed(double spd);

        // Creeper Arm toggle arm wheels
        void SetRelay(bool on);

        // Solenoid toggles
        void SetSolenoidAscend(bool on);
        void SetSolenoidDescend(bool on);

    private:
        WPI_TalonSRX  m_armRotate  {kCreeperArmRotate};
        frc::Relay    m_armDrive   {kCreeperArmDrive, frc::Relay::kForwardOnly};
        frc::Solenoid m_solAscend  {kPCM, kCreeperSolenoidAscend};
        frc::Solenoid m_solDescend {kPCM, kCreeperSolenoidDescend};
#   endif
};
