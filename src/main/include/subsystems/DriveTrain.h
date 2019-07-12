#pragma once

#include "RobotMap.h"
#include "util/StopWatch.h"
#include "VelocitySource.h"

#include <frc/ADXRS450_Gyro.h>
#include <frc/Encoder.h>
#include <frc/PIDController.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/XboxController.h>
#include <frc/commands/Subsystem.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/drive/RobotDriveBase.h>

#include <wpi/json.h>
#include <wpi/raw_ostream.h>

#ifdef PROTOBOT
    // Our team's prototyping bot just uses regular Victor motor controllers.
#   include <frc/Victor.h>
#   define  MOTORTYPE frc::Victor
#else
#   include <rev/CANSparkMax.h>
#   define  MOTORTYPE rev::CANSparkMax
#endif

#include "RobotMap.h"

typedef rev::CANSparkMax::IdleMode IdleMode;

class DriveTrain : public frc::Subsystem, public frc::RobotDriveBase {
    public:
        DriveTrain(wpi::json&);
        void InitDefaultCommand() override;
        void Drive(double left, double right);
        void Drive(frc::XboxController& driver);
        void ArcadeDrive(double xSpeed, double zRotation, bool squareInputs = false);

        void SetIdleMode(IdleMode mode);
        void ToggleIdleMode();
        IdleMode GetIdleMode();
        wpi::StringRef GetIdleModeText();

        void StopMotor() override;
        void GetDescription(wpi::raw_ostream& desc) const override;
        void InitSendable(frc::SendableBuilder& builder) override;

        void RunReset();

        void UseNormalSpeedLimit();
        void UseDukesSpeedLimit();

    private:
        // Motors
        MOTORTYPE m_MotorRight1 {kRightMotor1, rev::CANSparkMax::MotorType::kBrushless};
        MOTORTYPE m_MotorRight2 {kRightMotor2, rev::CANSparkMax::MotorType::kBrushless};
        MOTORTYPE m_MotorRight3 {kRightMotor3, rev::CANSparkMax::MotorType::kBrushless};
        MOTORTYPE m_MotorLeft1 {kLeftMotor1, rev::CANSparkMax::MotorType::kBrushless};
        MOTORTYPE m_MotorLeft2 {kLeftMotor2, rev::CANSparkMax::MotorType::kBrushless};
        MOTORTYPE m_MotorLeft3 {kLeftMotor3, rev::CANSparkMax::MotorType::kBrushless};

        IdleMode m_IdleMode {IdleMode::kCoast};

        // Group the motors into their sides and then combine them into the drivetrain
        frc::SpeedControllerGroup m_RightMotors {m_MotorRight1, m_MotorRight2, m_MotorRight3};
        frc::SpeedControllerGroup m_LeftMotors {m_MotorLeft1, m_MotorLeft2, m_MotorLeft3};
        // frc::DifferentialDrive m_Drive {m_LeftMotors, m_RightMotors};
        // Encoders (reverse the right encoder)
        frc::Encoder m_EncoderRight {kRightEncoderPin0, kRightEncoderPin1, false};
        frc::Encoder m_EncoderLeft  {kLeftEncoderPin0, kLeftEncoderPin1, true};
        //frc::ADXRS450_Gyro m_Gyro {}; // onboard RoboRIO gyro

        #ifdef USE_DRIVETRAIN_PID
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
        #else
            // Limit acceleration.
            double m_MaxAcceleration;
            StopWatch m_TimeDelta;

            double ComputeNextOutput(double iVel, double fVel, double maxAccel, double timeDelta);

            // Config setpoints.
            double m_MaxNormalSpeed;
            double m_SandstormStepSpeed;
            double m_TurnFactor;

            // Member variables used for dashboard reporting.
            double m_DashboardLeftOutput;
            double m_DashboardRightOutput;
            double m_DashboardTimeDelta;
        #endif
};
