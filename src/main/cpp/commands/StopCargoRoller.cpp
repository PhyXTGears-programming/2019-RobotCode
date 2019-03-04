#include "commands/StopCargoRoller.h"
#include "Robot.h"

StopCargoRoller::StopCargoRoller() {
  Requires(&Robot::GetCargoIntake());
}

void StopCargoRoller::Initialize() {}

void StopCargoRoller::Execute() {
    Robot::GetCargoIntake().StopRoller();
}

bool StopCargoRoller::IsFinished() { return true; }

// Called once after isFinished returns true
void StopCargoRoller::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void StopCargoRoller::Interrupted() {}
