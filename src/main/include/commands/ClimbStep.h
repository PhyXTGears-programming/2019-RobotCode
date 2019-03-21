#pragma once

#include <frc/commands/Command.h>

#include <frc/PIDController.h>

#include "util/Delay.h"
#include "AngleSource.h"

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
        Delay m_Delay{2.5};         // Roll creeper delay.
        Segment m_Segment = Segment::Initialize;

        AngleSource m_PitchSource{AngleSource::kPitch};
        frc::PIDController* m_LevelingPID;
};
