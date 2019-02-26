#include "commands/TakeCargoFromFloor.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Capture cargo from floor.
 * 
 *
 * Command start:
 *  - Rotate cargo intake to floor-pickup position (approx 120 deg from home
 *    position).
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

TakeCargoFromFloor::TakeCargoFromFloor() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::GetCargoIntake());
}

void TakeCargoFromFloor::Initialize() {
}

void TakeCargoFromFloor::Execute() {

#ifndef PROTOBOT
    Robot::GetCargoIntake().SetRollerSpeed("intake");
#endif

}

bool TakeCargoFromFloor::IsFinished() {
    return Robot::GetCargoIntake().HasCargo();
}

// Make sure the motors stop moving when they aren't being controlled.
void TakeCargoFromFloor::End() { 

#ifndef PROTOBOT
    Robot::GetCargoIntake().StopRoller();
#endif

}

void TakeCargoFromFloor::Interrupted() {
    Robot::GetCargoIntake().StopRoller();
}
