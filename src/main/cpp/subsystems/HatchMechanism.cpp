#include "subsystems/HatchMechanism.h"
#include "Robot.h"
#include "util/util.h"

#include <iostream>
#include <string>
#include <cmath>


HatchMechanism::HatchMechanism(wpi::json &jsonConfig) : Subsystem("CargoIntake") {
}

void HatchMechanism::InitDefaultCommand() {
}

void HatchMechanism::SetRotateSpeed(double speed) {
    m_HatchMechanismMotor.Set(speed);
}
