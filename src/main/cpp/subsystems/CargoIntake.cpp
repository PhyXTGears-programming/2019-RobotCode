/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/CargoIntake.h"

CargoIntake::CargoIntake() : Subsystem("CargoIntake") {
    m_RotationPID.SetPID(1.0, 0.0, 0.0); // TODO Read JSON (PID Values)
}

void CargoIntake::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

#ifndef PROTOBOT
void CargoIntake::TurnOffIntakeRoller() {
    this->m_IntakeRoller.Set(frc::Relay::Value::kOff);
}

void CargoIntake::TurnOnIntakeRoller() {
    this->m_IntakeRoller.Set(frc::Relay::Value::kForward);
}
#endif

bool CargoIntake::HasCargo() {
    return !this->m_CargoSensor.Get();
}

bool CargoIntake::IsRotationDone() {
    // Rotation is done when PID error is near zero.
    return 0.001 > std::fabs(m_RotationPID.GetError());
}

void CargoIntake::RotateToPosition(wpi::StringRef configName) {
    // TODO Read JSON (angle + zero-point)
}

void CargoIntake::RotateToPosition(int angle) {
    m_RotationPID.SetSetpoint(angle);
    m_RotationPID.Enable();
}

void CargoIntake::StopRotation() {
    m_RotationPID.Disable();
}