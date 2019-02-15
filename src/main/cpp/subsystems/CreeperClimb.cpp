#include "subsystems/CreeperClimb.h"

CreeperClimb::CreeperClimb() : Subsystem("CreeperClimb") {}

void CreeperClimb::InitDefaultCommand() {}

double CreeperClimb::GetArmAngle() { return m_ArmAngle.Get(); }

void CreeperClimb::SetRotateSpeed(double spd) { m_ArmRotate.Set(spd); }

void CreeperClimb::SetRelay(bool on) {
    m_ArmDrive.Set(on ? frc::Relay::kForward : frc::Relay::kOff);
}

void CreeperClimb::SetSolenoidAscend(bool on) { m_SolAscend.Set(on); }
void CreeperClimb::SetSolenoidDescend(bool on) { m_SolDescend.Set(on); }
