#pragma once

#include <frc/commands/Command.h>

class TakeCargoFromFloor : public frc::Command {
    public:
        TakeCargoFromFloor();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;

    private:
        int sensorCount = 0;
        bool hadCargo = false;
};
