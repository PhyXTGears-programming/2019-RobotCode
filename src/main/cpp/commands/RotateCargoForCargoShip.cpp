#include "commands/RotateCargoForCargoShip.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Position cargo-intake to shoot and score on cargo ship.
 * 
 *
 * Command start:
 *  - Rotate cargo-intake to cargo ship shooting position.
 * 
 * Command finished when:
 *  - Position sensor indicates cargo-intake is at desired rotation.
 * 
 * Command end:
 *  - Stop/disable cargo-intake rotation motor.
 * 
 * Follow-up options:
 *  - None.
 */

RotateCargoForCargoShip::RotateCargoForCargoShip() {
    Requires(&Robot::GetCargoIntake());
}

void RotateCargoForCargoShip::Initialize() {
    Robot::GetCargoIntake().RotateToPosition("cargo-ship-shoot");
}

void RotateCargoForCargoShip::Execute() {}

bool RotateCargoForCargoShip::IsFinished() {
    return Robot::GetCargoIntake().IsRotationDone();
}

void RotateCargoForCargoShip::End() {
    // Make sure the motors stop moving when they aren't being controlled.
    Robot::GetCargoIntake().StopRotation();
}

void RotateCargoForCargoShip::Interrupted() {
    Robot::GetCargoIntake().StopRotation();
}
