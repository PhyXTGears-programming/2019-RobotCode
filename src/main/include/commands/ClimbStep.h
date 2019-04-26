#pragma once

#include <frc/commands/Command.h>

#include "util/Delay.h"

enum Segment {
    Initialize,
    CheckArm,
    RollCreeper,
    StopCreeper,
    RaiseCylinder,
    StopCylinder,
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
        Delay m_CrawlDelay{1.4}; // Amount of time to roll creeper crawl wheels
        Delay m_StopCylinderDelay{2.5};
        Segment m_Segment = Segment::Initialize;
};
