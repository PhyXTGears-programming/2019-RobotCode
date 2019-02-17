#pragma once

#include <frc/commands/Command.h>

class TakeCargo : public frc::Command {
    public:
        TakeCargo();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;
};
