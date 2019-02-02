/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/DriveTrain.h"

#include "commands/TeleOpDrive.h"

#include <cmath>

DriveTrain::DriveTrain() : frc::Subsystem("DriveTrain") {
#   ifndef PROTOBOT
        // Set up TalonSRXs.
        m_motorRightFront.ConfigFactoryDefault();
        m_motorRightBack.ConfigFactoryDefault();
        m_motorLeftFront.ConfigFactoryDefault();
        m_motorLeftBack.ConfigFactoryDefault();

        // The documentation says to do this, so that both sides get the proper values.
        // See https://phoenix-documentation.readthedocs.io/en/latest/ch15_WPIDrive.html?highlight=wpi_talon
        m_motorRightFront.SetInverted(false);
        m_motorRightBack.SetInverted(false);
        m_motorLeftFront.SetInverted(true);
        m_motorLeftBack.SetInverted(true);
        m_robotDrive.SetRightSideInverted(false);
#   endif
}

void DriveTrain::InitDefaultCommand() {
    // If no other command is using this subsystem, just drive like normal.
    SetDefaultCommand(new TeleOpDrive());
}

// Manually change the motors' power.
void DriveTrain::Drive(double left, double right) {
    m_robotDrive.TankDrive(left, right);
}

// Given a controller object, use it to drive.
void DriveTrain::Drive(frc::XboxController& driver) {
    // Get left stick axes values.
    double hidX = -driver.GetX(frc::XboxController::kRightHand) * 0.7;
    double hidY = driver.GetY(frc::XboxController::kLeftHand) * 0.6;
    // double mag = (std::signbit(hidY) ? -1 : 1)*sqrt(hidX*hidX + hidY*hidY)*0.55;

    //m_robotDrive.TankDrive(hidY/3, hidY/3);
    m_robotDrive.ArcadeDrive(hidY, hidX, true);
}
