#pragma once

#include "util/Delay.h"

#include <frc/commands/Command.h>

class DriveSandstormStepWithHatch : public frc::Command {
    public:
        DriveSandstormStepWithHatch();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;

        enum class Action {
            WaitForLowerHatch,
            Go,
            WaitUntilLanding,
            End,
        };

    private:
        Action m_Action;

        Delay m_LowerHatchDelay{0.6};
        Delay m_RaiseHatchDelay{0.5};
        Delay m_WaitForLanding{1.6};
};
