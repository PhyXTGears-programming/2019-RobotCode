#pragma once

#include <frc/TimedRobot.h>
#include <frc/commands/Command.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/PowerDistributionPanel.h>
#include <cameraserver/CameraServer.h>

// for json config
#include <wpi/StringRef.h>
#include <wpi/json.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

// Subsystems
#include "subsystems/DriveTrain.h"
#include "subsystems/CreeperClimb.h"
#include "subsystems/CargoIntake.h"
// Commands
#include "commands/ClimbStep.h"

#include "commands/DriveSandstormStepWithCargo.h"
#include "commands/DriveSandstormStepWithHatch.h"
#include "commands/TeleOpDrive.h"

#include "commands/GrabHatchFromDispenser.h"
#include "commands/ReadyCreeperArm.h"
#include "commands/ReleaseHatch.h"
#include "commands/RotateHatchForFloor.h"
#include "commands/RotateHatchForDispenser.h"
#include "commands/RotateCargoForCargoShip.h"
#include "commands/RotateCargoForLevelOneRocket.h"
#include "commands/ShootCargoForCargoShip.h"
#include "commands/ShootCargoForLevelOneRocket.h"
#include "commands/ShootCargoForLevelTwoRocket.h"
#include "commands/StopCargoRoller.h"
#include "commands/TakeCargo.h"
#include "commands/TakeCargoFromDispenser.h"
#include "commands/TakeCargoFromFloor.h"

#include "OI.h"
#include "RobotMap.h"

class Robot : public frc::TimedRobot {
    public:
        // Operator Interface
        static OI m_OI;

        // Subsystems
        static DriveTrain*                      m_DriveTrain;
        static CreeperClimb*                    m_CreeperClimb;
        static CargoIntake*                     m_CargoIntake;
  
        // Commands - Drive
        static DriveSandstormStepWithHatch*     m_DriveSandstormStepWithHatch;
        static DriveSandstormStepWithCargo*     m_DriveSandstormStepWithCargo;

        // Commands - Intake
        static GrabHatchFromDispenser*          m_GrabHatchFromDispenser;
        static ReleaseHatch*                    m_ReleaseHatch;
        static RotateHatchForFloor*             m_RotateHatchForFloor;
        static RotateHatchForDispenser*         m_RotateHatchForDispenser;
        static RotateCargoForCargoShip*         m_RotateCargoForCargoShip;
        static RotateCargoForLevelOneRocket*    m_RotateCargoForLevelOneRocket;

        static ShootCargoForCargoShip*          m_ShootCargoForCargoShip;
        static ShootCargoForLevelOneRocket*     m_ShootCargoForLevelOneRocket;
        static ShootCargoForLevelTwoRocket*     m_ShootCargoForLevelTwoRocket;

        static StopCargoRoller*                 m_StopCargoRoller;
        static TakeCargo*                       m_TakeCargo;
        static TakeCargoFromDispenser*          m_TakeCargoFromDispenser;
        static TakeCargoFromFloor*              m_TakeCargoFromFloor;

        // Commands - Climb
        static ReadyCreeperArm*                 m_ReadyCreeperArm;
        static ClimbStep*                       m_ClimbStep;

        // JSON Reader for Config (this should probably be moved later)
        static wpi::json                        m_JsonConfig;
   
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
        static CreeperClimb & GetCreeperClimb() { return *Robot::m_CreeperClimb; };
        static DriveTrain & GetDriveTrain() { return *Robot::m_DriveTrain; };

    private:
        frc::SendableChooser<frc::Command*> m_Chooser;

        bool m_CanSandstormStepDrive;

        // Protobot doesn't have a CAN Bus to the PDP
#       ifndef PROTOBOT
            frc::PowerDistributionPanel m_PDP{0};
#       endif

        // Control function for competition.
        void CompetitionJoytickInput();

        // Control functions for testing subsystems.
        void CompetitionJoystickInput();
        void ButtonBoardDemo();
        void JoystickDemoCargo();
        void JoystickDemoCreeperClimb();
        void JoystickDemoHatchCheesecake();
        void JoystickDemoIntakeHatch();
};
