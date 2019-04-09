#include "subsystems/HatchMechanism.h"
#include "Robot.h"
#include "util/util.h"

#include <iostream>
#include <string>
#include <cmath>

#define HatchGrabberSpeed 0.7

HatchMechanism::HatchMechanism(wpi::json &jsonConfig) : Subsystem("HatchMechanism") {
}

void HatchMechanism::InitDefaultCommand() {
}

void HatchMechanism::RaiseHatch () {
    SetRotateSpeed(HatchGrabberSpeed);
}

void HatchMechanism::LowerHatch () {
    SetRotateSpeed(-HatchGrabberSpeed);
}

void HatchMechanism::StopRotation () {
    SetRotateSpeed(0.0);
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
}
