#include "commands/RotateCargoForLevelOneRocket.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Position cargo-intake to shoot and score on level 1 rocket.
 * 
 *
 * Command start:
 *  - Rotate cargo-intake to level 1 rocket position.
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

RotateCargoForLevelOneRocket::RotateCargoForLevelOneRocket() {
    Requires(&Robot::GetCargoIntake());
}

void RotateCargoForLevelOneRocket::Initialize() {
    Robot::GetCargoIntake().RotateToPosition("rocket-1-shoot");
}

void RotateCargoForLevelOneRocket::Execute() {}

bool RotateCargoForLevelOneRocket::IsFinished() {
    return Robot::GetCargoIntake().IsRotationDone();
}

void RotateCargoForLevelOneRocket::End() {
}

void RotateCargoForLevelOneRocket::Interrupted() {
    Robot::GetCargoIntake().StopRotation();
}
