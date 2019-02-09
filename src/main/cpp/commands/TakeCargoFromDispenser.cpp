#include "commands/TakeCargoFromDispenser.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Capture cargo from dispenser.
 * 
 *
 * Command start:
 *  - Rotate cargo intake to dispenser position.
 *  - Retract cargo kick completely (use time delay of ? seconds) before
 *    engaging rollers. It should already be retracted, but expect the unexpected.
 *  - Turn on rollers for intake.
 * 
 * Command finished when:
 *  - Cargo detected.  
 * 
 * Command end:
 *  - Stop rollers.
 *  - Retract cargo kick.
 * 
 * Follow-up options:
 *  - Schedule next command to position cargo intake for scoring. Use
 *    CommandGroup.
 */

TakeCargoFromDispenser::TakeCargoFromDispenser() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::m_CargoIntake);
}

void TakeCargoFromDispenser::Initialize() {
}

void TakeCargoFromDispenser::Execute() {

#ifndef PROTOBOT
    Robot::m_CargoIntake.TurnOnIntakeRoller();
#endif

}

bool TakeCargoFromDispenser::IsFinished() {
    return Robot::m_CargoIntake.HasCargo();
}

// Make sure the motors stop moving when they aren't being controlled.
void TakeCargoFromDispenser::End() { 

#ifndef PROTOBOT
    Robot::m_CargoIntake.TurnOffIntakeRoller();
#endif

}

void TakeCargoFromDispenser::Interrupted() {}
