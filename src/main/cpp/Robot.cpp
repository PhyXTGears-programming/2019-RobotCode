#include "Robot.h"
#include "util.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <wpi/StringRef.h>
#include <wpi/json.h>

// Initialize Operator Interface
OI Robot::m_OI;
// Initialize Subsystems
DriveTrain*                     Robot::m_DriveTrain;
CargoIntake*                    Robot::m_CargoIntake;
CreeperClimb*                   Robot::m_CreeperClimb;

// Initialize Commands - Intake
GrabHatchFromDispenser*         Robot::m_GrabHatchFromDispenser;
ReleaseHatch*                   Robot::m_ReleaseHatch;
RotateCargoForCargoShip*        Robot::m_RotateCargoForCargoShip;
RotateCargoForLevelOneRocket*   Robot::m_RotateCargoForLevelOneRocket;
RotateHatchForFloor*            Robot::m_RotateHatchForFloor;
RotateHatchForDispenser*        Robot::m_RotateHatchForDispenser;

ShootCargoForCargoShip*         Robot::m_ShootCargoForCargoShip;
ShootCargoForLevelOneRocket*    Robot::m_ShootCargoForLevelOneRocket;

StopCargoRoller*                Robot::m_StopCargoRoller;
TakeCargo*                      Robot::m_TakeCargo;
TakeCargoFromDispenser*         Robot::m_TakeCargoFromDispenser;
TakeCargoFromFloor*             Robot::m_TakeCargoFromFloor;

// Initialize Commands - Climb
ReadyCreeperArm*                Robot::m_ReadyCreeperArm;
ClimbStep*                      Robot::m_ClimbStep;

// Initialize Commands - Drive
DriveSandstormStepWithCargo*    Robot::m_DriveSandstormStepWithCargo;
DriveSandstormStepWithHatch*    Robot::m_DriveSandstormStepWithHatch;

// Initialize JSON reader
wpi::json                       Robot::m_JsonConfig;

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

    m_CargoIntake = new CargoIntake(m_JsonConfig);
    m_CreeperClimb = new CreeperClimb(m_JsonConfig);
    m_DriveTrain = new DriveTrain(m_JsonConfig);

    // Allocate and initialize commands - Teleop
    m_DriveSandstormStepWithCargo = new DriveSandstormStepWithCargo();
    m_DriveSandstormStepWithHatch = new DriveSandstormStepWithHatch();

    // Allocate and initialize commands - Intake
    m_GrabHatchFromDispenser = new GrabHatchFromDispenser();
    m_ReleaseHatch = new ReleaseHatch();
    m_RotateCargoForCargoShip = new RotateCargoForCargoShip();
    m_RotateCargoForLevelOneRocket = new RotateCargoForLevelOneRocket();
    m_RotateHatchForFloor = new RotateHatchForFloor();
    m_RotateHatchForDispenser = new RotateHatchForDispenser();

    m_ShootCargoForCargoShip = new ShootCargoForCargoShip();
    m_ShootCargoForLevelOneRocket = new ShootCargoForLevelOneRocket();

    m_StopCargoRoller = new StopCargoRoller();
    m_TakeCargo = new TakeCargo();
    m_TakeCargoFromDispenser = new TakeCargoFromDispenser();
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
    frc::SmartDashboard::PutNumber("flight throttle",
                                   m_OI.GetOperatorConsole().GetThrottle());
    frc::SmartDashboard::PutNumber("intake rotation",
                                   GetCargoIntake().GetIntakeRotation());
    frc::SmartDashboard::PutNumber("climb arm rotation",
                                   GetCreeperClimb().GetCurrentArmPosition());
    frc::SmartDashboard::PutNumber("climb stage", m_ClimbStep->GetSegment());
    frc::Scheduler::GetInstance()->Run();
}

void Robot::DisabledInit() {
    m_ClimbStep->Cancel();

    GetCreeperClimb().Disable();
    GetCargoIntake().Disable();

    // Clear pending commands out of scheduler.
    frc::Scheduler::GetInstance()->ResetAll();
}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {
    GetDriveTrain().RunReset();
    GetCreeperClimb().RunReset();
    GetCargoIntake().RunReset();
    m_CanSandstormStepDrive = true;
}

void Robot::AutonomousPeriodic() {
    TeleopPeriodic();
}

void Robot::TeleopInit() {
    m_CanSandstormStepDrive = false;
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
    CompetitionJoystickInput();
}

void Robot::TestPeriodic() {}

void Robot::CompetitionJoystickInput() {
    // DRIVER CONTROLS
    if (m_OI.GetDriverJoystick().GetBButton() && m_CanSandstormStepDrive) {
        m_DriveSandstormStepWithCargo->Start();
        m_CanSandstormStepDrive = false;
    }

    // OPERATOR CONTROLS
    OperatorHID& console = m_OI.GetOperatorConsole();
    // Change rotation of cargo intake
    if (console.GetFloorHatchPickupPressed()) {
        m_RotateHatchForFloor->Start();
    } else if (console.GetFloorCargoPickupPressed()) {
        m_TakeCargoFromFloor->Start();
    } else if (console.GetHatchFeederScorePressed()) {
        m_RotateHatchForDispenser->Start();
    } else if (console.GetRocketShotPressed()) {
        m_RotateCargoForLevelOneRocket->Start();
    } else if (m_OI.GetOperatorConsole().GetCargoShotPressed()) {
        m_RotateCargoForCargoShip->Start();
    } else if (console.GetGoHomePressed()) {
        GetCargoIntake().GoHome();  // CHANGE TO COMMAND
    }

    // action command buttons, stuff happens
    if (console.GetCargoCloseShotPressed()) {
        m_ShootCargoForLevelOneRocket->Start();
    } else if (console.GetCargoHighShotPressed()) {
        m_ShootCargoForCargoShip->Start();
    } else if (console.GetCargoIntakePressed()) {
        if (m_TakeCargo->IsRunning()) {
            m_StopCargoRoller->Start();
        } else {
            m_TakeCargo->Start();
        }
    }

    if (console.IsHatchReleaseDown() && console.IsHatchGrabDown()) {
        // Stop cheesecake hatch if both buttons are down.
        GetCargoIntake().SetHatchRotateSpeed(0.0);
    } else if (console.IsHatchReleaseDown()) {
        GetCargoIntake().SetHatchRotateSpeed(0.5);
    } else if (console.IsHatchGrabDown()) {
        GetCargoIntake().SetHatchRotateSpeed(-0.5);
    } else {
        GetCargoIntake().SetHatchRotateSpeed(0.0);
    }

    if (console.GetCreeperReadyArmPressed()) {
        m_ReadyCreeperArm->Start();
    } else if (console.GetClimbSequencePressed()) {
        m_ClimbStep->Start();
    }

    // manual controls
    if (console.GetRetractArm()) {
        Robot::GetCreeperClimb().RotateArmToPosition("home");
    } else if (console.GetRetractCylinder()) {
        Robot::GetCreeperClimb().PistonRetract();
    }

    if (console.GetThrottle() >= 0.75) {
        GetCargoIntake().SetRotateSpeed(console.GetJoystickY());
    } else if (console.GetThrottle() <= -0.75) {
        GetCreeperClimb().SetArmRotateSpeed(console.GetJoystickY());
    }
}

void Robot::ButtonBoardDemo() {
    if (m_OI.GetOperatorConsole().GetCreeperReadyArmPressed()) {
        m_ReadyCreeperArm->Start();
    } else if (m_OI.GetOperatorConsole().GetClimbSequencePressed()) {
        m_ClimbStep->Start();
    } else if (m_OI.GetOperatorConsole().GetRetractArm()) {
        Robot::GetCreeperClimb().RotateArmToPosition("home");
    } else if (m_OI.GetOperatorConsole().GetRetractCylinder()) {
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

#ifndef RUNNING_FRC_TESTS
int main() {
    return frc::StartRobot<Robot>();
}
#endif
