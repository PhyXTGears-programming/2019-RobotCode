#pragma once

#include <frc/commands/Command.h>

class ReleaseHatch : public frc::Command {
    public:
        ReleaseHatch();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
