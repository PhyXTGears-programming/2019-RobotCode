#pragma once

#include <frc/commands/Command.h>

class GrabHatchFromDispenser : public frc::Command {
    public:
        GrabHatchFromDispenser();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
