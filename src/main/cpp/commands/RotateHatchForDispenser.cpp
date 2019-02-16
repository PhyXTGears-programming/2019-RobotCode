#include "commands/RotateHatchForDispenser.h"

/* GOAL:
 *
 * Rotate cargo-intake into position for hatch pickup from dispenser.
 *
 *
 * Command start:
 * - Rotate cargo-intake until at 90 degrees and hold there.
 * - Adjust top and bottom gripper to open position.
 *
 * Command finished when:
 *  - Wait until cargo-intake is at 90 degrees.
 *
 * Command end:
 *  - Stop/disable cargo-intake rotation motor.
 *
 * Follow-up options:
 *  - None.
 */

RotateHatchForDispenser::RotateHatchForDispenser() {
    Requires(&Robot::GetCargoIntake());
}

// Called just before this Command runs the first time
void RotateHatchForDispenser::Initialize() {
    Robot::GetCargoIntake().ReleaseHatchBottom();
    Robot::GetCargoIntake().ReleaseHatchTop();
    Robot::GetCargoIntake().RotateToPosition("hatch-feeder-pickup-angle");
}

// Called repeatedly when this Command is scheduled to run
void RotateHatchForDispenser::Execute() {
    
}

// Make this return true when this Command no longer needs to run execute()
bool RotateHatchForDispenser::IsFinished() {
    return Robot::GetCargoIntake().IsRotationDone();
}

// Called once after isFinished returns true
void RotateHatchForDispenser::End() {
    Robot::GetCargoIntake().StopRotation();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void RotateHatchForDispenser::Interrupted() {}
