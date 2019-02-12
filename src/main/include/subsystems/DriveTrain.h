#pragma once

#include "RobotMap.h"
#include "VelocitySource.h"

#include <frc/ADXRS450_Gyro.h>
#include <frc/Encoder.h>
#include <frc/PIDController.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/XboxController.h>
#include <frc/commands/Subsystem.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/drive/RobotDriveBase.h>

#include <wpi/raw_ostream.h>

#ifdef PROTOBOT
    // Our team's prototyping bot just uses regular Victor motor controllers.
#   include <frc/Victor.h>
#   define  MOTORTYPE frc::Victor
#else
    // The WPI_TalonSRX type provides compatibility with WPILib's drivetrain types.
#   include <ctre/Phoenix.h>
#   define  MOTORTYPE WPI_TalonSRX
#endif

#include "RobotMap.h"

class DriveTrain : public frc::Subsystem, public frc::RobotDriveBase {
    public:
        DriveTrain();
        void InitDefaultCommand() override;
        void Drive(double left, double right);
        void Drive(frc::XboxController& driver);
        void ArcadeDrive(double xSpeed, double zRotation, bool squareInputs);

        void StopMotor() override;
        void GetDescription(wpi::raw_ostream& desc) const override;
        void InitSendable(frc::SendableBuilder& builder) override;

    private:
        // Motors
        MOTORTYPE m_MotorRightFront {kRightFrontMotor};
        MOTORTYPE m_MotorRightBack  {kRightBackMotor};
        MOTORTYPE m_MotorLeftFront  {kLeftFrontMotor};
        MOTORTYPE m_MotorLeftBack   {kLeftBackMotor};

        // Group the motors into their sides and then combine them into the drivetrain
        frc::SpeedControllerGroup m_RightMotors {m_MotorRightFront, m_MotorRightBack};
        frc::SpeedControllerGroup m_LeftMotors {m_MotorLeftFront, m_MotorLeftBack};
        // Encoders (reverse the right encoder)
        frc::Encoder m_EncoderRight {kRightEncoderPin0, kRightEncoderPin1, true};
        frc::Encoder m_EncoderLeft  {kLeftEncoderPin0, kLeftEncoderPin1, false};
        //frc::ADXRS450_Gyro m_Gyro {}; // onboard RoboRIO gyro

        VelocitySource m_EncoderRightPID {m_EncoderRight};
        VelocitySource m_EncoderLeftPID  {m_EncoderLeft};

        frc::PIDController m_RightPID {
            kRightP, kRightI, kRightD,
            m_EncoderRightPID,
            m_RightMotors
        };

        frc::PIDController m_LeftPID {
            kLeftP,  kLeftI,  kLeftD,
            m_EncoderLeftPID,
            m_LeftMotors
        };
};
