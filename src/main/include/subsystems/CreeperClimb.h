#pragma once

#include <frc/commands/Subsystem.h>

#include <ctre/Phoenix.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/DigitalInput.h>
#include <frc/PIDController.h>
#include <frc/Relay.h>
#include <frc/Solenoid.h>
#include <wpi/json.h>

#include "RobotMap.h"

class CreeperClimb : public frc::Subsystem {
    public:
        CreeperClimb(wpi::json&);
        void InitDefaultCommand() override;

        void SetArmAngle(double ang);    // set desired arm angle
        double GetCurrentArmAngle();     // get current arm angle
        void SetRotateSpeed(double spd); // set rotational speed
        void SetArmWheels(bool on);          // toggle wheels on arm

        // Solenoid toggles (independent)
        void SetSolenoidAscend(bool on);
        void SetSolenoidDescend(bool on);
        // get solenoid reed switch state
        bool GetSolenoidSwitch();

    private:
        // This gets the rotational position of the Creeper Arm
        // 10k Full-turn potentiometer, could need changes later
        frc::AnalogPotentiometer m_ArmAngle {kCreeperArmAngle, 1, 0};

        // The motor that Rotates the Creeper Arm
        WPI_TalonSRX      m_ArmRotate  {kCreeperArmRotate};
        frc::Relay        m_ArmDrive   {kCreeperArmDrive, frc::Relay::kForwardOnly};
        frc::Solenoid     m_SolAscend  {kPCM, kCreeperSolenoidAscend};
        frc::Solenoid     m_SolDescend {kPCM, kCreeperSolenoidDescend};
        frc::DigitalInput m_SolSwitch  {kCreeperSolenoidSwitch};

        frc::PIDController m_RotationPID {1, 0, 0, m_ArmAngle, m_ArmRotate};
};
