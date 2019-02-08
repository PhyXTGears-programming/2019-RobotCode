/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/RotateCargoForCargoShip.h"
#include "Robot.h"
#include "RobotMap.h"

/* GOAL:
 *
 * Position cargo-intake to shoot and score on cargo ship.
 * 
 *
 * Command start:
 *  - Rotate cargo-intake to cargo ship shooting position.
 * 
 * Command finished when:
 *  - Position sensor indicates cargo-intake is at desired rotation.
 * 
 * Command end:
 *  - Stop/disable cargo-intake rotation motor.
 * 
 * Follow-up options:
 *  - None.
 */

RotateCargoForCargoShip::RotateCargoForCargoShip() {
    // This command needs the drivetrain subsystem to be available while running.
    Requires(&Robot::m_CargoIntake);
}

void RotateCargoForCargoShip::Initialize() {}

void RotateCargoForCargoShip::Execute() {}

bool RotateCargoForCargoShip::IsFinished() {}

void RotateCargoForCargoShip::End() {
    // Make sure the motors stop moving when they aren't being controlled.
}

void RotateCargoForCargoShip::Interrupted() {}
