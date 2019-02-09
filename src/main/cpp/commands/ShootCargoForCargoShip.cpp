#include "commands/ShootCargoForCargoShip.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Eject cargo from cargo-intake to shoot and score on cargo ship.
 * 
 *
 * Command start:
 *  - Turn on rollers. Direction to eject cargo. Speed configured via
 *    Shuffleboard.
 *  - Wait ? seconds for rollers to achieve full speed. Time configured via
 *    Shuffleboard.
 *  - Extend cargo kick.  Cargo hits rollers and ejects.
 * 
 * Command finished when:
 *  - Wait ? seconds after extending cargo kick. Time configured via
 *    Shuffleboard.
 * 
 * Command end:
 *  - Stop/disable cargo-intake rollers.
 *  - Retract cargo kick.
 * 
 * Follow-up options:
 *  - None.
 */

ShootCargoForCargoShip::ShootCargoForCargoShip() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::m_CargoIntake);
}

void ShootCargoForCargoShip::Initialize() {}

void ShootCargoForCargoShip::Execute() {}

bool ShootCargoForCargoShip::IsFinished() {}

void ShootCargoForCargoShip::End() {
    // Make sure the motors stop moving when they aren't being controlled.
}

void ShootCargoForCargoShip::Interrupted() {}
