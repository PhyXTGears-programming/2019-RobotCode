#include "commands/RotateCargoForLevelTwoRocket.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Position cargo-intake to shoot and score on level 2 rocket.
 * 
 *
 * Command start:
 *  - Rotate cargo-intake to level 2 rocket position.
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

RotateCargoForLevelTwoRocket::RotateCargoForLevelTwoRocket() {
    Requires(&Robot::GetCargoIntake());
}

void RotateCargoForLevelTwoRocket::Initialize() {}

void RotateCargoForLevelTwoRocket::Execute() {}

bool RotateCargoForLevelTwoRocket::IsFinished() {}

void RotateCargoForLevelTwoRocket::End() {
    // Make sure the motors stop moving when they aren't being controlled.
}

void RotateCargoForLevelTwoRocket::Interrupted() {}
