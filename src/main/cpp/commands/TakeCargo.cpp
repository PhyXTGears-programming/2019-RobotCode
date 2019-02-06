/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/TakeCargo.h"
#include "Robot.h"
#include "RobotMap.h"

TakeCargo::TakeCargo() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::m_CargoIntake);
}

void TakeCargo::Initialize() {
}

void TakeCargo::Execute() {

#ifndef PROTOBOT
    Robot::m_CargoIntake.TurnOnIntakeRoller();
#endif

}

bool TakeCargo::IsFinished() {
    return Robot::m_CargoIntake.HasCargo();
}

// Make sure the motors stop moving when they aren't being controlled.
void TakeCargo::End() { 

#ifndef PROTOBOT
    Robot::m_CargoIntake.TurnOffIntakeRoller();
#endif

}

void TakeCargo::Interrupted() {}
