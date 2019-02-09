#pragma once

#include "Robot.h"
#include <frc/commands/Command.h>

class RotateHatchForDispenser : public frc::Command {
    public:
        RotateHatchForDispenser();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
