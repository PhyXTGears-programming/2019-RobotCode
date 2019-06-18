#include "commands/DriveClimb.h"
#include "Robot.h"

#include <iostream>

#define kDRIVE_SPEED -0.10

DriveClimb::DriveClimb() {
  // Use Requires() here to declare subsystem dependencies
  Requires(&Robot::GetDriveTrain());
}

// Called just before this Command runs the first time
void DriveClimb::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void DriveClimb::Execute() {
    Robot::GetDriveTrain().ArcadeDrive(kDRIVE_SPEED, 0);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveClimb::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void DriveClimb::End() {
    Robot::GetDriveTrain().ArcadeDrive(0, 0);    
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveClimb::Interrupted() {
    Robot::GetDriveTrain().ArcadeDrive(0, 0);
}
