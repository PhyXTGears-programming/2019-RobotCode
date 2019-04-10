#include "commands/ReadyHatch.h"
#include "Robot.h"

#include <iostream>
using std::cout;
using std::endl;

/* GOAL:
 *
 * Lift/lower hatch mechanism to mid position and release hatch grip.
 *
 * Command start:
 * - Lift/lower hatch to mid position with PID.
 * - Release hatch grip.
 *
 * Command execute:
 *
 * Command finished when:
 *  - Arm reaches mid position.
 *
 * Command end:
 *  - Leave PID enabled to hold position.  Arm lifts/lowers to setpoint with
 *    hatch and weight of hatch should not pull arm down.
 *
 * Command interrupted:
 *  - Stop rotation.  Command was probably interrupted for a reason.
 *
 * Follow-up options:
 *  - None.
 */

ReadyHatch::ReadyHatch() {
    Requires(&Robot::GetHatchMechanism());
}

void ReadyHatch::Initialize() {
    HatchMechanism &hatch = Robot::GetHatchMechanism();

    // Start moving to mid position.
    hatch.RotateToMidPosition();
    hatch.ReleaseHatch();
}

void ReadyHatch::Execute() {}

bool ReadyHatch::IsFinished() {
    return Robot::GetHatchMechanism().IsArmRotationDone();
}

void ReadyHatch::End() {}

void ReadyHatch::Interrupted() {
    Robot::GetHatchMechanism().StopRotation();
}
