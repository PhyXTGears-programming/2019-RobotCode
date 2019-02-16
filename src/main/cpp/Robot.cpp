#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

// Initialize Subsystems
DriveTrain   Robot::m_DriveTrain;
CargoIntake* Robot::m_CargoIntake;
CreeperArm   Robot::m_CreeperArm;
OI           Robot::m_OI;
// Initialize Commands
GrabHatchFromDispenser*      Robot::m_GrabHatchFromDispenser;
ReleaseHatch                 Robot::m_ReleaseHatch;
RotateCargoForCargoShip      Robot::m_RotateCargoForCargoShip;
RotateCargoForLevelOneRocket Robot::m_RotateCargoForLevelOneRocket;
RotateHatchForFloor          Robot::m_RotateHatchForFloor;
RotateHatchForDispenser      Robot::m_RotateHatchForDispenser;

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
 
    Robot::m_CargoIntake = new CargoIntake(m_JsonConfig);
    m_GrabHatchFromDispenser = new GrabHatchFromDispenser();
}

void Robot::RobotInit() {
}

void Robot::RobotPeriodic() {
    frc::SmartDashboard::PutNumber("intake rotation", GetCargoIntake().GetIntakeRotation());
    frc::Scheduler::GetInstance()->Run();
}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
    if (m_OI.GetDriverJoystick().GetXButtonPressed()) {
        m_RotateHatchForFloor.Start();
    } else if (m_OI.GetDriverJoystick().GetAButtonPressed()) {
        m_RotateCargoForCargoShip.Start();
    } else if (m_OI.GetDriverJoystick().GetBButtonPressed()) {
        m_RotateCargoForLevelOneRocket.Start();
    } else if (m_OI.GetDriverJoystick().GetYButtonPressed()) {
        m_RotateHatchForDispenser.Start();
    }
    
    m_DriveTrain.Drive(0, 0);
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
