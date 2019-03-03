#include "Robot.h"
#include "util.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <wpi/json.h>

// Initialize Operator Interface
OI            Robot::m_OI;
// Initialize Subsystems
DriveTrain*   Robot::m_DriveTrain;
CargoIntake*  Robot::m_CargoIntake;
CreeperClimb* Robot::m_CreeperClimb;

// Initialize Commands - Intake
GrabHatchFromDispenser*         Robot::m_GrabHatchFromDispenser;
ReleaseHatch*                   Robot::m_ReleaseHatch;
RotateCargoForCargoShip*        Robot::m_RotateCargoForCargoShip;
RotateCargoForLevelOneRocket*   Robot::m_RotateCargoForLevelOneRocket;
RotateHatchForFloor*            Robot::m_RotateHatchForFloor;
RotateHatchForDispenser*        Robot::m_RotateHatchForDispenser;

ShootCargoForCargoShip*         Robot::m_ShootCargoForCargoShip;

TakeCargo*                      Robot::m_TakeCargo;
TakeCargoFromFloor*             Robot::m_TakeCargoFromFloor;

// Initialize Commands - Climb
ReadyCreeperArm* Robot::m_ReadyCreeperArm;
ClimbStep*       Robot::m_ClimbStep;

// Initialize JSON reader
wpi::json Robot::m_JsonConfig;

Robot::Robot() {
    // get the json config deployed onto the roborio
    std::ifstream jsonStream(CONFIGPATH);
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
    m_DriveTrain = new DriveTrain(m_JsonConfig);

    // Allocate and initialize commands - Intake
    m_GrabHatchFromDispenser = new GrabHatchFromDispenser();
    m_ReleaseHatch = new ReleaseHatch();
    m_RotateCargoForCargoShip = new RotateCargoForCargoShip();
    m_RotateCargoForLevelOneRocket = new RotateCargoForLevelOneRocket();
    m_RotateHatchForFloor = new RotateHatchForFloor();
    m_RotateHatchForDispenser = new RotateHatchForDispenser();

    m_ShootCargoForCargoShip = new ShootCargoForCargoShip();

    m_TakeCargo = new TakeCargo();
    m_TakeCargoFromFloor = new TakeCargoFromFloor();
    
    // Allocate and initialize commands - 
    m_ReadyCreeperArm = new ReadyCreeperArm();
    m_ClimbStep = new ClimbStep();
}

void Robot::RobotInit() {
    frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
    frc::CameraServer::GetInstance()->StartAutomaticCapture(1);
}

void Robot::RobotPeriodic() {
    frc::SmartDashboard::PutNumber("flight throttle", m_OI.GetOperatorJoystick().GetThrottle());
    frc::SmartDashboard::PutNumber("intake rotation", GetCargoIntake().GetIntakeRotation());
    frc::SmartDashboard::PutNumber("climb arm rotation", GetCreeperClimb().GetCurrentArmPosition());
    frc::SmartDashboard::PutNumber("climb stage", m_ClimbStep->GetSegment());
    frc::Scheduler::GetInstance()->Run();
}

void Robot::DisabledInit() {
    m_ClimbStep->Cancel();
}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
    GetDriveTrain().RunReset();
}

void Robot::TeleopPeriodic() {
    // No control code goes here.  Put control code for testing in a new
    // JoystickDemo method, or control code for competition in
    // CompetitionJoytickInput().
    //
    // TestPeriodic() will only invoke CompetitionJoytickInput(), and we can
    // swap out a JoystickDemo method for testing, but that modification to
    // TeleopPeriodic() had better not be committed to the develop branch, or
    // so help me, more words will ensue.
    //
    // (╯°Д°）╯︵┻━┻
}

void Robot::TestPeriodic() {}

void Robot::CompetitionJoytickInput() {
    // Competition Controls will go here.

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

void Robot::JoystickDemoCargo() {
    frc::XboxController& driver = m_OI.GetDriverJoystick();

    // X + trigger = rotate cargo intake.
    // Y = rotate cargo intake to cargo ship.
    // A = shoot cargo at cargo ship.
    // B = take cargo from floor.
    // R Bumper = take cargo from loading station.

    if (driver.GetXButton()) {
        double leftTrigger = driver.GetTriggerAxis(frc::XboxController::kLeftHand);
        double rightTrigger = driver.GetTriggerAxis(frc::XboxController::kRightHand);

        if (0.1 < leftTrigger) {
            GetCargoIntake().SetRotateSpeed(leftTrigger * 1.0);
        } else if (0.1 < rightTrigger) {
            GetCargoIntake().SetRotateSpeed(rightTrigger * -1.0);
        } else {
            GetCargoIntake().SetRotateSpeed(0.0);
        }
    } else if (driver.GetXButtonReleased()) {
        GetCargoIntake().SetRotateSpeed(0.0);
    }

    if (driver.GetYButtonPressed()) {
        m_RotateCargoForCargoShip->Start();
    }

    if (driver.GetAButtonPressed()) {
        m_ShootCargoForCargoShip->Start();
    }

    if (driver.GetBButtonPressed()) {
        m_TakeCargoFromFloor->Start();
    }

    if (driver.GetBumperPressed(frc::XboxController::kRightHand)) {
        m_TakeCargo->Start();
    }
}

void Robot::JoystickDemoCreeperClimb() {
    frc::XboxController& driver = m_OI.GetDriverJoystick();

    // Y = Ready creeper arm.
    // A = Start climb sequence.
    //
    // X + Trigger = Rotate creeper arm.
    //
    // POV Down = extend piston.
    // POV Up = retract piston.
    //
    // Back = rotate creeper arm to home position.
    // Start (hold) = engage creeper arm wheels.

    if (driver.GetYButtonPressed()) {
        m_ReadyCreeperArm->Start();
    } else if (driver.GetAButtonPressed()) {
        m_ClimbStep->Start();
    }

    if (driver.GetXButton()) {
        double leftTrigger = driver.GetTriggerAxis(frc::XboxController::kLeftHand);
        double rightTrigger = driver.GetTriggerAxis(frc::XboxController::kRightHand);

        if (0.1 < leftTrigger) {
            GetCreeperClimb().SetArmRotateSpeed(leftTrigger * -1.0);
        } else if (0.1 < rightTrigger) {
            GetCreeperClimb().SetArmRotateSpeed(rightTrigger * 1.0);
        } else {
            GetCreeperClimb().SetArmRotateSpeed(0.0);
        }
    } else if (driver.GetXButtonReleased()) {
        GetCreeperClimb().SetArmRotateSpeed(0.0);
    }

    int pov = driver.GetPOV();
    if (180 == pov) {
        Robot::GetCreeperClimb().PistonExtend();
    } else if (0 == pov) {
        Robot::GetCreeperClimb().PistonRetract();
    }
    
    if (driver.GetBackButtonPressed()) {
        Robot::GetCreeperClimb().RotateArmToPosition("home");
    }

    if (driver.GetStartButton()) {
        Robot::GetCreeperClimb().SetArmWheels(true); // wheels need to be SLOWED!!
        std::cout << "start" << std::endl;
    } else if (driver.GetStartButtonReleased()) {
        Robot::GetCreeperClimb().StopArmWheels();
    }
}

void Robot::JoystickDemoHatchCheesecake() {
    frc::XboxController& driver = m_OI.GetDriverJoystick();

    if (driver.GetXButton()) {
        double leftTrigger = driver.GetTriggerAxis(frc::XboxController::kLeftHand);
        double rightTrigger = driver.GetTriggerAxis(frc::XboxController::kRightHand);

        if (0.01 < leftTrigger) {
            GetCargoIntake().SetHatchRotateSpeed(leftTrigger * 0.5);
        } else if (0.01 < rightTrigger) {
            GetCargoIntake().SetHatchRotateSpeed(rightTrigger * -0.5);
        } else {
            GetCargoIntake().SetHatchRotateSpeed(0.0);
        }
    } else if (driver.GetXButtonReleased()) {
        GetCargoIntake().SetHatchRotateSpeed(0.0);
    }
}

void Robot::JoystickDemoIntakeHatch() {
    frc::XboxController& driver = m_OI.GetDriverJoystick();

    static double bottom = 0.0;
    static double top = 0.0;

    // POV Up + Y = Increase top hatch servo position up to 1.0.
    // POV Up + X = Decrease top hatch servo position down to 0.0.
    //
    // POV Down + Y = Increase bottom hatch servo position up to 1.0.
    // POV Down + X = Decrease bottom hatch servo position down to 0.0.


    int pov = driver.GetPOV();

    switch (pov) {
        case 0:   // Up
            if (driver.GetYButtonPressed()) {
                top = util::clamp(top + 0.01, 0.2, 0.8);
            } else if (driver.GetXButtonPressed()) {
                top = util::clamp(top - 0.01, 0.2, 0.8);
            }
            GetCargoIntake().SetTopHookPosition(top);
            break;

        case 180:    // Down
            if (driver.GetYButtonPressed()) {
                bottom = util::clamp(bottom + 0.01, 0.2, 0.8);
            } else if (driver.GetXButtonPressed()) {
                bottom = util::clamp(bottom - 0.01, 0.2, 0.8);
            }
            GetCargoIntake().SetBottomHookPosition(bottom);
            break;
    }

    std::cout << "hook: b(" << bottom << ") t(" << top << ")" << std::endl;
}



#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
