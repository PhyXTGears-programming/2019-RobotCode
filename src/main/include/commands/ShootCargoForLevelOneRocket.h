#pragma once

#include "util/Delay.h"

#include <frc/commands/Command.h>

class ShootCargoForLevelOneRocket : public frc::Command {
    public:
        ShootCargoForLevelOneRocket();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        bool IsInterruptible();
        void End() override;
        void Interrupted() override;

        enum class Action {
            TurnOnRollers,
            WaitForSpeed,
            WaitForEjector,
            End,
        };

    private:
        bool m_HasPrerequisites;
        Delay m_WaitForRollers{0.5};
        Delay m_WaitForEjector{0.75};
        Action m_Action;
};
