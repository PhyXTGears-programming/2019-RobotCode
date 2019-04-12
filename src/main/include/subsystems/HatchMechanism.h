#pragma once

#include <frc/commands/Subsystem.h>
#include <wpi/json.h>
#include "RobotMap.h"

#include <frc/AnalogPotentiometer.h>
#include <frc/PIDController.h>
#include <frc/Spark.h>
#include <frc/Solenoid.h>

class HatchMechanism : public frc::Subsystem {
    public:
        HatchMechanism(wpi::json&);
        void InitDefaultCommand() override;

        void SetRotateSpeed(double);

        // Move up and down via motor speed.  No PID.
        void RaiseHatch();
        void LowerHatch();

        // Use PID to hold position.
        void RotateToTopPosition();
        void RotateToMidPosition();

        // Stop moving arm.  Zero speed, disable PID.
        void StopRotation();

        // Run solenoid gripper.
        void GrabHatch();
        void ReleaseHatch();

        double GetArmPosition() { return m_ArmPosition.Get(); }

        bool IsArmRotationDone();

        void Disable();

        bool IsAboveGripThreshold() { return GetArmPosition() <= m_Config.GripThreshold; }

    private:
        frc::Spark m_ArmMotor {kHatchArmMotor};

        frc::AnalogPotentiometer m_ArmPosition {kHatchArmPositionSensor, 5.0, 0.0};
        frc::PIDController m_ArmPID {1, 0, 0, m_ArmPosition, m_ArmMotor};

        frc::Solenoid m_SolRetractHatch {kPCM, AIR_2};
        frc::Solenoid m_SolExtendHatch  {kPCM, AIR_1};

        int m_InRangeCount = 0;

        struct Config {
            double TopPosition = 0.0;
            double MidPosition = 0.0;
            double GripThreshold = 0.0;
        };

        Config m_Config;
};
