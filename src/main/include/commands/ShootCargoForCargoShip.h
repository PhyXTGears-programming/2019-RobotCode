#pragma once

#include <frc/commands/Command.h>

class ShootCargoForCargoShip : public frc::Command {
    public:
        ShootCargoForCargoShip();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
