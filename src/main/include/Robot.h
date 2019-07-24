#pragma once

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

#include <cameraserver/CameraServer.h>      // NOTE
#include <frc/TimedRobot.h>
#include <frc/PowerDistributionPanel.h>
#include <frc/commands/Command.h>
#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <wpi/StringRef.h>
#include <wpi/json.h>

// Subsystems
#include "subsystems/DriveTrain.h"
#include "subsystems/CreeperClimb.h"
#include "subsystems/CargoIntake.h"
#include "subsystems/HatchMechanism.h"
// Commands
#include "commands/ClimbStep.h"

#include "commands/DriveSandstormStepWithCargo.h"
#include "commands/DriveSandstormStepWithHatch.h"
#include "commands/DriveClimb.h"
#include "commands/TeleOpDrive.h"

#include "commands/GrabHatchFromLoadingStation.h"
#include "commands/LowerHatch.h"
#include "commands/RaiseHatch.h"
#include "commands/ReadyCreeperArm.h"
#include "commands/ReadyHatch.h"
#include "commands/ReadyTakeHatch.h"
#include "commands/RotateCargoForCargoShip.h"
#include "commands/RotateCargoForLevelOneRocket.h"
#include "commands/ShootCargoForCargoShip.h"
#include "commands/ShootCargoForLevelOneRocket.h"
#include "commands/ShootCargoForLevelTwoRocket.h"
#include "commands/StopCargoRoller.h"
#include "commands/TakeCargo.h"
#include "commands/TakeCargoFromDispenser.h"
#include "commands/TakeCargoFromFloor.h"

#include "Bling.h"

#include "OI.h"
#include "RobotMap.h"


class Robot : public frc::TimedRobot {
    public:
        // Operator Interface
        static OI                               m_OI;

        // Subsystems
        static DriveTrain*     m_DriveTrain;
        static CreeperClimb*   m_CreeperClimb;
        static CargoIntake*    m_CargoIntake;
        static HatchMechanism* m_HatchMechanism;
  
        // Commands - Drive
        static DriveSandstormStepWithHatch* m_DriveSandstormStepWithHatch;
        static DriveSandstormStepWithCargo* m_DriveSandstormStepWithCargo;
        static DriveClimb* m_DriveClimb;

        // Commands - Intake
        static RotateCargoForCargoShip*      m_RotateCargoForCargoShip;
        static RotateCargoForLevelOneRocket* m_RotateCargoForLevelOneRocket;

        static ShootCargoForCargoShip*      m_ShootCargoForCargoShip;
        static ShootCargoForLevelOneRocket* m_ShootCargoForLevelOneRocket;
        static ShootCargoForLevelTwoRocket* m_ShootCargoForLevelTwoRocket;

        static StopCargoRoller*        m_StopCargoRoller;
        static TakeCargo*              m_TakeCargo;
        static TakeCargoFromDispenser* m_TakeCargoFromDispenser;
        static TakeCargoFromFloor*     m_TakeCargoFromFloor;

        // Commands - Climb
        static ReadyCreeperArm* m_ReadyCreeperArm;
        static ClimbStep*       m_ClimbStep;

        // Commands - Hatch
        static LowerHatch*  m_LowerHatch;
        static RaiseHatch*  m_RaiseHatch;

        static GrabHatchFromLoadingStation*   m_GrabHatchFromLoadingStation;
        static ReadyHatch*                    m_ReadyHatch;
        static ReadyTakeHatch*                m_ReadyTakeHatch;

        // JSON Reader for Config (this should probably be moved later)
        static wpi::json m_JsonConfig;

        // Bling
        Bling m_Bling;
   
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

        static CargoIntake & GetCargoIntake()       { return *Robot::m_CargoIntake;    };
        static HatchMechanism & GetHatchMechanism() { return *Robot::m_HatchMechanism; };
        static CreeperClimb & GetCreeperClimb()     { return *Robot::m_CreeperClimb;   };
        static DriveTrain & GetDriveTrain()         { return *Robot::m_DriveTrain;     };

    private:
        frc::SendableChooser<frc::Command*> m_Chooser;

        bool m_UsingCamera1 = false;
        cs::UsbCamera m_Camera0;            // NOTE
        cs::UsbCamera m_Camera1;

        frc::AnalogInput m_AirPressureMeter{kAirPressureMeterPin};

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
        void JoystickDemoCargoEjector();
        void JoystickDemoCreeperClimb();
        void JoystickDemoHatchCheesecake();
        void JoystickDemoIntakeHatch();

        // Bling code
        void CompetitionBling();

        //
        void PrintVersionFile();
};
