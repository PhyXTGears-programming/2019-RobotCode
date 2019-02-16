#pragma once

#include <frc/TimedRobot.h>
#include <frc/commands/Command.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/PowerDistributionPanel.h>

// for json config
#include <wpi/StringRef.h>
#include <wpi/json.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

// Subsystems
#include "subsystems/DriveTrain.h"
#include "subsystems/CreeperArm.h"
#include "subsystems/CargoIntake.h"
// Commands
#include "commands/TeleOpDrive.h"
#include "commands/GrabHatchFromDispenser.h"
#include "commands/ReleaseHatch.h"
#include "commands/RotateCargoForCargoShip.h"

#include "OI.h"
#include "RobotMap.h"

class Robot : public frc::TimedRobot {
    public:
        // Subsystems
        static DriveTrain   m_DriveTrain;
        static CreeperArm   m_CreeperArm;
        static CargoIntake* m_CargoIntake;
        static OI m_Oi;
        // Commands
        static GrabHatchFromDispenser* m_GrabHatchFromDispenser;
        static ReleaseHatch m_ReleaseHatch;
        static RotateCargoForCargoShip m_RotateCargoForCargoShip;

        // this should probably be moved later
        static wpi::json m_JsonConfig;
   
        Robot();
        void RobotInit() override;
        void RobotPeriodic() override;
        void DisabledInit() override;
        void DisabledPeriodic() override;
        void AutonomousInit() override;
        void AutonomousPeriodic() override;
        void TeleopInit() override;
        void TeleopPeriodic() override;
        void TestPeriodic() override;

        static CargoIntake & GetCargoIntake() { return *Robot::m_CargoIntake; };
   
    private:
        frc::SendableChooser<frc::Command*> m_Chooser;

        // Protobot doesn't have a CAN Bus to the PDP
#       ifndef PROTOBOT
            frc::PowerDistributionPanel m_PDP{0};
#       endif
};
