#include "subsystems/CreeperArm.h"

CreeperArm::CreeperArm() : Subsystem("ExampleSubsystem") {}

void CreeperArm::InitDefaultCommand() {}

// #ifndef PROTOBOT

// double CreeperArm::GetArmAngle() { return m_ArmHipot.Get(); }

// void CreeperArm::SetRotateSpeed(double spd) { m_ArmRotate.Set(spd); }

// void CreeperArm::SetRelay(bool on) {
//     m_ArmDrive.Set(on ? frc::Relay::kForward : frc::Relay::kOff);
// }

// void CreeperArm::SetSolenoidAscend(bool on) { m_SolAscend.Set(on); }
// void CreeperArm::SetSolenoidDescend(bool on) { m_SolDescend.Set(on); }

// #endif
