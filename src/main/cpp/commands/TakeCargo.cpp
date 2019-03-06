#include "commands/TakeCargo.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Capture cargo from anywhere.  Sometimes the ball doesn't shoot well and
 * we'd like to scoop it up quick if the cargo bounces back.
 * 
 *
 * Command start:
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
 */

TakeCargo::TakeCargo() {
    Requires(&Robot::GetCargoIntake());
}

void TakeCargo::Initialize() {
    Robot::GetCargoIntake().RetractEjector();
    Robot::GetCargoIntake().SetTopHookPosition("docked");
    Robot::GetCargoIntake().SetBottomHookPosition("docked");
}

void TakeCargo::Execute() {
    if (IsFinished()) {
        // Prevent command from running rollers if cargo already captured.
        return;
    }

    CargoIntake& intake = Robot::GetCargoIntake();

#ifndef PROTOBOT
    intake.SetRollerSpeed("intake");
#endif

}

bool TakeCargo::IsFinished() {
    return Robot::GetCargoIntake().HasCargo();
}

// Make sure the motors stop moving when they aren't being controlled.
void TakeCargo::End() { 
    // Leave cargo intake arm under PID control to hold position.
#ifndef PROTOBOT
    Robot::GetCargoIntake().StopRoller();
#endif

}

void TakeCargo::Interrupted() {
    Robot::GetCargoIntake().StopRoller();
}
