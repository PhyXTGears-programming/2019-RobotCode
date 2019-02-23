#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <wpi/json.h>

// Initialize Operator Interface
OI            Robot::m_OI;
// Initialize Subsystems
DriveTrain    Robot::m_DriveTrain;
CargoIntake*  Robot::m_CargoIntake;
CreeperClimb* Robot::m_CreeperClimb;
// Initialize Commands
GrabHatchFromDispenser*      Robot::m_GrabHatchFromDispenser;
ReleaseHatch                 Robot::m_ReleaseHatch;
RotateCargoForCargoShip      Robot::m_RotateCargoForCargoShip;
RotateCargoForLevelOneRocket Robot::m_RotateCargoForLevelOneRocket;
RotateHatchForFloor          Robot::m_RotateHatchForFloor;
RotateHatchForDispenser      Robot::m_RotateHatchForDispenser;

// Initialize Commands - Climb
ReadyCreeperArm* Robot::m_ReadyCreeperArm;
ClimbStep*       Robot::m_ClimbStep;

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
 
    // Allocate and initialize subsystems. 

    m_CargoIntake = new CargoIntake(m_JsonConfig);
    m_CreeperClimb = new CreeperClimb(m_JsonConfig);

    // Allocate and initialize commands.
    m_ReadyCreeperArm = new ReadyCreeperArm();
    m_ClimbStep = new ClimbStep();
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

void Robot::TeleopInit() {
    GetDriveTrain().RunReset();
}

void Robot::TeleopPeriodic() {
    frc::SmartDashboard::PutNumber("climb stage", m_ClimbStep->m_Segment);
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
