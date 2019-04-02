#include "subsystems/CargoIntake.h"
#include "Robot.h"
#include "util/util.h"

#include <iostream>
#include <string>
#include <cmath>

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
    AddChild("Intake Top Roller", &m_IntakeRollerTop);
    AddChild("Intake Bottom Roller", &m_IntakeRollerBottom);
    AddChild("Cargo Ejecter", &m_CargoEjector);
    AddChild("Cargo Sensor", &m_CargoSensor);

    m_IntakeArmMotor.SetInverted(true);

    m_Config.EjectorExtend = jsonConfig["intake"]["ejector"]["extend"];
    m_Config.EjectorRetract = jsonConfig["intake"]["ejector"]["retract"];
}

void CargoIntake::InitDefaultCommand() {
}

#ifndef PROTOBOT
void CargoIntake::SetRollerSpeed(wpi::StringRef configName) {
    double topspeed = Robot::m_JsonConfig["intake"]["roller-speed"][configName]["top"];
    double bottomspeed = Robot::m_JsonConfig["intake"]["roller-speed"][configName]["bottom"];
    SetRollerSpeed(topspeed, bottomspeed);
}

void CargoIntake::SetRollerSpeed(double topspeed, double bottomspeed) {
    this->m_IntakeRollerTop.Set(topspeed);
    this->m_IntakeRollerBottom.Set(bottomspeed);
}

void CargoIntake::StopRoller() {
    this->m_IntakeRollerTop.Set(0.0);
    this->m_IntakeRollerBottom.Set(0.0);
}
#endif

bool CargoIntake::HasCargo() {
    return !m_CargoSensor.Get();
}

void CargoIntake::SetBottomHookPosition(wpi::StringRef configName) {
    double position = Robot::m_JsonConfig["intake"]["hooks"]["bottom"][configName];
    SetBottomHookPosition(position);
}

void CargoIntake::SetBottomHookPosition(double position) {
    // Don't adjust limits, otherwise the servo will spin, screw up its
    // positioning, and throw all setpoints out the window.
    m_HatchGripBottom.Set(util::clamp(position, 0.2, 0.8));
}

void CargoIntake::SetTopHookPosition(wpi::StringRef configName) {
    double position = Robot::m_JsonConfig["intake"]["hooks"]["top"][configName];
    SetTopHookPosition(position);
}

void CargoIntake::SetTopHookPosition(double position) {
    // Don't adjust limits, otherwise the servo will spin, screw up its
    // positioning, and throw all setpoints out the window.
    m_HatchGripTop.Set(util::clamp(position, 0.2, 0.8));
}

void CargoIntake::ExtendEjector() {
    m_CargoEjector.Set(m_Config.EjectorExtend);
}

void CargoIntake::RetractEjector() {
    m_CargoEjector.Set(m_Config.EjectorRetract);
}

void CargoIntake::SetEjector(double value) {
    m_CargoEjector.Set(value);
}

bool CargoIntake::IsRotationDone() {
    // Rotation is done when PID error is near zero.
    if (std::fabs(m_RotationPID.GetError()) < 0.008) {
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
    SetBottomHookPosition("docked");
    SetTopHookPosition("docked");

    StopRoller();

    RotateToPosition("home");
}

void CargoIntake::RotateToPosition(wpi::StringRef configName) {
    double ang = Robot::m_JsonConfig["intake"]["rotation"]["angles"][configName];
    RotateToPosition(ang);
}

void CargoIntake::RotateToPosition(double worldAngle) {
    m_RotationPID.SetSetpoint(worldAngleToMachine(worldAngle));
    m_RotationPID.Enable();
}

void CargoIntake::SetRotateSpeed(double spd) {
    if (std::abs(spd) > 0.1) {
        m_RotationPID.Disable();
        m_IntakeArmMotor.Set(spd);
    } else if (!m_RotationPID.IsEnabled()) {
        // Allow manual control deadband to stop motor, but don't interfere
        // when PID is active.
        m_IntakeArmMotor.Set(std::abs(spd) > 0.03 ? spd : 0.0);
    }
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

void CargoIntake::RunReset() {
    RotateToPosition(GetIntakeRotation());
}

void CargoIntake::Disable() {
    m_RotationPID.Reset();
}

bool CargoIntake::IsRollerRunning() {
    return (std::abs(m_IntakeRollerTop.Get()) > 0.1) || (std::abs(m_IntakeRollerBottom.Get()) > 0.1);
}