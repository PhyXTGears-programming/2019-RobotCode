/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "util/Delay.h"

#include <frc/commands/Command.h>

class DriveSandstormStepWithHatch : public frc::Command {
    public:
        DriveSandstormStepWithHatch();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;

        enum class Action {
            WaitForLowerHatch,
            Go,
            WaitUntilLanding,
            End,
        };

    private:
        Action m_Action;

        Delay m_LowerHatchDelay{0.6};
        Delay m_RaiseHatchDelay{0.5};
        Delay m_WaitForLanding{1.6};
};
