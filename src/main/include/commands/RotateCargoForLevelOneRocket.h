#pragma once

#include <frc/commands/Command.h>

class RotateCargoForLevelOneRocket : public frc::Command {
    public:
        RotateCargoForLevelOneRocket();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
