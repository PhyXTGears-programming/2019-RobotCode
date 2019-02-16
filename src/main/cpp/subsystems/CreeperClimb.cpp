#include "subsystems/CreeperClimb.h"

#include <iostream>

CreeperClimb::CreeperClimb(wpi::json &jsonConfig) : Subsystem("CreeperClimb") {
    double p = jsonConfig["climb"]["PID"]["P"];
    double i = jsonConfig["climb"]["PID"]["I"];
    double d = jsonConfig["climb"]["PID"]["D"];

    m_RotationPID.SetPID(p, i, d);
    std::cout << "CreeperClimbPID: " << p << " " << i << " " << d << std::endl;
}

void CreeperClimb::InitDefaultCommand() {}

void CreeperClimb::SetArmAngle(double ang) { m_RotationPID.SetSetpoint(ang); }
double CreeperClimb::GetCurrentArmAngle() { return m_ArmAngle.Get(); }

void CreeperClimb::SetRotateSpeed(double spd) { m_ArmRotate.Set(spd); }

void CreeperClimb::SetArmWheels(bool on) {
    m_ArmDrive.Set(on ? frc::Relay::kForward : frc::Relay::kOff);
}

void CreeperClimb::SetSolenoidAscend(bool on) { m_SolAscend.Set(on); }
void CreeperClimb::SetSolenoidDescend(bool on) { m_SolDescend.Set(on); }

bool CreeperClimb::GetSolenoidSwitch() { m_SolSwitch.Get(); }
