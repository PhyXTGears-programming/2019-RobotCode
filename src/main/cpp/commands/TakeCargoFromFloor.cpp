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
    Requires(&Robot::GetCargoIntake());
}

void TakeCargoFromFloor::Initialize() {
    Robot::GetCargoIntake().RetractEjector();
    Robot::GetCargoIntake().SetTopHookPosition("docked");
    Robot::GetCargoIntake().SetBottomHookPosition("docked");
    hadCargo = Robot::GetCargoIntake().HasCargo();
    sensorCount = 0;
}

void TakeCargoFromFloor::Execute() {
    if (IsFinished()) {
        // Prevent command from running rollers if cargo already captured.
        return;
    }

    CargoIntake& intake = Robot::GetCargoIntake();

    intake.RotateToPosition("cargo-floor-pickup");

#ifndef PROTOBOT
    intake.SetRollerSpeed("intake");
#endif

}

bool TakeCargoFromFloor::IsFinished() {
    if (Robot::GetCargoIntake().HasCargo()) {
        sensorCount++;
    } else {
        sensorCount = 0;
    }
    return sensorCount >= 5;
}

// Make sure the motors stop moving when they aren't being controlled.
void TakeCargoFromFloor::End() { 
    // Leave cargo intake arm under PID control to hold position.
#ifndef PROTOBOT
    Robot::GetCargoIntake().StopRoller();

    // if (!hadCargo) {
    //     Robot::GetCargoIntake().RotateToPosition("cargo-ship-shoot");
    // }
#endif
}

void TakeCargoFromFloor::Interrupted() {
    Robot::GetCargoIntake().StopRoller();
}
