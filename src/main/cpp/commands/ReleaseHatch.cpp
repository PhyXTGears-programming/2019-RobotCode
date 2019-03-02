#include "commands/ReleaseHatch.h"
#include "Robot.h"

/* GOAL:
 *
 * Rotate cargo-intake grabbers into neutral position.
 *
 *
 * Command start:
 * - Rotate cargo-intake grabbers into neutral position and hold there.
 *
 * Command finished when:
 *  - Servos can move independently to the desired location without us checking.
 *  - Always finishes on its own. IsFinished() is always true.
 *
 * Command end:
 *  - Stop/disable cargo-intake grabbers.
 *
 * Follow-up options:
 *  - None.
 */

ReleaseHatch::ReleaseHatch() {
    Requires(&Robot::GetCargoIntake());
}

void ReleaseHatch::Initialize() {}

void ReleaseHatch::Execute() {
    CargoIntake& intake = Robot::GetCargoIntake();

    intake.SetBottomHookPosition("kick-out");
    intake.SetTopHookPosition("kick-out");
}

bool ReleaseHatch::IsFinished() { return true; }

void ReleaseHatch::End() {}

void ReleaseHatch::Interrupted() {}
