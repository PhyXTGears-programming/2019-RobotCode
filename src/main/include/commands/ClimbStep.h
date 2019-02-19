#pragma once

#include <frc/commands/Command.h>

class ClimbStep : public frc::Command {
    public:
        ClimbStep();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;
    private:
        int m_Segment = 0;
        bool m_HasPrerequisites;
};
