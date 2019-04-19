#pragma once

#include <frc/commands/Command.h>

class ReadyHatch : public frc::Command {
    public:
        ReadyHatch();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
