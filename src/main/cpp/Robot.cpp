#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

// Initialize Subsystems
DriveTrain Robot::m_DriveTrain;
CreeperArm Robot::m_CreeperArm;
CargoIntake Robot::m_CargoIntake;
OI Robot::m_Oi;
// Initialize Commands
GrabHatchFromDispenser Robot::m_GrabHatchFromDispenser;
ReleaseHatch Robot::m_ReleaseHatch;
RotateCargoForCargoShip Robot::m_RotateCargoForCargoShip;

void Robot::RobotInit() {}

void Robot::RobotPeriodic() {}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
    if (m_Oi.GetDriverJoystick().GetAButtonPressed()) {
        m_RotateCargoForCargoShip.Start();
    }
    frc::Scheduler::GetInstance()->Run();
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
