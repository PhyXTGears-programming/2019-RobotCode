#pragma once

#include <frc/commands/Command.h>

#include "util/Delay.h"

enum Segment {
    Initialize,
    CheckSwitch,
    CheckArm,
    RollCreeper,
    StopCreeper,
    RaiseSolenoids,
    End
};

class ClimbStep : public frc::Command {
    public:
        ClimbStep();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
        Segment m_Segment = Segment::Initialize;
    private:
        bool m_HasPrerequisites;
        Delay m_Delay{2.5};
};
