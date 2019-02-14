/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/CargoIntake.h"
#include "Robot.h"

#include <iostream>
#include <string>

CargoIntake::CargoIntake(wpi::json &jsonConfig) : Subsystem("CargoIntake") {
    double p = jsonConfig["intake"]["PID"]["P"];
    double i = jsonConfig["intake"]["PID"]["I"];
    double d = jsonConfig["intake"]["PID"]["D"];

    m_RotationPID.SetPID(p, i, d); // TODO Read JSON (PID Values)
    std::cout << "PID: " << p << " " << i << " " << d << std::endl;
}

void CargoIntake::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

#ifndef PROTOBOT
void CargoIntake::TurnOffIntakeRoller() {
    this->m_IntakeRoller.Set(frc::Relay::Value::kOff);
}

void CargoIntake::TurnOnIntakeRoller() {
    this->m_IntakeRoller.Set(frc::Relay::Value::kForward);
}
#endif

bool CargoIntake::HasCargo() {
    return !this->m_CargoSensor.Get();
}

bool CargoIntake::IsRotationDone() {
    // Rotation is done when PID error is near zero.
    if (3 > std::fabs(m_RotationPID.GetError())) {
        m_InRangeCount++;
        if (m_InRangeCount > 5) {
            return true;
        }
    } else {
        m_InRangeCount = 0;
    }
    return false;
}

void CargoIntake::RotateToPosition(wpi::StringRef configName) {
    // TODO Read JSON (angle + zero-point)
    double ang = std::stod(Robot::m_JsonConfig["intake"][configName].dump());
    ang += std::stod(Robot::m_JsonConfig["intake"]["zero-point"].dump());
    RotateToPosition(ang);
}

void CargoIntake::RotateToPosition(int angle) {
    m_RotationPID.SetSetpoint(angle);
    m_RotationPID.Enable();
}

void CargoIntake::StopRotation() {
}