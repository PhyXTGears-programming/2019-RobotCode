#pragma once

#include "util/Delay.h"

#include <frc/commands/Command.h>

class DriveClimb : public frc::Command {
    public:
        DriveClimb();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
};
