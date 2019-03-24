#pragma once

#include <frc/commands/Command.h>

#include "util/Delay.h"

enum Segment {
    Initialize,
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

        Segment GetSegment() { return m_Segment; }
    private:
        bool m_HasPrerequisites;
        Delay m_Delay{2};         // Roll creeper delay.
        Segment m_Segment = Segment::Initialize;
};
