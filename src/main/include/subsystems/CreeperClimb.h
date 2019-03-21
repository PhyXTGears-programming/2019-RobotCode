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
        void SetSolenoidExtend(bool leftOn, bool rightOn);
        void SetSolenoidRetract(bool leftOn, bool rightOn);

        // Piston control methods.
        void PistonDisable();   // Disconnect air pressure from piston.
        void PistonExtend();    // Extends piston to cause robot to ascend.
        void PistonRetract();   // Retract piston to cause robot to descend.
        void PistonHold();      // Pressurize both sides of piston to hold its position.

        // get solenoid reed switch state
        void ResetPistonLimitLatch();
        bool IsPistonAtLimit();

        void SetRotatePIDOutputRange(double minimumOutput, double maximumOutput);

        frc::PIDController& GetArmPID() { return m_RotationPID; }
        WPI_TalonSRX& GetArmMotor() { return m_ArmRotate; }

        void RunReset();
        void Disable();

    private:
        // This gets the rotational position of the Creeper Arm
        // 10k Full-turn potentiometer, could need changes later
        // frc::AnalogPotentiometer m_ArmPosition {kCreeperArmAngle, 199.12, 31.66}; // Testbot
        frc::AnalogPotentiometer m_ArmPosition {kCreeperArmAngle, 184.43, 35.23}; // Compbot

        // The motor that Rotates the Creeper Arm
        WPI_TalonSRX      m_ArmRotate  {kCreeperArmRotate};
        frc::Relay        m_ArmDrive   {kCreeperArmDrive, frc::Relay::kForwardOnly};

        frc::Solenoid     m_SolExtendLeft  {kPCM, kCreeperSolenoidExtendLeft};
        frc::Solenoid     m_SolRetractLeft {kPCM, kCreeperSolenoidRetractLeft};

        frc::Solenoid     m_SolExtendRight  {kPCM, kCreeperSolenoidExtendRight};
        frc::Solenoid     m_SolRetractRight {kPCM, kCreeperSolenoidRetractRight};

        frc::PIDController m_RotationPID {0, 0, 0, m_ArmPosition, m_ArmRotate};

        int m_InRangeCount = 0;


        /**
        * Machine angles are those reported by machine sensors and subject to drift
        * and defects in assembly.
        *
        * World angles are ideal values pertaining to actual orientation in the world
        * with respect to a zero reference.
        *
        * config.json stores world angles with a zero-point used to convert between
        * world and machine angles.
        */
        double machineAngleToWorld(double);
        double worldAngleToMachine(double);
};
