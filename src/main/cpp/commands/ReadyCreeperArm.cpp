#include "commands/ReadyCreeperArm.h"
#include "Robot.h"

/* GOAL:
 *
 * Prepare robot for climbing the platform
 *
 *
 * Command start:
 *  - set creeper arm setpoint to the proper angle dictated by the json config
 *
 * Command finished when:
 *  - the angle of the creeper arm is close enough to the proper angle
 *
 * Command end:
 *  - None.
 *
 * Follow-up options:
 *  - None.
 */

ReadyCreeperArm::ReadyCreeperArm() {
  Requires(&Robot::GetCreeperClimb());
}

void ReadyCreeperArm::Initialize() {}

void ReadyCreeperArm::Execute() {}

bool ReadyCreeperArm::IsFinished() { return false; }

void ReadyCreeperArm::End() {}

void ReadyCreeperArm::Interrupted() {}
