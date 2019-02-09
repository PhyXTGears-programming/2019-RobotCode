#pragma once

#include <frc/commands/Command.h>

class ShootCargoForLevelTwoRocket : public frc::Command {
    public:
        ShootCargoForLevelTwoRocket();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
