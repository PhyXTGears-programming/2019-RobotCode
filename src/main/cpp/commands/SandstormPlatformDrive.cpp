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
    Robot::GetCargoIntake().SetHatchRotateSpeed(0.3);
    m_LowerHatchDelay.Start();
    m_WaitForLanding.Start();
    m_RaiseHatchDelay.Stop();
    logTimer.Reset();

    m_Action = Action::WaitForLowerHatch;

    std::cout << logTimer.Elapsed() << ": SandstormPlatformDrive.Initialize: Lowering Hatch" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void SandstormPlatformDrive::Execute() {
    if (ENABLE_DRIVETRAIN_CONTROL) {
        switch (m_Action) {
            case Action::WaitForLowerHatch:
                Robot::GetDriveTrain().ArcadeDrive(0, 0);

                if (m_LowerHatchDelay.IsDone()) {
                    std::cout << logTimer.Elapsed() << ": SandstormPlatformDrive.Execute: Driving now." << std::endl;

                    Robot::GetDriveTrain().ArcadeDrive(1, 0);
                    m_RaiseHatchDelay.Start();

                    m_Action = Action::Go;
                }
                break;

            case Action::Go:
                Robot::GetDriveTrain().ArcadeDrive(1, 0);

                if (m_RaiseHatchDelay.IsDone()) {
                    std::cout << logTimer.Elapsed() << ": SandstormPlatformDrive.Execute: Raising hatch now." << std::endl;

                    m_RaiseHatchDelay.Stop();
                    Robot::GetCargoIntake().SetHatchRotateSpeed(-0.5);

                    m_Action = Action::WaitUntilLanding;
                }
                break;

            case Action::WaitUntilLanding:
                if (m_WaitForLanding.IsDone()) {
                    m_WaitForLanding.Stop();

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
