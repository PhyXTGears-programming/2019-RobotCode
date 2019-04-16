#include "Robot.h"

#include "util/util.h"

// Select hatch/piston controls
//
// 0 = Tippecanoe controls.  No PID hatch arm.  2 hatch buttons.
// 1 = Kokomo controls.  PID hatch arm.  3 hatch buttons.
#define HATCH_CONTROL_TYPE 1

// Initialize Operator Interface
OI Robot::m_OI;
// Initialize Subsystems
DriveTrain*     Robot::m_DriveTrain;
CargoIntake*    Robot::m_CargoIntake;
CreeperClimb*   Robot::m_CreeperClimb;
HatchMechanism* Robot::m_HatchMechanism;

// Initialize Commands - Intake
RotateCargoForCargoShip*      Robot::m_RotateCargoForCargoShip;
RotateCargoForLevelOneRocket* Robot::m_RotateCargoForLevelOneRocket;

ShootCargoForCargoShip*      Robot::m_ShootCargoForCargoShip;
ShootCargoForLevelOneRocket* Robot::m_ShootCargoForLevelOneRocket;
ShootCargoForLevelTwoRocket* Robot::m_ShootCargoForLevelTwoRocket;

StopCargoRoller*        Robot::m_StopCargoRoller;
TakeCargo*              Robot::m_TakeCargo;
TakeCargoFromDispenser* Robot::m_TakeCargoFromDispenser;
TakeCargoFromFloor*     Robot::m_TakeCargoFromFloor;

// Initialize Commands - Climb
ReadyCreeperArm* Robot::m_ReadyCreeperArm;
ClimbStep*       Robot::m_ClimbStep;

// Initialize Commands - Drive
DriveSandstormStepWithCargo* Robot::m_DriveSandstormStepWithCargo;
DriveSandstormStepWithHatch* Robot::m_DriveSandstormStepWithHatch;

// Initialize Commands - Hatch
LowerHatch* Robot::m_LowerHatch;
RaiseHatch* Robot::m_RaiseHatch;

GrabHatchFromLoadingStation*  Robot::m_GrabHatchFromLoadingStation;
ReadyHatch*                   Robot::m_ReadyHatch;

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

    m_JsonConfig = wpi::json::parse(jsonString);

    // Allocate and initialize subsystems.
    m_CargoIntake    = new CargoIntake(m_JsonConfig);
    m_HatchMechanism = new HatchMechanism(m_JsonConfig);
    m_CreeperClimb   = new CreeperClimb(m_JsonConfig);
    m_DriveTrain     = new DriveTrain(m_JsonConfig);

    // Allocate and initialize commands - Teleop
    m_DriveSandstormStepWithCargo = new DriveSandstormStepWithCargo();
    m_DriveSandstormStepWithHatch = new DriveSandstormStepWithHatch();

    // Allocate and initialize commands - Intake
    m_RotateCargoForCargoShip      = new RotateCargoForCargoShip();
    m_RotateCargoForLevelOneRocket = new RotateCargoForLevelOneRocket();

    m_ShootCargoForCargoShip      = new ShootCargoForCargoShip();
    m_ShootCargoForLevelOneRocket = new ShootCargoForLevelOneRocket();
    m_ShootCargoForLevelTwoRocket = new ShootCargoForLevelTwoRocket();

    m_StopCargoRoller        = new StopCargoRoller();
    m_TakeCargo              = new TakeCargo();
    m_TakeCargoFromDispenser = new TakeCargoFromDispenser();
    m_TakeCargoFromFloor     = new TakeCargoFromFloor();

    // Allocate and initialize commands -
    m_ReadyCreeperArm = new ReadyCreeperArm();
    m_ClimbStep       = new ClimbStep();

    // Allocate and initialize commands - Hatch
    m_GrabHatchFromLoadingStation  = new GrabHatchFromLoadingStation();
    m_ReadyHatch                   = new ReadyHatch();
    m_RaiseHatch                   = new RaiseHatch();
    m_LowerHatch                   = new LowerHatch();
}

void Robot::RobotInit() {
    m_Camera0 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
    m_Camera1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(1);

    // m_Camera0.SetResolution(320, 240);
    // m_Camera1.SetResolution(320, 240);
    // m_Camera0.SetFPS(15);
    // m_Camera1.SetFPS(15);
    m_Camera0.SetConnectionStrategy(cs::VideoSource::ConnectionStrategy::kConnectionKeepOpen);
    m_Camera1.SetConnectionStrategy(cs::VideoSource::ConnectionStrategy::kConnectionKeepOpen);

    frc::CameraServer::GetInstance()->GetServer().SetSource(m_Camera0);

    m_OI.ClearButtonBuffer();
}

void Robot::RobotPeriodic() {
    frc::SmartDashboard::PutNumber("flight throttle", m_OI.GetOperatorConsole().GetThrottle());
    frc::SmartDashboard::PutNumber("intake rotation", GetCargoIntake().GetIntakeRotation());
    frc::SmartDashboard::PutNumber("climb arm rotation", GetCreeperClimb().GetCurrentArmPosition());
    frc::SmartDashboard::PutNumber("climb stage", m_ClimbStep->GetSegment());
    const double psiPerVolt = 150.0 / 1.1;
    frc::SmartDashboard::PutNumber("air pressure", (m_AirPressureMeter.GetVoltage() - 0.3) * psiPerVolt);
    frc::SmartDashboard::PutBoolean("climb ready", GetCreeperClimb().IsArmAtPosition("arm-ready"));
    frc::SmartDashboard::PutBoolean("climb done", GetCreeperClimb().IsArmAtPosition("arm-climb"));

    {
        static bool lastCargoState = false;
        bool cargoState = m_CargoIntake->HasCargo();

        if (lastCargoState != cargoState) {
            std::cout
                << "Cargo "
                << (cargoState ? "detected" : "not detected")
                << std::endl;
            lastCargoState = cargoState;
        }
    }

    frc::Scheduler::GetInstance()->Run();

    bool bumperPressed = m_OI.GetDriverJoystick().GetBumperPressed(frc::XboxController::kRightHand);
    bool flightstickPressed = m_OI.GetOperatorConsole().GetCameraSwapPressed();

    if (bumperPressed || flightstickPressed) {
        if (m_UsingCamera1) {
            frc::CameraServer::GetInstance()->GetServer().SetSource(m_Camera0);
        } else {
            frc::CameraServer::GetInstance()->GetServer().SetSource(m_Camera1);
        }
        m_UsingCamera1 = !m_UsingCamera1;
    }
}

void Robot::DisabledInit() {
    PrintVersionFile();

    m_ClimbStep->Cancel();

    GetCreeperClimb().Disable();
    GetCargoIntake().Disable();
    GetHatchMechanism().Disable();

    // Clear pending commands out of scheduler.
    // frc::Scheduler::GetInstance()->ResetAll();

    m_OI.ClearButtonBuffer();
}

void Robot::DisabledPeriodic() {
    frc::Scheduler::GetInstance()->Run();
}

void Robot::AutonomousInit() {
    GetDriveTrain().RunReset();
    GetCreeperClimb().RunReset();
    GetCargoIntake().RunReset();

    // Some positions of hatch mechanism are compliant, so disable
    // PID/rotation instead of RunReset.
    GetHatchMechanism().StopRotation();

    m_CanSandstormStepDrive = true;

    m_OI.ClearButtonBuffer();
}

void Robot::AutonomousPeriodic() {
    TeleopPeriodic();
}

void Robot::TeleopInit() {
    m_CanSandstormStepDrive = false;

    m_OI.ClearButtonBuffer();
}

void Robot::TeleopPeriodic() {
    // No control code goes here.  Put control code for testing in a new
    // JoystickDemo method, or control code for competition in
    // CompetitionJoystickInput().
    //
    // TestPeriodic() will only invoke CompetitionJoystickInput(), and we can
    // swap out a JoystickDemo method for testing, but that modification to
    // TeleopPeriodic() had better not be committed to the develop branch, or so
    // help me, more words will ensue.
    //
    // (╯°Д°）╯︵┻━┻
    
    CompetitionJoystickInput();
    m_Bling.RunBling();
}

void Robot::TestPeriodic() {}

void Robot::CompetitionJoystickInput() {
    // DRIVER CONTROLS
    if (m_OI.GetDriverJoystick().GetBButton() && m_CanSandstormStepDrive) {
        std::cout << "Comp Joy Input: Driver: B Button Down" << std::endl;
        m_DriveSandstormStepWithCargo->Start();
        m_CanSandstormStepDrive = false;
    }

    // OPERATOR CONTROLS
    OperatorHID& console = m_OI.GetOperatorConsole();
    // Change rotation of cargo intake
    if (console.GetIntakeRotateToPickupPressed()) {
        std::cout << "Comp Joy Input: Console: Floor Cargo Pickup Pressed" << std::endl;
        m_TakeCargoFromFloor->Start();
        m_Bling.SetBling(m_Bling.CargoIntakePattern);
    } else if (console.GetIntakeRotateToRocketPressed()) {
        std::cout << "Comp Joy Input: Console: Rotate Rocket Pressed" << std::endl;
        m_RotateCargoForLevelOneRocket->Start();
        m_Bling.SetBling(m_Bling.IntakeRotateRocket);
    } else if (console.GetIntakeRotateToCargoShipPressed()) {
        std::cout << "Comp Joy Input: Console: Rotate Cargoship Pressed" << std::endl;
        m_RotateCargoForCargoShip->Start();
        m_Bling.SetBling(m_Bling.IntakeRotateShip);
    }

    // action command buttons, stuff happens
    if (console.GetCargoShootRocketOnePressed()) {
        std::cout << "Comp Joy Input: Console: Cargo Close Shot Pressed" << std::endl;
        m_ShootCargoForLevelOneRocket->Start();
        m_Bling.SetBling(m_Bling.CargoShootRocketPattern);
    } else if (console.GetCargoShootCargoShipPressed()) {
        std::cout << "Comp Joy Input: Console: Cargo High Shot Pressed" << std::endl;
        m_ShootCargoForCargoShip->Start();
        m_Bling.SetBling(m_Bling.CargoShootShipPattern);
    } else if (console.GetCargoIntakeCargoPressed()) {
        std::cout << "Comp Joy Input: Console: Cargo Intake Pressed" << std::endl;
        if (GetCargoIntake().IsRollerRunning()) {
            m_StopCargoRoller->Start();
        } else {
            m_TakeCargo->Start();
        }
        m_Bling.SetBling(m_Bling.CargoIntakePattern);
    }

#if HATCH_CONTROL_TYPE==0
    // Hatch controls for Compeition 2 Tippecanoe.
    // 2 vertical black buttons.
    //
    // Top button     - RAISE + GRAB hatch.
    // Bottom button  - LOWER + RELEASE hatch.

    if (console.GetHatchGrabPressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Grab Pressed" << std::endl;
        m_RaiseHatch
          ->Until([]() { return Robot::m_OI.GetOperatorConsole().GetHatchGrabReleased(); })
          ->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    } else if (console.GetHatchReleasePressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Release Pressed" << std::endl;
        m_LowerHatch
          ->Until([]() { return Robot::m_OI.GetOperatorConsole().GetHatchReleaseReleased(); })
          ->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    }

    // Piston controls for Competition 2 Tippecanoe.
    // Top 2 of 3 vertical buttons along right edge of console.
    //
    // Top button    - RETRACT piston.
    // Middle button - EXTEND piston.

    if (console.GetHatchTopPositionPressed()) {
        std::cout << "Comp Joy Input: Console: Piston Extend Pressed" << std::endl;
        GetCreeperClimb().PistonExtend();
    } else if (console.GetHatchTopPositionReleased()) {
        std::cout << "Comp Joy Input: Console: Piston Extend Released" << std::endl;
        GetCreeperClimb().PistonHold();
    } else if (console.GetHatchMidPositionPressed()) {
        std::cout << "Comp Joy Input: Console: Piston Retract Pressed" << std::endl;
        GetCreeperClimb().PistonRetract();
    } else if (console.GetHatchMidPositionReleased()) {
        std::cout << "Comp Joy Input: Console: Piston Retract Released" << std::endl;
        GetCreeperClimb().PistonHold();
    }

#elif HATCH_CONTROL_TYPE==1

    // Hatch controls for Competition 3 Kokomo.
    // 3 buttons on right edge of console.
    //
    // Top button     - GRAB hatch    + TOP position + HOLD pid.
    // Middle button  - GRAB hatch    + MID position + HOLD pid.
    // Lower button   - RELEASE hatch + rotate DOWN  + no pid.

    if (console.GetHatchTopPositionPressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Top Position Pressed" << std::endl;
        m_GrabHatchFromLoadingStation->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    } else if (console.GetHatchMidPositionPressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Mid Position Pressed" << std::endl;
        m_ReadyHatch->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    } else if (console.GetHatchLowerPressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Lower Pressed" << std::endl;
        m_LowerHatch
          ->Until([]() { return Robot::m_OI.GetOperatorConsole().GetHatchLowerReleased(); })
          ->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    }

    // Hatch controls for Compeition 2 Tippecanoe.
    // 2 vertical black buttons.
    //
    // Top button     - RAISE + GRAB hatch.
    // Bottom button  - LOWER + RELEASE hatch.

    if (console.GetHatchGrabPressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Grab Pressed" << std::endl;
        m_RaiseHatch
          ->Until([]() { return Robot::m_OI.GetOperatorConsole().GetHatchGrabReleased(); })
          ->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    } else if (console.GetHatchReleasePressed()) {
        std::cout << "Comp Joy Input: Console: Hatch Release Pressed" << std::endl;
        m_LowerHatch
          ->Until([]() { return Robot::m_OI.GetOperatorConsole().GetHatchReleaseReleased(); })
          ->Start();
        m_Bling.SetBling(m_Bling.HatchPattern);
    }

    // Piston controls for Competition 3 Kokomo.
    // 2 vertical black buttons (originally for hatch)
    // Top button    - RETRACT piston.
    // Bottom button - EXTEND piston.

    if (console.GetCreeperExtendPistonPressed()) {
        std::cout << "Comp Joy Input: Console: Piston Extend Pressed" << std::endl;
        GetCreeperClimb().PistonExtend();
    } else if (console.GetCreeperExtendPistonReleased()) {
        std::cout << "Comp Joy Input: Console: Piston Extend Released" << std::endl;
        GetCreeperClimb().PistonHold();
    } else if (console.GetCreeperRetractPistonPressed()) {
        std::cout << "Comp Joy Input: Console: Piston Retract Pressed" << std::endl;
        GetCreeperClimb().PistonRetract();
    } else if (console.GetCreeperRetractPistonReleased()) {
        std::cout << "Comp Joy Input: Console: Piston Retract Released" << std::endl;
        GetCreeperClimb().PistonHold();
    }

#endif

    if (console.GetCargoShootRocketTwoPressed()) {
        std::cout << "Comp Joy Input: Console: Rocket Shot (Level 2)" << std::endl;
        m_ShootCargoForLevelTwoRocket->Start();
        m_Bling.SetBling(m_Bling.CargoShootRocketPattern);
    }

    if (console.GetCreeperReadyArmPressed()) {
        std::cout << "Comp Joy Input: Console: Creeper Ready Arm Pressed" << std::endl;
        m_ReadyCreeperArm->Start();
        m_Bling.SetBling(m_Bling.Climb);
    } else if (console.GetCreeperClimbEnabled()) {
        std::cout << "Comp Joy Input: Console: Climb Sequence Pressed" << std::endl;
        m_ClimbStep->Start();
        m_Bling.SetBling(m_Bling.Climb);
    }

    // manual controls
    if (console.GetCreeperHomeArmPressed()) {
        std::cout << "Comp Joy Input: Console: Retract Climb Arm Pressed" << std::endl;
        Robot::GetCreeperClimb().RotateArmToPosition("home");
    } else if (console.GetCreeperReadyArmPressed()) {
        std::cout << "Comp Joy Input: Console: Retract Climb Piston Pressed" << std::endl;
        Robot::GetCreeperClimb().PistonRetract();
    }

    if (console.GetThrottle() >= 0.75) {
        GetCargoIntake().SetRotateSpeed(-console.GetJoystickY());
    } else if (console.GetThrottle() <= -0.75) {
        m_ClimbStep->Cancel();
        GetCreeperClimb().SetArmRotateSpeed(console.GetJoystickY());
    }

    if (console.IsCreeperCrawlForwardDown()) {
        std::cout << "Comp Joy Input: Console: Creeper Crawl Pressed" << std::endl;
        Robot::GetCreeperClimb().SetArmWheels(true);
    } else if (console.GetCreeperCrawlForwardReleased()) {
        std::cout << "Comp Joy Input: Console: Creeper Crawl Released" << std::endl;
        Robot::GetCreeperClimb().StopArmWheels();
    }

    // if (console.GetBoard2().GetButtonPressed(9)) {
    //     std::cout << "Comp Joy Input: Console: Piston Extend Pressed" << std::endl;
    //     GetCreeperClimb().PistonExtend();
    // } else if (console.GetBoard2().GetButtonReleased(9)) {
    //     std::cout << "Comp Joy Input: Console: Piston Extend Released" << std::endl;
    //     GetCreeperClimb().PistonHold();
    // } else if (console.GetBoard2().GetButtonPressed(10)) {
    //     std::cout << "Comp Joy Input: Console: Piston Retract Pressed" << std::endl;
    //     GetCreeperClimb().PistonRetract();
    // } else if (console.GetBoard2().GetButtonReleased(10)) {
    //     std::cout << "Comp Joy Input: Console: Piston Retract Released" << std::endl;
    //     GetCreeperClimb().PistonHold();
    // }
}

void Robot::ButtonBoardDemo() {
    if (m_OI.GetOperatorConsole().GetCreeperReadyArmPressed()) {
        m_ReadyCreeperArm->Start();
    } else if (m_OI.GetOperatorConsole().GetCreeperClimbEnabled()) {
        m_ClimbStep->Start();
    } else if (m_OI.GetOperatorConsole().GetCreeperHomeArmPressed()) {
        Robot::GetCreeperClimb().RotateArmToPosition("home");
    } else if (m_OI.GetOperatorConsole().GetCreeperRetractPistonPressed()) {
        Robot::GetCreeperClimb().PistonRetract();
    }
}

void Robot::JoystickDemoCargo() {
    frc::XboxController& driver = m_OI.GetDriverJoystick();

    // X + trigger = rotate cargo intake.
    // Y = rotate cargo intake to cargo ship.
    // A = shoot cargo at cargo ship.
    // B = take cargo from floor.
    // R Bumper = take cargo from loading station.

    if (driver.GetXButton()) {
        double leftTrigger =
            driver.GetTriggerAxis(frc::XboxController::kLeftHand);
        double rightTrigger =
            driver.GetTriggerAxis(frc::XboxController::kRightHand);

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

void Robot::JoystickDemoCargoEjector() {
    static double value = 0.3;

    frc::XboxController& driver = m_OI.GetDriverJoystick();

    double leftTrigger =
        driver.GetTriggerAxis(frc::XboxController::kLeftHand);
    double rightTrigger =
        driver.GetTriggerAxis(frc::XboxController::kRightHand);

    if (0.1 < leftTrigger) {
        value = util::clamp(value - 0.001 * leftTrigger, 0.0, 0.7);
    } else if (0.1 < rightTrigger) {
        value = util::clamp(value + 0.001 * rightTrigger, 0.0, 0.7);
    }

    std::cout << "Kick setpoint: " << value << std::endl;
    GetCargoIntake().SetEjector(value);
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
        double leftTrigger =
            driver.GetTriggerAxis(frc::XboxController::kLeftHand);
        double rightTrigger =
            driver.GetTriggerAxis(frc::XboxController::kRightHand);

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
        Robot::GetCreeperClimb().SetArmWheels(true);
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
            GetHatchMechanism().SetRotateSpeed(leftTrigger * 0.5);
        } else if (0.01 < rightTrigger) {
            GetHatchMechanism().SetRotateSpeed(rightTrigger * -0.5);
        } else {
            GetHatchMechanism().SetRotateSpeed(0.0);
        }
    } else if (driver.GetXButtonReleased()) {
        GetHatchMechanism().SetRotateSpeed(0.0);
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
        case 0:  // Up
            if (driver.GetYButtonPressed()) {
                top = util::clamp(top + 0.01, 0.2, 0.8);
            } else if (driver.GetXButtonPressed()) {
                top = util::clamp(top - 0.01, 0.2, 0.8);
            }
            GetCargoIntake().SetTopHookPosition(top);
            break;

        case 180:  // Down
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

void Robot::PrintVersionFile() {
    std::ifstream versionFile("/home/lvuser/deploy/version.txt");

    if (versionFile.is_open()) {
        std::cout << versionFile.rdbuf();
    }
}

#ifndef RUNNING_FRC_TESTS
int main() {
    return frc::StartRobot<Robot>();
}
#endif
