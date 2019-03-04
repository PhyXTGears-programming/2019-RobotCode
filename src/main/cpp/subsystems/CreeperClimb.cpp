#include "subsystems/CreeperClimb.h"
#include "Robot.h"

#include <iostream>
#include <cmath>

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

    // Configure interrupt on limit switch.  Since input is normally high,
    // watch for falling edge.
    m_SolSwitch.RequestInterrupts(
        [](uint32_t mask, void* obj) { static_cast<CreeperClimb*>(obj)->m_IsPistonAtLimitLatch = true; },
        static_cast<void*>(this)
    );
    m_SolSwitch.SetUpSourceEdge(false /* rising */, true /* falling */);
    m_SolSwitch.EnableInterrupts();

    AddChild("Climb Arm PID", m_RotationPID);
    AddChild("Climb Arm Angle", m_ArmPosition);
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

void CreeperClimb::SetArmRotateSpeed(double spd) {
    if (std::abs(spd) > 0.1) {
        m_RotationPID.Disable();
        m_ArmRotate.Set(spd);
    } else if (!m_RotationPID.IsEnabled()) {
        // Allow manual control deadband to stop motor, but don't interfere
        // when PID is active.
        m_ArmRotate.Set(spd);
    }
}

void CreeperClimb::SetArmWheels(bool on) {
    m_ArmDrive.Set(on ? frc::Relay::kOn : frc::Relay::kOff);
}

void CreeperClimb::StopArmWheels() {
    m_ArmDrive.Set(frc::Relay::kOff);
}

/**
 * Open (on) or close (off) the solenoid that controls ascension of the robot
 * as done by extending the piston.
 */
void CreeperClimb::SetSolenoidAscend(bool on) { m_SolAscend.Set(on); }

/**
 * Open (on) or close (off) the solenoid that controls the descent of the robot
 * as done by retracting the piston.
 */
void CreeperClimb::SetSolenoidDescend(bool on) { m_SolDescend.Set(on); }

/**
 * Disconnect air pressure from piston.
 */
void CreeperClimb::PistonDisable() {
    SetSolenoidAscend(false);
    SetSolenoidDescend(false);
}

/**
 * Extends piston to cause robot to ascend.
 */
void CreeperClimb::PistonExtend() {
    SetSolenoidAscend(true);
    SetSolenoidDescend(false);
}

/**
 * Retract piston to cause robot to descend.
 */
void CreeperClimb::PistonRetract() {
    SetSolenoidAscend(false);
    SetSolenoidDescend(true);
}

/**
 * Pressurize both sides of piston to hold its position.
 */
void CreeperClimb::PistonHold() {
    SetSolenoidAscend(true);
    SetSolenoidDescend(true);
}

void CreeperClimb::ResetPistonLimitLatch() {
    m_IsPistonAtLimitLatch = false;
}

bool CreeperClimb::IsPistonAtLimit() {
    return m_IsPistonAtLimitLatch;
}

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

void CreeperClimb::SetRotatePIDOutputRange(double minimumOutput, double maximumOutput) {
    m_RotationPID.SetOutputRange(minimumOutput, maximumOutput);
}


// HELPER FUNCTIONS

double CreeperClimb::machineAngleToWorld(double machine) {
    return machine - (double)Robot::m_JsonConfig["climb"]["rotation"]["zero-point"];
}

double CreeperClimb::worldAngleToMachine(double world) {
    return world + (double)Robot::m_JsonConfig["climb"]["rotation"]["zero-point"];
}
