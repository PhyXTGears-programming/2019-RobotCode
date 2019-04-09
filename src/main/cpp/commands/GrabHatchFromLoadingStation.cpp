#include "commands/GrabHatchFromLoadingStation.h"
#include "Robot.h"

#include <iostream>
using std::cout;
using std::endl;

/* GOAL:
 *
 * Lift hatch mechanism to top position and grip hatch when arm rises above
 * threshold.
 *
 * Command start:
 * - Lift hatch to top position with PID.
 *
 * Command execute:
 * - When top position exceeds "grip threshold", then grab hatch. This ensures
 *   the hatch is fully seated inside gripper.
 *
 * Command finished when:
 *  - Arm reaches top position.
 *
 * Command end:
 *  - Leave PID enabled to hold position.  Arm lifts to grab hatch and weight
 *    of hatch should not pull arm down.
 *
 * Command interrupted:
 *  - Stop rotation.  Command was probably interrupted for a reason.
 *
 * Follow-up options:
 *  - None.
 */

GrabHatchFromLoadingStation::GrabHatchFromLoadingStation() {
    Requires(&Robot::GetHatchMechanism());
}

void GrabHatchFromLoadingStation::Initialize() {
    // Start moving to top position.
    Robot::GetHatchMechanism().RotateToTopPosition();
    // Assume gripper is open.
    m_IsHatchGrabbed = false;
}

void GrabHatchFromLoadingStation::Execute() {
    HatchMechanism &hatch = Robot::GetHatchMechanism();

    if (!m_IsHatchGrabbed && hatch.IsAboveGripThreshold()) {
        // Only grab once.
        hatch.GrabHatch();
        m_IsHatchGrabbed = true;
    }
}

bool GrabHatchFromLoadingStation::IsFinished() {
    return Robot::GetHatchMechanism().IsArmRotationDone();
}

void GrabHatchFromLoadingStation::End() {}

void GrabHatchFromLoadingStation::Interrupted() {
    Robot::GetHatchMechanism().StopRotation();
}
