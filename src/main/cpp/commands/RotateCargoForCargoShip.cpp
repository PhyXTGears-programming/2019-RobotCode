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
    Requires(&Robot::m_CargoIntake);
}

void RotateCargoForCargoShip::Initialize() {
    Robot::m_CargoIntake.RotateToPosition("cargo-ship-shoot-angle");
}

void RotateCargoForCargoShip::Execute() {}

bool RotateCargoForCargoShip::IsFinished() {
    return Robot::m_CargoIntake.IsRotationDone();
}

void RotateCargoForCargoShip::End() {
    // Make sure the motors stop moving when they aren't being controlled.
    Robot::m_CargoIntake.StopRotation();
}

void RotateCargoForCargoShip::Interrupted() {
    Robot::m_CargoIntake.StopRotation();
}
