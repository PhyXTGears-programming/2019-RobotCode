#include "commands/ClimbPrep.h"
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
 *  - Run HomeCargo command before starting ClimbPrep as cargo-intake may
 *    collide with creeper arm.
 */

ClimbPrep::ClimbPrep() {
    Requires(&Robot::GetCreeperClimb());
}

// Called just before this Command runs the first time
void ClimbPrep::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void ClimbPrep::Execute() {
    Robot::GetCreeperClimb().SetArmAngle("arm-prep-angle");
}

// Make this return true when this Command no longer needs to run execute()
bool ClimbPrep::IsFinished() {
    return Robot::GetCreeperClimb().IsArmRotationDone();
}

// Called once after isFinished returns true
void ClimbPrep::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ClimbPrep::Interrupted() {}
