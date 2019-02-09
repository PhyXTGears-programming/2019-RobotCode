#include "subsystems/DriveTrain.h"

#include "commands/TeleOpDrive.h"

#include <cmath>

DriveTrain::DriveTrain() : frc::Subsystem("DriveTrain") {
#   ifndef PROTOBOT
        // Set up TalonSRXs.
        m_MotorRightFront.ConfigFactoryDefault();
        m_MotorRightBack.ConfigFactoryDefault();
        m_MotorLeftFront.ConfigFactoryDefault();
        m_MotorLeftBack.ConfigFactoryDefault();

        // The documentation says to do this, so that both sides get the proper values.
        // See https://phoenix-documentation.readthedocs.io/en/latest/ch15_WPIDrive.html?highlight=wpi_talon
        m_MotorRightFront.SetInverted(false);
        m_MotorRightBack.SetInverted(false);
        m_MotorLeftFront.SetInverted(true);
        m_MotorLeftBack.SetInverted(true);
        m_RobotDrive.SetRightSideInverted(false);
#   endif
}

void DriveTrain::InitDefaultCommand() {
    // If no other command is using this subsystem, just drive like normal.
    SetDefaultCommand(new TeleOpDrive());
}

// Manually change the motors' power.
void DriveTrain::Drive(double left, double right) {
    m_RobotDrive.TankDrive(left, right);
}

// Given a controller object, use it to drive.
void DriveTrain::Drive(frc::XboxController& driver) {
    // Get left stick axes values.
    double hidX = -driver.GetX(frc::XboxController::kRightHand) * 0.7;
    double hidY = driver.GetY(frc::XboxController::kLeftHand) * 0.6;
    // double mag = (std::signbit(hidY) ? -1 : 1)*sqrt(hidX*hidX + hidY*hidY)*0.55;

    //m_RobotDrive.TankDrive(hidY/3, hidY/3);
    m_RobotDrive.ArcadeDrive(hidY, hidX, true);
}
