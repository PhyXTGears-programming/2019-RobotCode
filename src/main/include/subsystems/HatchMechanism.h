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

        // Move up and down
        void RaiseHatch();
        void LowerHatch();

        // Stop Moving
        void StopRotation();

        // Run solenoid
        void GrabHatch();
        void ReleaseHatch();

        double GetArmPosition() { return m_ArmPosition.Get(); }

        bool IsArmRotationDone();

        void Disable();

    private:
        frc::Spark m_ArmMotor {kHatchArmMotor};

        frc::AnalogPotentiometer m_ArmPosition {kHatchArmPositionSensor, 5.0, 0.0};
        frc::PIDController m_ArmPID {1, 0, 0, m_ArmPosition, m_ArmMotor};

        frc::Solenoid m_SolRetractHatch {kPCM, AIR_2};
        frc::Solenoid m_SolExtendHatch  {kPCM, AIR_1};

        int m_InRangeCount = 0;
};
