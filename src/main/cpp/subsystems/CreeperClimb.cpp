#include "subsystems/CreeperClimb.h"
#include "Robot.h"

#include <iostream>

#define PID_NEAR_ZERO_THRESHOLD 1
#define PID_NEAR_ZERO_MAX_COUNT 6

CreeperClimb::CreeperClimb(wpi::json &jsonConfig) : Subsystem("CreeperClimb") {
    double p = jsonConfig["climb"]["PID"]["P"];
    double i = jsonConfig["climb"]["PID"]["I"];
    double d = jsonConfig["climb"]["PID"]["D"];

    m_RotationPID.SetPID(p, i, d);
    std::cout << "CreeperClimbPID: " << p << " " << i << " " << d << std::endl;

    m_ArmRotate.SetInverted(true);

    AddChild("Climb Arm PID", m_RotationPID);
    AddChild("Climb Arm Angle", m_ArmAngle);
    AddChild("Climb Arm Motor", m_ArmRotate);
    AddChild("Climb Arm Wheels", m_ArmDrive);
    AddChild("Climb Sol Ascend", m_SolAscend);
    AddChild("Climb Sol Descend", m_SolDescend);
    AddChild("Climb Sol Limit", m_SolSwitch);
}

void CreeperClimb::InitDefaultCommand() {}

void CreeperClimb::SetArmAngle(wpi::StringRef configName) {
    SetArmAngle((double)Robot::m_JsonConfig["climb"][configName]);
}

void CreeperClimb::SetArmAngle(double ang) {
    m_RotationPID.SetSetpoint(ang);
    m_RotationPID.Enable();
}

double CreeperClimb::GetCurrentArmAngle() { return m_ArmAngle.Get(); }

void CreeperClimb::SetRotateSpeed(double spd) { m_ArmRotate.Set(spd); }

void CreeperClimb::SetArmWheels(bool on) {
    m_ArmDrive.Set(on ? frc::Relay::kForward : frc::Relay::kOff);
}

void CreeperClimb::SetSolenoidAscend(bool on) { m_SolAscend.Set(on); }
void CreeperClimb::SetSolenoidDescend(bool on) { m_SolDescend.Set(on); }

bool CreeperClimb::GetSolenoidSwitch() { m_SolSwitch.Get(); }

bool CreeperClimb::IsArmRotationDone() {
    // Rotation is done when PID error is near zero.
    if (PID_NEAR_ZERO_THRESHOLD > std::fabs(m_RotationPID.GetError())) {
        m_InRangeCount++;
        if (m_InRangeCount > PID_NEAR_ZERO_MAX_COUNT) {
            return true;
        }
    } else {
        m_InRangeCount = 0;
    }
    return false;
}

void CreeperClimb::StopArmRotation() {
    m_RotationPID.Disable();
}
