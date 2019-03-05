/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "util/Delay.h"

#include <frc/commands/Command.h>

class SandstormPlatformDrive : public frc::Command {
    public:
        SandstormPlatformDrive();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;

        enum class Action {
            Capture,
            WaitForCapture,
            Go,
            End,
        };

    private:
        Action m_Action;

        Delay m_DrivePeriod{1.0};
};
