#pragma once

#include <frc/commands/Command.h>

class RotateHatchForPickup : public frc::Command {
    public:
        RotateHatchForPickup();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
