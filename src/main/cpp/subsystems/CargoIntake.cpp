#include "subsystems/CargoIntake.h"
#include "Robot.h"

#include <iostream>
#include <string>

#define ANGLE_TOLERANCE 2       // Degrees


CargoIntake::CargoIntake(wpi::json &jsonConfig) : Subsystem("CargoIntake") {
    double p = jsonConfig["intake"]["PID"]["P"];
    double i = jsonConfig["intake"]["PID"]["I"];
    double d = jsonConfig["intake"]["PID"]["D"];

    m_RotationPID.SetPID(p, i, d);

    AddChild("Intake Arm PID", &m_RotationPID);
    AddChild("Intake Angle", &m_IntakeRotation);
    AddChild("Hatch Grip (Bottom)", &m_HatchGripBottom);
    AddChild("Hatch Grip (Top)", &m_HatchGripTop);
    AddChild("Intake Arm Motor", &m_IntakeArmMotor);
    AddChild("Intake Rollers", &m_IntakeRoller);
    AddChild("Intake Ejecter", &m_IntakeEject);
    AddChild("Cargo Sensor", &m_CargoSensor);

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
    return this->m_CargoSensor.Get();
}

bool CargoIntake::IsRotationDone() {
    // Rotation is done when PID error is near zero.
    if (std::fabs(m_RotationPID.GetError()) < 1) {
        m_InRangeCount++;
        if (m_InRangeCount > 5) {
            return true;
        }
    } else {
        m_InRangeCount = 0;
    }

    return false;
}

bool CargoIntake::IsAtPosition(wpi::StringRef configName) {
    double desiredAngle = Robot::m_JsonConfig["intake"]["rotation"]["angles"][configName];
    double actualAngle = GetIntakeRotation();

    return (ANGLE_TOLERANCE >= std::fabs(actualAngle - desiredAngle));
}

void CargoIntake::GoHome() {
    GripHatchBottom();
    GripHatchTop();

    TurnOffIntakeRoller();

    double ang = Robot::m_JsonConfig["intake"]["rotation"]["home"];
    RotateToPosition(ang);
}

void CargoIntake::RotateToPosition(wpi::StringRef configName) {
    double ang = Robot::m_JsonConfig["intake"]["rotation"]["angles"][configName];
    RotateToPosition(ang);
}

void CargoIntake::RotateToPosition(int worldAngle) {
    m_RotationPID.SetSetpoint(worldAngleToMachine(worldAngle));
    m_RotationPID.Enable();
}

void CargoIntake::SetRotateSpeed(double speed) {
    m_IntakeArmMotor.Set(speed);
}

void CargoIntake::StopRotation() {
    m_RotationPID.Disable();
}

double CargoIntake::GetIntakeRotation() {
    return machineAngleToWorld(m_IntakeRotation.Get());
}

// HELPER FUNCTIONS

double CargoIntake::machineAngleToWorld(double machine) {
    return machine - (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
}

double CargoIntake::worldAngleToMachine(double world) {
    return world + (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
}
