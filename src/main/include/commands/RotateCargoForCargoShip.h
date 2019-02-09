#pragma once

#include <frc/commands/Command.h>

class RotateCargoForCargoShip : public frc::Command {
    public:
        RotateCargoForCargoShip();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
