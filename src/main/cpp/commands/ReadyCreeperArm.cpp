#include "commands/ReadyCreeperArm.h"
#include "Robot.h"

/* GOAL:
 *
 * Prepare the robot for climb.  Operator can activate preparation while
 * driving robot to platform.
 *
 *
 * Command start:
 *  - Return cargo-intake to home position.
 *    - Rotate hatch grippers into closed position.
 *
 * Command finished when:
 *  - When creeper arm is in position (angle puts wheels approx 19.5" above ground).
 *
 * Command end:
 *  - No action.
 *
 * Notes:
 *  - Run HomeCargo command before starting ReadyCreeperArm as cargo-intake may
 *    collide with creeper arm.
 */

ReadyCreeperArm::ReadyCreeperArm() {
    Requires(&Robot::GetCreeperClimb());
}

// Called just before this Command runs the first time
void ReadyCreeperArm::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void ReadyCreeperArm::Execute() {
    Robot::GetCreeperClimb().RotateArmToPosition("arm-ready");
}

// Make this return true when this Command no longer needs to run execute()
bool ReadyCreeperArm::IsFinished() {
    return Robot::GetCreeperClimb().IsArmRotationDone();
}

// Called once after isFinished returns true
void ReadyCreeperArm::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ReadyCreeperArm::Interrupted() {}
