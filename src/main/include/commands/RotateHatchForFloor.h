#pragma once

#include "Robot.h"
#include <frc/commands/Command.h>

class RotateHatchForFloor : public frc::Command {
    public:
        RotateHatchForFloor();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
