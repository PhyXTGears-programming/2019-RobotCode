#pragma once

#include <frc/commands/Subsystem.h>
#include <frc/DigitalInput.h>
#include <wpi/StringRef.h>
#include <wpi/json.h>
#include "RobotMap.h"

#ifndef PROTOBOT
#   include <ctre/Phoenix.h>
#   include <frc/AnalogPotentiometer.h>
#   include <frc/PIDController.h>
#   include <frc/Relay.h>
#   include <frc/Servo.h>
#   include <frc/Spark.h>
#endif

class CargoIntake : public frc::Subsystem {
    public:
        CargoIntake(wpi::json&);
        void InitDefaultCommand() override;

        bool HasCargo();

        // HATCH functions

        // parallel w/ cargo-intake
        void GripHatchBottom() { m_HatchGripBottom.Set(0.7); }
        void GripHatchTop()    { m_HatchGripTop.Set(0.3); }
        // perpendicular (bottom goes down, top goes up)
        void ReleaseHatchBottom() { m_HatchGripBottom.Set(0.3); }
        void ReleaseHatchTop()    { m_HatchGripTop.Set(0.7); }

        void SetBottomHookPosition(wpi::StringRef configName);
        void SetBottomHookPosition(double);

        void SetTopHookPosition(wpi::StringRef configName);
        void SetTopHookPosition(double);

        void ExtendEjector();
        void RetractEjector();
        void SetEjector(double value);

        bool IsRotationDone();
        bool IsAtPosition(wpi::StringRef configName);
        void RotateToPosition(wpi::StringRef configName);
        void RotateToPosition(double position);
        void SetRotateSpeed(double speed);
        void StopRotation();

        void GoHome();
        double GetIntakeRotation();

        void RunReset();
        void Disable();

        bool IsRollerRunning();

    private:
        frc::DigitalInput m_CargoSensor {kCargoSensor};
    
    // Protobot must not use any of this.
#   ifndef PROTOBOT

    public:
        void SetRollerSpeed(wpi::StringRef configName);
        void SetRollerSpeed(double topspeed, double bottomspeed);
        void StopRoller();

    private:
        // frc::AnalogPotentiometer  m_IntakeRotation  {kCargoRotationSensor, 189, -108}; // Testbot
        // frc::AnalogPotentiometer  m_IntakeRotation  {kCargoRotationSensor, 211.8, -90.2}; // Testbot
        frc::AnalogPotentiometer  m_IntakeRotation     {kCargoRotationSensor, 1.0, 0}; // Compbot
        frc::Servo                m_HatchGripBottom    {kCargoHatchServoBottom};
        frc::Servo                m_HatchGripTop       {kCargoHatchServoTop};
        WPI_TalonSRX              m_IntakeArmMotor     {kCargoTalonRotate};
        WPI_TalonSRX              m_IntakeRollerTop    {kCargoTalonRollerTop};
        WPI_TalonSRX              m_IntakeRollerBottom {kCargoTalonRollerBottom};
        frc::Servo                m_CargoEjector       {kCargoEjectServo};

        frc::PIDController        m_RotationPID     {1, 0, 0, m_IntakeRotation, m_IntakeArmMotor};

        int m_InRangeCount = 0;

#   endif

    // Add variables for servo positions

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

        struct Config {
            double EjectorExtend = 0.0;
            double EjectorRetract = 0.0;
        };

        Config m_Config;
};
