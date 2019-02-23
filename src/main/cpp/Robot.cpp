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

// Initialize Commands - Intake
GrabHatchFromDispenser*         Robot::m_GrabHatchFromDispenser;
ReleaseHatch*                   Robot::m_ReleaseHatch;
RotateCargoForCargoShip*        Robot::m_RotateCargoForCargoShip;
RotateCargoForLevelOneRocket*   Robot::m_RotateCargoForLevelOneRocket;
RotateHatchForFloor*            Robot::m_RotateHatchForFloor;
RotateHatchForDispenser*        Robot::m_RotateHatchForDispenser;

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

    // Allocate and initialize commands - Intake
    m_GrabHatchFromDispenser = new GrabHatchFromDispenser();
    m_ReleaseHatch = new ReleaseHatch();
    m_RotateCargoForCargoShip = new RotateCargoForCargoShip();
    m_RotateCargoForLevelOneRocket = new RotateCargoForLevelOneRocket();
    m_RotateHatchForFloor = new RotateHatchForFloor();
    m_RotateHatchForDispenser = new RotateHatchForDispenser();
    
    // Allocate and initialize commands - 
    m_ReadyCreeperArm = new ReadyCreeperArm();
    m_ClimbStep = new ClimbStep();
}

void Robot::RobotInit() {
    frc::CameraServer::GetInstance()->StartAutomaticCapture();
}

void Robot::RobotPeriodic() {
    frc::SmartDashboard::PutNumber("flight throttle", m_OI.GetOperatorJoystick().GetThrottle());
    frc::Scheduler::GetInstance()->Run();
}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {
    // Controls will go here.

    /* DRIVE CONTROLS
    

    */

    /* OPERATOR CONTROLS

    // Change rotation of cargo intake
    if m_OI::GetOperator().GetFloorHatchPickupPressed() then
        RotateHatchForFloor.Start()
    elif GetFloorCargoPickupPressed() then
        RotateCargoForFloor.Start()
    elif GetHatchFeederScorePressed() then
        RotateHatchForDispenser.Start()
    elif GetRocketShotPressed() then
        RotateCargoForLevelOneRocket.Start()
    elif GetCargoShotPressed() then
        RotateCargoForCargoShip.Start()
    elif GetGoHomePressed() then
        RotateIntakeToHome.Start()
    end

    // action command buttons, stuff happens
    if m_OI::GetOperator().GetCargoCloseShotPressed() then
        ShootCargo.Start() ??
    elif GetCargoHighShotPressed() then
        ShootCargoForLevelOneRocket.Start()
    elif GetHatchGrabPressed() then
        GrabHatchFromDispenser.start()
    elif GetHatchReleasePressed() then
        ReleaseHatch.Start()
    end

    // manual controls
    if m_OI::GetOperator().GetThrottle() >= 0.75 then
        Flightstick controls creeper arms
    else if m_OI::GetOperator().GetThrottle() <= -0.75 then
        Flightstick controls cargo intake
    end

    */
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
