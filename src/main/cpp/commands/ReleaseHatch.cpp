#include "commands/ReleaseHatch.h"
#include "Robot.h"

#include <iostream>
using std::cout;
using std::endl;

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
    Requires(&Robot::m_CargoIntake);
}

void ReleaseHatch::Initialize() {}

void ReleaseHatch::Execute() {
    Robot::m_CargoIntake.ReleaseHatchBottom();
    Robot::m_CargoIntake.ReleaseHatchTop();
}

bool ReleaseHatch::IsFinished() { return true; }

void ReleaseHatch::End() {}

void ReleaseHatch::Interrupted() {}
