#pragma once

#include <frc/commands/Command.h>

class TeleOpDrive : public frc::Command {
    public:
        TeleOpDrive();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
