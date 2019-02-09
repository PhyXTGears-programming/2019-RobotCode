#pragma once

#include <frc/commands/Command.h>

class RotateCargoForLevelTwoRocket : public frc::Command {
    public:
        RotateCargoForLevelTwoRocket();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
