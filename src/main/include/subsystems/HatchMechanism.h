#pragma once

#include <frc/commands/Subsystem.h>
#include <wpi/json.h>
#include "RobotMap.h"

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

    private:

        frc::Spark m_HatchMechanismMotor {kHatchMechanismMotor};
        
        frc::Solenoid m_SolRetractHatch {kPCM, AIR_2};
        frc::Solenoid m_SolExtendHatch  {kPCM, AIR_1};
};
