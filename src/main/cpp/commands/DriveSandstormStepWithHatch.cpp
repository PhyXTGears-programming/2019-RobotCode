#include "commands/DriveSandstormStepWithHatch.h"
#include "Robot.h"

#include "util/StopWatch.h"

#include <iostream>

static StopWatch logTimer;

DriveSandstormStepWithHatch::DriveSandstormStepWithHatch() : m_Action(Action::End) {
  // Use Requires() here to declare subsystem dependencies
  Requires(&Robot::GetDriveTrain());
  Requires(&Robot::GetCargoIntake());
}

// Called just before this Command runs the first time
void DriveSandstormStepWithHatch::Initialize() {
    Robot::GetDriveTrain().UseDukesSpeedLimit();
    m_LowerHatchDelay.Start();
    m_WaitForLanding.Start();
    m_RaiseHatchDelay.Stop();
    logTimer.Reset();

    m_Action = Action::WaitForLowerHatch;

    std::cout << logTimer.Elapsed() << ": DriveSandstormStepWithHatch.Initialize: Lowering Hatch" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void DriveSandstormStepWithHatch::Execute() {
    if (ENABLE_DRIVETRAIN_CONTROL) {
        switch (m_Action) {
            case Action::WaitForLowerHatch:
                Robot::GetDriveTrain().ArcadeDrive(0, 0);

                if (m_LowerHatchDelay.IsDone()) {
                    std::cout << logTimer.Elapsed() << ": DriveSandstormStepWithHatch.Execute: Driving now." << std::endl;

                    Robot::GetDriveTrain().ArcadeDrive(1, 0);
                    m_RaiseHatchDelay.Start();

                    m_Action = Action::Go;
                }
                break;

            case Action::Go:
                Robot::GetDriveTrain().ArcadeDrive(1, 0);

                if (m_RaiseHatchDelay.IsDone()) {
                    std::cout << logTimer.Elapsed() << ": DriveSandstormStepWithHatch.Execute: Raising hatch now." << std::endl;

                    m_RaiseHatchDelay.Stop();
                    Robot::GetHatchMechanism().RaiseHatch();

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
bool DriveSandstormStepWithHatch::IsFinished() {
    return Action::End == m_Action;
}

// Called once after isFinished returns true
void DriveSandstormStepWithHatch::End() {
    std::cout << logTimer.Elapsed() << ": DriveSandstormStepWithHatch.End" << std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveSandstormStepWithHatch::Interrupted() {
    Robot::GetDriveTrain().UseNormalSpeedLimit();
}
