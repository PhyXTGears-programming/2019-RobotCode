#include "subsystems/CargoIntake.h"
#include "Robot.h"

#include <iostream>
#include <string>

CargoIntake::CargoIntake(wpi::json &jsonConfig) : Subsystem("CargoIntake") {
    double p = jsonConfig["intake"]["PID"]["P"];
    double i = jsonConfig["intake"]["PID"]["I"];
    double d = jsonConfig["intake"]["PID"]["D"];

    m_RotationPID.SetPID(p, i, d);

    AddChild(&m_IntakeRotation);

    m_IntakeArmMotor.SetInverted(true);
}

void CargoIntake::InitDefaultCommand() {
}

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
    if (std::fabs(m_RotationPID.GetError()) < 3) {
        m_InRangeCount++;
        if (m_InRangeCount > 5) {
            return true;
        }
    } else {
        m_InRangeCount = 0;
    }

    return false;
}

void CargoIntake::GoHome() {
    GripHatchBottom();
    GripHatchTop();

    TurnOffIntakeRoller();

    double ang = Robot::m_JsonConfig["intake"]["rotation"]["home"];
    ang += (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
    RotateToPosition(ang);
}

void CargoIntake::RotateToPosition(wpi::StringRef configName) {
    double ang = Robot::m_JsonConfig["intake"]["rotation"]["angles"][configName];
    ang += (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
    RotateToPosition(ang);
}

void CargoIntake::RotateToPosition(int angle) {
    m_RotationPID.SetSetpoint(angle);
    m_RotationPID.Enable();
}

void CargoIntake::StopRotation() {
    m_RotationPID.Disable();
}

double CargoIntake::GetIntakeRotation() {
    return m_IntakeRotation.Get() - (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
}
