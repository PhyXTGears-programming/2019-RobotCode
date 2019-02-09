#pragma once

#include <frc/commands/Command.h>

class TakeCargoFromDispenser : public frc::Command {
    public:
        TakeCargoFromDispenser();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
