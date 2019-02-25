#pragma once

#include <frc/commands/Subsystem.h>

#include <ctre/Phoenix.h>
#include <frc/AnalogPotentiometer.h>
#include <frc/DigitalInput.h>
#include <frc/PIDController.h>
#include <frc/Relay.h>
#include <frc/Solenoid.h>
#include <wpi/json.h>
#include <wpi/StringRef.h>

#include "RobotMap.h"

class CreeperClimb : public frc::Subsystem {
    public:
        CreeperClimb(wpi::json&);
        void InitDefaultCommand() override;

        void RotateArmToPosition(wpi::StringRef configName);
        void RotateArmToPosition(double ang); // set desired arm angle
        void SetArmRotateSpeed(double spd);   // set rotational speed
        void StopArmRotation();               // Disable PID.

        double GetCurrentArmPosition(); // get current arm angle
        bool IsArmRotationDone();
        bool IsArmAtPosition(wpi::StringRef configName);

        void SetArmWheels(bool on);          // toggle wheels on arm
        void StopArmWheels();

        // Solenoid toggles (independent)   !!! Use Piston* methods instead.
        void SetSolenoidAscend(bool on);
        void SetSolenoidDescend(bool on);

        // Piston control methods.
        void PistonDisable();   // Disconnect air pressure from piston.
        void PistonExtend();    // Extends piston to cause robot to ascend.
        void PistonRetract();   // Retract piston to cause robot to descend.
        void PistonHold();      // Pressurize both sides of piston to hold its position.

        // get solenoid reed switch state
        bool IsPistonAtLimit();

        void SetOutputRange(double minimumOutput, double maximumOutput);

        frc::PIDController& GetArmPID() { return m_RotationPID; }

    private:
        // This gets the rotational position of the Creeper Arm
        // 10k Full-turn potentiometer, could need changes later
        frc::AnalogPotentiometer m_ArmPosition {kCreeperArmAngle, 195.7, 58.7};

        // The motor that Rotates the Creeper Arm
        WPI_TalonSRX      m_ArmRotate  {kCreeperArmRotate};
        frc::Relay        m_ArmDrive   {kCreeperArmDrive, frc::Relay::kForwardOnly};
        frc::Solenoid     m_SolAscend  {kPCM, kCreeperSolenoidAscend};
        frc::Solenoid     m_SolDescend {kPCM, kCreeperSolenoidDescend};
        frc::DigitalInput m_SolSwitch  {kCreeperSolenoidSwitch};

        frc::PIDController m_RotationPID {0, 0, 0, m_ArmPosition, m_ArmRotate};

        int m_InRangeCount = 0;
};
