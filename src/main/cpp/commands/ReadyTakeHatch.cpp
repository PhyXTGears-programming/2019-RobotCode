#include "commands/ReadyTakeHatch.h"
#include "Robot.h"

#include <iostream>
using std::cout;
using std::endl;

/* GOAL:
 *
 * Lift/lower hatch mechanism to bottom position and release hatch grip.
 *
 * Command start:
 * - Lift/lower hatch to bottom position with PID.
 * - Release hatch grip.
 *
 * Command execute:
 *
 * Command finished when:
 *  - Arm reaches bottom position.
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

ReadyTakeHatch::ReadyTakeHatch() {
    Requires(&Robot::GetHatchMechanism());
}

void ReadyTakeHatch::Initialize() {
    HatchMechanism &hatch = Robot::GetHatchMechanism();

    // Start moving to bottom position.
    hatch.RotateToBottomPosition();
    hatch.ReleaseHatch();
}

void ReadyTakeHatch::Execute() {}

bool ReadyTakeHatch::IsFinished() {
    return Robot::GetHatchMechanism().IsArmRotationDone();
}

void ReadyTakeHatch::End() {}

void ReadyTakeHatch::Interrupted() {
    Robot::GetHatchMechanism().StopRotation();
}
