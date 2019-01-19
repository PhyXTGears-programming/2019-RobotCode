/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/TeleOpDrive.h"
#include "Robot.h"
#include "RobotMap.h"

TeleOpDrive::TeleOpDrive() {
    Requires(&Robot::m_DriveTrain);
}

// Called just before this Command runs the first time
void TeleOpDrive::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void TeleOpDrive::Execute() {
    // Robot::m_DriveTrain->Drive(1,1);
}

// Make this return true when this Command no longer needs to run execute()
bool TeleOpDrive::IsFinished() { return false; }

// Called once after isFinished returns true
void TeleOpDrive::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TeleOpDrive::Interrupted() {}
