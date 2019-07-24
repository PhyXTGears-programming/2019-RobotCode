#pragma once

#include <frc/commands/Command.h>

class ReadyTakeHatch : public frc::Command {
    public:
        ReadyTakeHatch();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
