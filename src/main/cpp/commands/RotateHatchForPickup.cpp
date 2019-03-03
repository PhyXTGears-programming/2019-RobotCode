#include "commands/RotateHatchForPickup.h"

/* GOAL:
 *
 * Lift hatch from floor with spatulas and let is drop into place, then grip
 * with top hook.
 */

RotateHatchForPickup::RotateHatchForPickup() {
    // Use Requires() here to declare subsystem dependencies
    // eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void RotateHatchForPickup::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void RotateHatchForPickup::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool RotateHatchForPickup::IsFinished() { return false; }

// Called once after isFinished returns true
void RotateHatchForPickup::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void RotateHatchForPickup::Interrupted() {}
