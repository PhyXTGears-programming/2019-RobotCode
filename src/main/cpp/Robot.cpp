#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

// Initialize Operator Interface
OI            Robot::m_OI;
// Initialize Subsystems
DriveTrain    Robot::m_DriveTrain;
CargoIntake*  Robot::m_CargoIntake;
CreeperClimb* Robot::m_CreeperClimb;
// Initialize Commands
GrabHatchFromDispenser* Robot::m_GrabHatchFromDispenser;
ReleaseHatch            Robot::m_ReleaseHatch;
RotateCargoForCargoShip Robot::m_RotateCargoForCargoShip;

// Initialize JSON reader
wpi::json Robot::m_JsonConfig;

Robot::Robot() {
    // get the json config deployed onto the roborio
    std::ifstream jsonStream("/home/lvuser/deploy/config.json");
    std::string jsonString;

    // preallocate memory for string
    jsonStream.seekg(0, std::ios::end);
    jsonString.reserve(jsonStream.tellg());
    jsonStream.seekg(0, std::ios::beg);

    // pass the file data into the string
    jsonString.assign((std::istreambuf_iterator<char>(jsonStream)),
        std::istreambuf_iterator<char>());

    //m_ConfigReader = new wpi::json(str);
    m_JsonConfig = wpi::json::parse(jsonString);
 
    m_CargoIntake = new CargoIntake(m_JsonConfig);
    m_GrabHatchFromDispenser = new GrabHatchFromDispenser();

    m_CreeperClimb = new CreeperClimb();
}

void Robot::RobotInit() {
}

void Robot::RobotPeriodic() {}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {
    frc::SmartDashboard::PutNumber("intake rotation", GetCargoIntake().GetIntakeRotation());
    frc::Scheduler::GetInstance()->Run();
}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
    if (m_OI.GetDriverJoystick().GetAButtonPressed()) {
        m_RotateCargoForCargoShip.Start();
    }
    frc::Scheduler::GetInstance()->Run();
    
    m_DriveTrain.Drive(0, 0);
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
