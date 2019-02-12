#include "subsystems/CargoIntake.h"

CargoIntake::CargoIntake() : Subsystem("CargoIntake") {}

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

bool CargoIntake::IsRotationDone() {}

void CargoIntake::RotateToPosition(wpi::StringRef configName) {}

void CargoIntake::StopRotation() {}