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
    Requires(&Robot::GetCargoIntake());
}

void TakeCargoFromDispenser::Initialize() {
}

void TakeCargoFromDispenser::Execute() {

#ifndef PROTOBOT
    Robot::GetCargoIntake().SetRollerSpeed("intake");
#endif

}

bool TakeCargoFromDispenser::IsFinished() {
    return Robot::GetCargoIntake().HasCargo();
}

// Make sure the motors stop moving when they aren't being controlled.
void TakeCargoFromDispenser::End() { 

#ifndef PROTOBOT
    Robot::GetCargoIntake().StopRoller();
#endif

}

void TakeCargoFromDispenser::Interrupted() {
    Robot::GetCargoIntake().StopRoller();
}
