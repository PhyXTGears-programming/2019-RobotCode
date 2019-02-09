#pragma once

#include <frc/TimedRobot.h>
#include <frc/commands/Command.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/PowerDistributionPanel.h>

// Subsystems
#include "subsystems/DriveTrain.h"
#include "subsystems/CreeperArm.h"
#include "subsystems/CargoIntake.h"
// Commands
#include "commands/TeleOpDrive.h"
#include "commands/GrabHatchFromDispenser.h"
#include "commands/ReleaseHatch.h"

#include "OI.h"
#include "RobotMap.h"

class Robot : public frc::TimedRobot {
    public:
        // Subsystems
        static DriveTrain m_DriveTrain;
        static CreeperArm m_CreeperArm;
        static CargoIntake m_CargoIntake;
        static OI m_Oi;
        // Commands
        static GrabHatchFromDispenser m_GrabHatchFromDispenser;
        static ReleaseHatch m_ReleaseHatch;
   
        void RobotInit() override;
        void RobotPeriodic() override;
        void DisabledInit() override;
        void DisabledPeriodic() override;
        void AutonomousInit() override;
        void AutonomousPeriodic() override;
        void TeleopInit() override;
        void TeleopPeriodic() override;
        void TestPeriodic() override;
   
    private:
        frc::SendableChooser<frc::Command*> m_Chooser;

        // Protobot doesn't have a CAN Bus to the PDP
#       ifndef PROTOBOT
            frc::PowerDistributionPanel m_PDP{0};
#       endif
};
