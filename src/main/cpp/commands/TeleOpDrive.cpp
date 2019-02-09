#include "commands/TeleOpDrive.h"
#include "Robot.h"
#include "RobotMap.h"

TeleOpDrive::TeleOpDrive() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::m_DriveTrain);
}

void TeleOpDrive::Initialize() {}

void TeleOpDrive::Execute() {
    Robot::m_DriveTrain.Drive(Robot::m_Oi.GetDriverJoystick());
}

bool TeleOpDrive::IsFinished() { return false; }

// Make sure the motors stop moving when they aren't being controlled.
void TeleOpDrive::End()         { Robot::m_DriveTrain.Drive(0, 0); }
void TeleOpDrive::Interrupted() { Robot::m_DriveTrain.Drive(0, 0); }
