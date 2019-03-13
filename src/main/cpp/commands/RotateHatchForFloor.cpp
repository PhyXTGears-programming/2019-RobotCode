#include "commands/RotateHatchForFloor.h"
#include "Robot.h"

/* GOAL:
 *
 * Rotate cargo-intake into position for hatch pickup from floor.
 *
 *
 * Command start:
 * - Rotate cargo-intake until at -90 degrees and hold there.
 *
 * Command finished when:
 *  - Wait until cargo-intake is at -90 degrees.
 *
 * Command end:
 *  - Stop/disable cargo-intake rotation motor.
 *
 * Follow-up options:
 *  - None.
 */

RotateHatchForFloor::RotateHatchForFloor() {
    Requires(&Robot::GetCargoIntake());
}

// Called just before this Command runs the first time
void RotateHatchForFloor::Initialize() {
    Robot::GetCargoIntake().RotateToPosition("hatch-floor-pickup");
}

// Called repeatedly when this Command is scheduled to run
void RotateHatchForFloor::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool RotateHatchForFloor::IsFinished() {
    return Robot::GetCargoIntake().IsRotationDone();
}

// Called once after isFinished returns true
void RotateHatchForFloor::End() {
    // Make sure the motors stop moving when they aren't being controlled.
    Robot::GetCargoIntake().StopRotation();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void RotateHatchForFloor::Interrupted() {}
