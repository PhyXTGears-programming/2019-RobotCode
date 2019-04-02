#pragma once

#include <frc/commands/Subsystem.h>
#include <frc/DigitalInput.h>
#include <wpi/StringRef.h>
#include <wpi/json.h>
#include "RobotMap.h"

#include <frc/Spark.h>

class HatchMechanism : public frc::Subsystem {
    public:
        HatchMechanism(wpi::json&);
        void InitDefaultCommand() override;

        void SetRotateSpeed(double);

    private:
        frc::Spark m_HatchMechanismMotor {kHatchMechanismMotor};
};
