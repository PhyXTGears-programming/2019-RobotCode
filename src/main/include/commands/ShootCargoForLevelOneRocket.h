#pragma once

#include <frc/commands/Command.h>

class ShootCargoForLevelOneRocket : public frc::Command {
    public:
        ShootCargoForLevelOneRocket();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
