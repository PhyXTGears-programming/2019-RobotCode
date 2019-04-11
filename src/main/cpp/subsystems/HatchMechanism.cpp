#include "subsystems/HatchMechanism.h"
#include "Robot.h"
#include "util/util.h"

#include <iostream>
#include <string>
#include <cmath>

#define HatchGrabberSpeed 0.7

#define PID_NEAR_ZERO_THRESHOLD 3
#define PID_NEAR_ZERO_MAX_COUNT 5


HatchMechanism::HatchMechanism(wpi::json &jsonConfig) : Subsystem("HatchMechanism") {
    double p = jsonConfig["hatch"]["PID"]["P"];
    double i = jsonConfig["hatch"]["PID"]["I"];
    double d = jsonConfig["hatch"]["PID"]["D"];

    m_ArmPID.SetPID(p, i, d);

    m_Config.MidPosition = jsonConfig["hatch"]["rotation"]["mid-position"];
    m_Config.TopPosition = jsonConfig["hatch"]["rotation"]["top-position"];
    m_Config.GripThreshold = jsonConfig["hatch"]["rotation"]["grip-threshold"];

    AddChild("Hatch Arm Position", &m_ArmPosition);
    AddChild("Hatch Arm PID", &m_ArmPID);
}

void HatchMechanism::InitDefaultCommand() {
}

void HatchMechanism::RaiseHatch () {
    m_ArmPID.Disable();
    SetRotateSpeed(HatchGrabberSpeed);
}

void HatchMechanism::LowerHatch () {
    m_ArmPID.Disable();
    SetRotateSpeed(-HatchGrabberSpeed);
}

void HatchMechanism::RotateToMidPosition() {
    m_ArmPID.SetSetpoint(m_Config.MidPosition);
    m_ArmPID.Enable();
}

void HatchMechanism::RotateToTopPosition() {
    m_ArmPID.SetSetpoint(m_Config.TopPosition);
    m_ArmPID.Enable();
}

bool HatchMechanism::IsArmRotationDone() {
    // Rotation is done when PID error is near zero.
    if (PID_NEAR_ZERO_THRESHOLD > std::abs(m_ArmPID.GetError())) {
        m_InRangeCount++;
        if (m_InRangeCount > PID_NEAR_ZERO_MAX_COUNT) {
            // Don't give next check doesn't have a head start.
            m_InRangeCount = 0;
            return true;
        }
    } else {
        m_InRangeCount = 0;
    }
    return false;
}

void HatchMechanism::StopRotation () {
    SetRotateSpeed(0.0);
    m_ArmPID.Disable();
}

void HatchMechanism::GrabHatch () {
    m_SolRetractHatch.Set(true);
    m_SolExtendHatch.Set(false);
}

void HatchMechanism::ReleaseHatch () {
    m_SolExtendHatch.Set(true);
    m_SolRetractHatch.Set(false);
}

void HatchMechanism::SetRotateSpeed(double speed) {
    m_ArmMotor.Set(speed);
}

void HatchMechanism::Disable() {
    SetRotateSpeed(0.0);
    m_ArmPID.Reset();
}
