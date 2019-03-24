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

    m_RotationUpPID.SetPID(p, i, d);
    m_RotationDownPID.SetPID(p, i, d);

    AddChild("Intake Arm PID (Up)", &m_RotationUpPID);
    AddChild("Intake Arm PID (Down)", &m_RotationDownPID);
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
    bool bothDisabled = !(m_RotationDownPID.IsEnabled() || m_RotationDownPID.IsEnabled());
    bool upDone = m_RotationUpPID.IsEnabled() && std::fabs(m_RotationUpPID.GetError()) < 0.008;
    bool downDone = m_RotationDownPID.IsEnabled() && std::fabs(m_RotationDownPID.GetError()) < 0.008;

    if (bothDisabled) {
        return true;
    } else if (upDone || downDone) {
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
    if (worldAngle > GetIntakeRotation()) {
        m_RotationUpPID.SetSetpoint(worldAngleToMachine(worldAngle));
        m_RotationUpPID.Enable();
    } else {
        m_RotationDownPID.SetSetpoint(worldAngleToMachine(worldAngle));
        m_RotationDownPID.Enable();
    }
}

void CargoIntake::SetRotateSpeed(double speed) {
    if (std::abs(speed) > 0.1) {
        m_RotationUpPID.Disable();
        m_RotationDownPID.Disable();
        m_IntakeArmMotor.Set(speed);
    } else if (!(m_RotationUpPID.IsEnabled() || m_RotationDownPID.IsEnabled())) {
        // Allow manual control deadband to stop motor, but don't interfere
        // when PID is active.
        m_IntakeArmMotor.Set(0.0);
    }
}

void CargoIntake::StopRotation() {
    m_RotationUpPID.Disable();
    m_RotationDownPID.Disable();
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

void CargoIntake::RunReset() {
    RotateToPosition(GetIntakeRotation());
}

void CargoIntake::Disable() {
    m_RotationUpPID.Reset();
    m_RotationDownPID.Reset();
}

bool CargoIntake::IsRollerRunning() {
    return (std::abs(m_IntakeRollerTop.Get()) > 0.1) || (std::abs(m_IntakeRollerBottom.Get()) > 0.1);
}