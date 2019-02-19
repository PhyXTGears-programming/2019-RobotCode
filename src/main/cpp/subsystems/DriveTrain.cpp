#include "subsystems/DriveTrain.h"

#include "commands/TeleOpDrive.h"

#include <frc/smartdashboard/SendableBuilder.h>

#include <cmath>
#include <iostream>
#include <iomanip>

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

#   endif

    m_EncoderLeft.SetDistancePerPulse(kEncoderDistPerPulse);
    m_EncoderRight.SetDistancePerPulse(kEncoderDistPerPulse);

    SetMaxOutput(1.0);
    SetDeadband(0.05);

    Subsystem::AddChild("Left Drive PID", &m_LeftPID);
    Subsystem::AddChild("Right Drive PID", &m_RightPID);
    Subsystem::AddChild("Left Drive Encoder", &m_EncoderLeft);
    Subsystem::AddChild("Right Drive Encoder", &m_EncoderRight);

    static int instances = 0;
    ++instances;
    Subsystem::SetName("DriveTrain", instances);
}

void DriveTrain::InitDefaultCommand() {
    // If no other command is using this subsystem, just drive like normal.
    SetDefaultCommand(new TeleOpDrive());
}

// Manually change the motors' power.
void DriveTrain::Drive(double left, double right) {
    //m_RobotDrive.TankDrive(left, right);
}

// Given a controller object, use it to drive.
void DriveTrain::Drive(frc::XboxController& driver) {
    // Get left stick axes values.
    double hidX = -driver.GetX(frc::XboxController::kRightHand);
    double hidY = driver.GetY(frc::XboxController::kLeftHand);

    ArcadeDrive(hidY, hidX, true);
}

void DriveTrain::ArcadeDrive(double xSpeed, double zRotation, bool squareInputs) {
    xSpeed = Limit(xSpeed);
    xSpeed = ApplyDeadband(xSpeed, m_deadband);

    zRotation = Limit(zRotation);
    zRotation = ApplyDeadband(zRotation, m_deadband);

    // Square the inputs (while preserving the sign) to increase fine control
    // while permitting full power.
    if (squareInputs) {
        xSpeed = std::copysign(xSpeed * xSpeed, xSpeed);
        zRotation = std::copysign(zRotation * zRotation, zRotation);
    }

    double leftMotorOutput;
    double rightMotorOutput;

    double maxInput =
        std::copysign(std::max(
            std::abs(xSpeed),
            std::abs(zRotation)
        ),
        xSpeed
    );

    if (xSpeed >= 0.0) {
        // First quadrant, else second quadrant
        if (zRotation >= 0.0) {
            leftMotorOutput = maxInput;
            rightMotorOutput = xSpeed - zRotation;
        } else {
            leftMotorOutput = xSpeed + zRotation;
            rightMotorOutput = maxInput;
        }
    } else {
        // Third quadrant, else fourth quadrant
        if (zRotation >= 0.0) {
            leftMotorOutput = xSpeed + zRotation;
            rightMotorOutput = maxInput;
        } else {
            leftMotorOutput = maxInput;
            rightMotorOutput = xSpeed - zRotation;
        }
    }

    m_LeftPID.SetSetpoint(Limit(leftMotorOutput) * m_maxOutput * 140.0);
    m_RightPID.SetSetpoint(Limit(rightMotorOutput) * m_maxOutput * 140.0);
    m_LeftPID.Enable();
    m_RightPID.Enable();

    Feed();
}

void DriveTrain::StopMotor() {
    m_LeftPID.Disable();
    m_RightPID.Disable();

    Feed();
}

void DriveTrain::GetDescription(wpi::raw_ostream& desc) const {
    desc << "DriveTrain";
}

void DriveTrain::InitSendable(frc::SendableBuilder& builder) {
    builder.SetSmartDashboardType("DifferentialDrive");
    builder.SetActuator(true);
    builder.SetSafeState([=] { StopMotor(); });
    builder.AddDoubleProperty(
        "Left Motor Speed",
        [=]() { return m_LeftPID.Get(); },
        [=](double value) { return m_LeftPID.SetSetpoint(value); }
    );
    builder.AddDoubleProperty(
        "Right Motor Speed",
        [=]() { return m_RightPID.Get(); },
        [=](double value) { return m_RightPID.SetSetpoint(value); }
    );
}