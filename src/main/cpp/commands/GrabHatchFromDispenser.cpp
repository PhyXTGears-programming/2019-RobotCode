#include "commands/GrabHatchFromDispenser.h"
#include "Robot.h"

#include <iostream>
using std::cout;
using std::endl;

/* GOAL:
 *
 * Rotate cargo-intake grabbers into grabbing position.
 *
 *
 * Command start:
 * - Rotate cargo-intake grabbers into grabbing position and hold there.
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

GrabHatchFromDispenser::GrabHatchFromDispenser() {
    Requires(&Robot::m_CargoIntake);
}

void GrabHatchFromDispenser::Initialize() {}

void GrabHatchFromDispenser::Execute() {
    Robot::m_CargoIntake.GripHatchBottom();
    Robot::m_CargoIntake.GripHatchTop();
}

bool GrabHatchFromDispenser::IsFinished() {
    return true;
}

void GrabHatchFromDispenser::End() {}

void GrabHatchFromDispenser::Interrupted() {}
