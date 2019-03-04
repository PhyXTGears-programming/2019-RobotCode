#include "commands/TeleOpDrive.h"
#include "Robot.h"
#include "RobotMap.h"

TeleOpDrive::TeleOpDrive() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::GetDriveTrain());
}

void TeleOpDrive::Initialize() {
    Robot::GetDriveTrain().UseNormalSpeedLimit();
}

void TeleOpDrive::Execute() {
    if (ENABLE_DRIVETRAIN_CONTROL) {
        Robot::GetDriveTrain().Drive(Robot::m_OI.GetDriverJoystick());
    }
}

bool TeleOpDrive::IsFinished() { return false; }

// Make sure the motors stop moving when they aren't being controlled.
void TeleOpDrive::End()         { Robot::GetDriveTrain().Drive(0, 0); }
void TeleOpDrive::Interrupted() {}
