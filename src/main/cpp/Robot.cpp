#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <wpi/json.h>
#include <frc/Timer.h>

enum class AirState {
    HOME,
    LIFTING,
    STOPPED,
    LOCKED,
    RETRACTING,
};

class StopWatch {
    double mStartTime;
    public:
    StopWatch() {
        Reset();
    }

    void Reset() { mStartTime = frc::Timer::GetFPGATimestamp(); }

    double Elapsed() { return frc::Timer::GetFPGATimestamp() - mStartTime; }
};

class Delay {
    double mPeriod;
    StopWatch mWatch;
    public:
    Delay(double period) : mPeriod(period) {}
    bool IsDone() { return mPeriod <= mWatch.Elapsed(); }
    void Reset() { mWatch.Reset(); }
};

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
    frc::SmartDashboard::PutNumber("intake: angle", GetCargoIntake().GetIntakeRotation());
    frc::SmartDashboard::PutNumber("climb arm: angle", GetCreeperClimb().GetCurrentArmAngle());
    frc::SmartDashboard::PutNumber("climb arm: pid error", GetCreeperClimb().GetArmPID().GetError());
    frc::SmartDashboard::PutNumber("climb arm: pid output", GetCreeperClimb().GetArmPID().Get());
    frc::SmartDashboard::PutBoolean("climb cyl: limit switch", GetCreeperClimb().GetSolenoidSwitch());
}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
    m_DriveTrain.RunReset();
}

void Robot::TeleopPeriodic() {

    frc::XboxController& driver = m_OI.GetDriverJoystick();

    // Hold A during manual control.  Release A to stop manual control.
    //
    //  1.  Press Left Trigger to rotate arm toward floor.
    //  2.  Press Right Trigger to rotate arm toward home position.
    //
    // PID Control:
    //
    //  1.  Press X to rotate arm to 90 deg (vertical).
    //  2.  Press B to rotate to preparation angle (19.5" above floor).
    //  3.  Press Y to abort and disable PID beFORE SOMETHING BREAKS, ROBBY!!!! :P

    if (driver.GetAButton()) {
        double leftTrigger = driver.GetTriggerAxis(frc::XboxController::kLeftHand);
        double rightTrigger = driver.GetTriggerAxis(frc::XboxController::kRightHand);
        if (0.01 < leftTrigger) {
            m_CreeperClimb->SetRotateSpeed(leftTrigger * -1);
        } else if (0.01 < rightTrigger) {
            m_CreeperClimb->SetRotateSpeed(rightTrigger * 1);
        } else {
            m_CreeperClimb->SetRotateSpeed(0.0);
        }
    } else if (driver.GetAButtonReleased()) {
        m_CreeperClimb->SetRotateSpeed(0.0);
    } else {
        if (driver.GetXButtonPressed()) {
            m_CreeperClimb->SetArmAngle(90.0);
        }
        if (driver.GetBButtonPressed()) {
            m_CreeperClimb->SetArmAngle("arm-prep-angle");
        }
        if (driver.GetYButtonPressed()) {
            m_CreeperClimb->StopArmRotation();
        }
        if (m_CreeperClimb->IsArmRotationDone()) {
            m_CreeperClimb->StopArmRotation();
        }
    }


    static AirState creeperState = AirState::HOME;
    static Delay retractDelay{1.5};
    static StopWatch liftTime;

    // Retract cylinder.  Robot down.
    // Extend cyclinder.  Robot up.
    switch (creeperState) {
        case AirState::HOME:
            if (driver.GetBumperPressed(frc::XboxController::kRightHand)) {
                //std::cout << "Right bumper pressed       -> LIFTING" << std::endl;
                GetCreeperClimb().SetSolenoidAscend(true);
                GetCreeperClimb().SetSolenoidDescend(false);
                liftTime.Reset();
                creeperState = AirState::LIFTING;
            }

            break;

        case AirState::LIFTING:
            if (GetCreeperClimb().GetSolenoidSwitch()) {
                //std::cout
                //    << "Lifting reached max height -> STOPPED  "
                //    << liftTime.Elapsed()
                //    << " seconds"
                //    << std::endl;
                std::cout << liftTime.Elapsed() << " seconds" << std::endl;
                GetCreeperClimb().SetSolenoidDescend(true);

                creeperState = AirState::LOCKED;
            }

            if (driver.GetBumperPressed(frc::XboxController::kLeftHand)) {
                //std::cout << "Left bumper pressed, halting lifting" << std::endl;
                GetCreeperClimb().SetSolenoidAscend(false);
                GetCreeperClimb().SetSolenoidDescend(false);

                creeperState = AirState::STOPPED;
            }

            break;

        case AirState::STOPPED:
            if (driver.GetBumperPressed(frc::XboxController::kLeftHand)) {
                //std::cout << "Left bumper pressed, retracting, wait 2 seconds" << std::endl;
                GetCreeperClimb().SetSolenoidAscend(false);
                GetCreeperClimb().SetSolenoidDescend(true);

                retractDelay.Reset();

                creeperState = AirState::RETRACTING;
            }

            break;

        case AirState::LOCKED:
            if (driver.GetBumperPressed(frc::XboxController::kLeftHand)) {
                //std::cout << "Left bumper pressed, retracting, wait 2 seconds" << std::endl;
                GetCreeperClimb().SetSolenoidAscend(false);
                GetCreeperClimb().SetSolenoidDescend(true);

                retractDelay.Reset();

                creeperState = AirState::RETRACTING;
            }
            break;

        case AirState::RETRACTING:
            if (retractDelay.IsDone()) {
                //std::cout << "Time elapsed, retracting complete" << std::endl;
                GetCreeperClimb().SetSolenoidAscend(false);
                GetCreeperClimb().SetSolenoidDescend(false);

                creeperState = AirState::HOME;
            }

            break;
    }

    frc::Scheduler::GetInstance()->Run();
    
    m_DriveTrain.Drive(0, 0);
}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
