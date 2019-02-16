#include "commands/GrabHatchFromFloor.h"
#include "Robot.h"

/* GOAL:
 *
 * Grab hatch from floor. Secure hatch so robot can pick it up.
 *
 *
 * Command start:
 * - Rotate cargo-intake to ? degrees (adjust ?'s via config). Hatch is in
 *   cargo-intake spatula and will slide down from gravity into cargo-intake.
 * - Wait ? seconds for hatch to slide down. (adjust via config).
 * - Close top grip to capture hatch before it falls out.
 *
 * Command finished when:
 *  - Wait ? seconds until top grip is in closed position.
 *
 * Command end:
 *  - Stop/disable cargo-intake rotation motor.
 *
 * Follow-up options:
 *  - None.
 */


GrabHatchFromFloor::GrabHatchFromFloor() {
  Requires(&Robot::m_CargoIntake);
}

// Called just before this Command runs the first time
void GrabHatchFromFloor::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void GrabHatchFromFloor::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool GrabHatchFromFloor::IsFinished() { return false; }

// Called once after isFinished returns true
void GrabHatchFromFloor::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void GrabHatchFromFloor::Interrupted() {}
