#include "subsystems/CargoIntake.h"
#include "Robot.h"
#include "util.h"

#include <iostream>
#include <string>

#define ANGLE_TOLERANCE 4       // Degrees


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
    AddChild("Cargo Ejecter", &m_CargoEjector);
    AddChild("Cargo Sensor", &m_CargoSensor);

    m_IntakeArmMotor.SetInverted(true);
}

void CargoIntake::InitDefaultCommand() {
}

#ifndef PROTOBOT
void CargoIntake::SetRollerSpeed(wpi::StringRef configName) {
    double speed = Robot::m_JsonConfig["intake"]["roller-speed"][configName];
    SetRollerSpeed(speed);
}

void CargoIntake::SetRollerSpeed(double speed) {
    this->m_IntakeRoller.Set(speed);
}

void CargoIntake::StopRoller() {
    this->m_IntakeRoller.Set(0.0);
}
#endif

bool CargoIntake::HasCargo() {
    return !m_CargoSensor.Get();
}

void CargoIntake::SetBottomHookPosition(double position) {
    // Don't adjust limits, otherwise the servo will spin, screw up its
    // positioning, and throw all setpoints out the window.
    m_HatchGripBottom.Set(util::clamp(position, 0.2, 0.8));
}

void CargoIntake::SetTopHookPosition(double position) {
    // Don't adjust limits, otherwise the servo will spin, screw up its
    // positioning, and throw all setpoints out the window.
    m_HatchGripTop.Set(util::clamp(position, 0.2, 0.8));
}

void CargoIntake::ExtendEjector() {
    m_CargoEjector.Set(0.3);
}

void CargoIntake::RetractEjector() {
    m_CargoEjector.Set(0.5);
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

    StopRoller();

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

void CargoIntake::SetHatchRotateSpeed(double speed) {
    m_HatchCheesecakeMotor.Set(speed);
}

// HELPER FUNCTIONS

double CargoIntake::machineAngleToWorld(double machine) {
    return machine - (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
}

double CargoIntake::worldAngleToMachine(double world) {
    return world + (double)Robot::m_JsonConfig["intake"]["rotation"]["zero-point"];
}
