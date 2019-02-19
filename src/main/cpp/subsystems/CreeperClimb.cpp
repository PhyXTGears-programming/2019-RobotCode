#include "subsystems/CreeperClimb.h"
#include "Robot.h"

#include <iostream>

#define PID_NEAR_ZERO_THRESHOLD 1
#define PID_NEAR_ZERO_MAX_COUNT 6

#define ANGLE_TOLERANCE 2.5

/**
 * Machine angles are those reported by machine sensors and subject to drift
 * and defects in assembly.
 *
 * World angles are ideal values pertaining to actual orientation in the world
 * with respect to a zero reference.
 *
 * config.json stores world angles with a zero-point used to convert between
 * world and machine angles.
 */
double machineAngleToWorld(double);
double worldAngleToMachine(double);

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

void CreeperClimb::RotateArmToPosition(wpi::StringRef configName) {
    RotateArmToPosition((double)Robot::m_JsonConfig["climb"]["rotation"]["angles"][configName]);
}

void CreeperClimb::RotateArmToPosition(double worldAngle) {
    m_RotationPID.SetSetpoint(worldAngleToMachine(worldAngle));
    m_RotationPID.Enable();
}

// Returns a world angle of the creeper arm's position.
double CreeperClimb::GetCurrentArmPosition() {
    return machineAngleToWorld(m_ArmPosition.Get());
}

void CreeperClimb::SetArmRotateSpeed(double spd) { m_ArmRotate.Set(spd); }

void CreeperClimb::SetArmWheels(bool on) {
    m_ArmDrive.Set(on ? frc::Relay::kForward : frc::Relay::kOff);
}

void CreeperClimb::StopArmWheels() {
    m_ArmDrive.Set(frc::Relay::kOff);
}

void CreeperClimb::SetSolenoidAscend(bool on) { m_SolAscend.Set(on); }
void CreeperClimb::SetSolenoidDescend(bool on) { m_SolDescend.Set(on); }

bool CreeperClimb::GetSolenoidSwitch() { return !m_SolSwitch.Get(); }

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

bool CreeperClimb::IsArmAtPosition(wpi::StringRef configName) {
    double desiredAngle = Robot::m_JsonConfig["climb"]["rotation"]["angles"][configName];
    double actualAngle = GetCurrentArmPosition();

    return (ANGLE_TOLERANCE >= std::fabs(actualAngle - desiredAngle));
}

void CreeperClimb::StopArmRotation() {
    m_RotationPID.Disable();
}


// HELPER FUNCTIONS

double machineAngleToWorld(double machine) {
    return machine - (double)Robot::m_JsonConfig["climb"]["rotation"]["zero-point"];
}

double worldAngleToMachine(double world) {
    return world + (double)Robot::m_JsonConfig["climb"]["rotation"]["zero-point"];
}
