#pragma once

#include "util/Delay.h"

#include <frc/commands/Command.h>

class DriveSandstormStepWithCargo : public frc::Command {
    public:
        DriveSandstormStepWithCargo();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;

        enum class Action {
            Capture,
            WaitForCapture,
            Go,
            End,
        };

    private:
        Action m_Action;

        Delay m_DrivePeriod{1.0};
};
