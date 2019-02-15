#pragma once

#include <frc/commands/Subsystem.h>

#include "RobotMap.h"
#ifndef PROTOBOT
#   include <ctre/Phoenix.h>
#   include <frc/AnalogPotentiometer.h>
#   include <frc/Relay.h>
#   include <frc/Solenoid.h>
#endif

class CreeperClimb : public frc::Subsystem {
    public:
        CreeperClimb();
        void InitDefaultCommand() override;

        double GetArmAngle();            // get arm angle
        void SetRotateSpeed(double spd); // set rotational speed
        void SetRelay(bool on);          // toggle wheels on arm

        // Solenoid toggles (independent)
        void SetSolenoidAscend(bool on);
        void SetSolenoidDescend(bool on);

    private:
        // This gets the rotational position of the Creeper Arm
        // 10k Full-turn potentiometer, could need changes later
        frc::AnalogPotentiometer m_ArmAngle {kCreeperArmAngle, 360, 0};

        // The motor that Rotates the Creeper Arm
        WPI_TalonSRX     m_ArmRotate  {kCreeperArmRotate};
        frc::Relay       m_ArmDrive   {kCreeperArmDrive, frc::Relay::kForwardOnly};
        frc::Solenoid    m_SolAscend  {kPCM, kCreeperSolenoidAscend};
        frc::Solenoid    m_SolDescend {kPCM, kCreeperSolenoidDescend};
};
