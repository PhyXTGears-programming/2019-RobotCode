/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/SandstormPlatformDrive.h"
#include "Robot.h"

#include "util/StopWatch.h"

#include <iostream>

//using Action = SandstormPlatformDrive::Action;

static StopWatch logTimer;

SandstormPlatformDrive::SandstormPlatformDrive() : m_Action(Action::End) {
  // Use Requires() here to declare subsystem dependencies
  Requires(&Robot::GetDriveTrain());
  Requires(&Robot::GetCargoIntake());
}

// Called just before this Command runs the first time
void SandstormPlatformDrive::Initialize() {
    Robot::GetDriveTrain().UseDukesSpeedLimit();
    logTimer.Reset();
    m_DrivePeriod.Stop();

    m_Action = Action::Capture;

    std::cout << logTimer.Elapsed() << ": SandstormPlatformDrive.Initialize: Lowering Hatch" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void SandstormPlatformDrive::Execute() {
    DriveTrain& drive = Robot::GetDriveTrain();
    CargoIntake& intake = Robot::GetCargoIntake();

    if (ENABLE_DRIVETRAIN_CONTROL) {
        switch (m_Action) {
            case Action::Capture:
                drive.ArcadeDrive(0, 0);
                intake.RotateToPosition("cargo-ship-shoot");
                intake.SetRollerSpeed("intake");

                    
                    std::cout << logTimer.Elapsed()
                        << ": SandstormPlatformDrive.Execute: Intake in position."
                        << std::endl;


                    m_Action = Action::WaitForCapture;

                break;

            case Action::WaitForCapture:
                drive.ArcadeDrive(0, 0);

                if (intake.HasCargo()) {
                    
                    std::cout << logTimer.Elapsed()
                        << ": SandstormPlatformDrive.Execute: Cargo captured."
                        << std::endl;

                    intake.StopRoller();

                    m_DrivePeriod.Start();

                    drive.ArcadeDrive(-1, 0);
                    m_Action = Action::Go;
                }

                break;

            case Action::Go:
                drive.ArcadeDrive(-1, 0);

                if (m_DrivePeriod.IsDone()) {
                    m_DrivePeriod.Stop();

                    m_Action = Action::End;
                }

                break;
        }
    }
}

// Make this return true when this Command no longer needs to run execute()
bool SandstormPlatformDrive::IsFinished() {
    return Action::End == m_Action;
}

// Called once after isFinished returns true
void SandstormPlatformDrive::End() {
    std::cout << logTimer.Elapsed() << ": SandstormPlatformDrive.End" << std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SandstormPlatformDrive::Interrupted() {
    Robot::GetDriveTrain().UseNormalSpeedLimit();
}
