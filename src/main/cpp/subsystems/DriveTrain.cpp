/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/DriveTrain.h"

#include "commands/TeleOpDrive.h"

DriveTrain::DriveTrain() : frc::Subsystem("DriveTrain") {}

void DriveTrain::InitDefaultCommand() {
    SetDefaultCommand(new TeleOpDrive());
}

void DriveTrain::Drive(frc::XboxController& driver) {
    double hidX = driver.GetX(frc::XboxController::kLeftHand);
    double hidY = driver.GetY(frc::XboxController::kLeftHand);

    m_robotDrive.TankDrive(hidY, hidY);
} 

// Put methods for controlling this subsystem
// here. Call these from Commands.
