#include "subsystems/DriveTrain.h"

#include "commands/TeleOpDrive.h"

#include <frc/smartdashboard/SendableBuilder.h>

#include <cmath>

DriveTrain::DriveTrain(wpi::json &jsonConfig) : frc::Subsystem("DriveTrain") {
#   ifndef PROTOBOT
        // Set up TalonSRXs.
        m_MotorRightFront.ConfigFactoryDefault();
        m_MotorRightBack.ConfigFactoryDefault();
        m_MotorLeftFront.ConfigFactoryDefault();
        m_MotorLeftBack.ConfigFactoryDefault();

        int contLimit = 40;
        int peakLimit = 50;
        int peakTime = 1800;

        m_MotorRightFront.EnableCurrentLimit(true);
        m_MotorRightFront.ConfigContinuousCurrentLimit(contLimit, 10);
        m_MotorRightFront.ConfigPeakCurrentLimit(peakLimit, 10);
        m_MotorRightFront.ConfigPeakCurrentDuration(peakTime, 10);

        m_MotorRightBack.EnableCurrentLimit(true);
        m_MotorRightBack.ConfigContinuousCurrentLimit(contLimit, 10);
        m_MotorRightBack.ConfigPeakCurrentLimit(peakLimit, 10);
        m_MotorRightBack.ConfigPeakCurrentDuration(peakTime, 10);

        m_MotorLeftFront.EnableCurrentLimit(true);
        m_MotorLeftFront.ConfigContinuousCurrentLimit(contLimit, 10);
        m_MotorLeftFront.ConfigPeakCurrentLimit(peakLimit, 10);
        m_MotorLeftFront.ConfigPeakCurrentDuration(peakTime, 10);

        m_MotorLeftBack.EnableCurrentLimit(true);
        m_MotorLeftBack.ConfigContinuousCurrentLimit(contLimit, 10);
        m_MotorLeftBack.ConfigPeakCurrentLimit(peakLimit, 10);
        m_MotorLeftBack.ConfigPeakCurrentDuration(peakTime, 10);

        // The documentation says to do this, so that both sides get the proper values.
        // See https://phoenix-documentation.readthedocs.io/en/latest/ch15_WPIDrive.html?highlight=wpi_talon
        m_MotorRightFront.SetInverted(false);
        m_MotorRightBack.SetInverted(false);
        m_MotorLeftFront.SetInverted(true);
        m_MotorLeftBack.SetInverted(true);

#   endif

    m_EncoderLeft.SetDistancePerPulse(kEncoderDistPerPulse);
    m_EncoderRight.SetDistancePerPulse(kEncoderDistPerPulse);

    #ifdef USE_DRIVETRAIN_PID
        Subsystem::AddChild("Left Drive PID", &m_LeftPID);
        Subsystem::AddChild("Right Drive PID", &m_RightPID);
    #else
        m_MaxAcceleration = jsonConfig["drive"]["max-acceleration"];

        m_MaxNormalSpeed = jsonConfig["drive"]["max-normal-speed"];
        m_SandstormStepSpeed = jsonConfig["drive"]["sandstorm-step-speed"];
        m_TurnFactor = jsonConfig["drive"]["turn-factor"];

        m_DashboardLeftOutput = 0.0;
        m_DashboardRightOutput = 0.0;
        m_DashboardTimeDelta = 0.0;
    #endif

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
    // m_RobotDrive.TankDrive(left, right);
}

// Given a controller object, use it to drive.
void DriveTrain::Drive(frc::XboxController& driver) {
    // Get left stick axes values.
    double hidX = -driver.GetX(frc::XboxController::kRightHand);
    double hidY = driver.GetY(frc::XboxController::kLeftHand);
    double sprintFactor = 0.75;
    sprintFactor += driver.GetTriggerAxis(frc::XboxController::kRightHand) * 0.25;
    sprintFactor -= driver.GetTriggerAxis(frc::XboxController::kLeftHand) * 0.25;

    double turnFactor = 1 - (driver.GetTriggerAxis(frc::XboxController::kLeftHand) * 0.3); // THIS

    if (ENABLE_DRIVETRAIN_CONTROL) {
        ArcadeDrive(hidY * sprintFactor, hidX * turnFactor, true);
    } else {
        // m_Drive.ArcadeDrive(hidY, hidX, true);
    }
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

    #ifdef USE_DRIVETRAIN_PID
        m_LeftPID.SetSetpoint(Limit(leftMotorOutput) * m_maxOutput * 140.0);
        m_RightPID.SetSetpoint(Limit(rightMotorOutput) * m_maxOutput * 140.0);
        m_LeftPID.Enable();
        m_RightPID.Enable();
    #else

        // Now that we have the desired left and right speeds, limit the
        // acceleration while maintaining the ratio between left and right
        // velocities.
        double desiredLeft = leftMotorOutput;
        double desiredRight = rightMotorOutput;

        double currentLeft = m_LeftMotors.Get() / m_maxOutput;
        double currentRight = m_RightMotors.Get() / m_maxOutput;

        double maxLeftAccel = m_MaxAcceleration, maxRightAccel = m_MaxAcceleration;

        /* *
        // Descrease the max velocity step size for the slower drive side so
        // turning takes effect sooner.
        //
        // WARNING: Doesn't really work from initial testing.  Robot drives more straight.
        if (0.01 < std::abs(desiredLeft) - std::abs(desiredRight)) {
            // Left speed is faster than right.
            maxLeftAccel = m_MaxAcceleration;
            maxRightAccel = m_MaxAcceleration * desiredRight / desiredLeft;
        } else if (0.01 < std::abs(desiredRight) - std::abs(desiredLeft)) {
            // Right speed is faster than left.
            maxLeftAccel = m_MaxAcceleration * desiredLeft / desiredRight;
            maxRightAccel = m_MaxAcceleration;
        } else {
            // Both speeds are effectively same.
            maxLeftAccel = m_MaxAcceleration;
            maxRightAccel = m_MaxAcceleration;
        }
        /* */

        double timeDelta = m_TimeDelta.Split();

        // Basic formula behind this is:
        //
        //  v_{i+1} = v_i + min(abs(v_{i+1} - v_i), a * dt) * sign(v_{i+1} - v_i)
        //
        // where v_i is the current velocity (aka motor output [-1, 1]),
        //       v_{i+1} is the next velocity to send to the motors,
        //       a is the max acceleration, and
        //       dt is the time delta in seconds since the velocity step.
        double allowedLeft = ComputeNextOutputDelta(
            currentLeft,
            desiredLeft,
            maxLeftAccel,
            timeDelta
        );

        double allowedRight = ComputeNextOutputDelta(
            currentRight,
            desiredRight,
            maxRightAccel,
            timeDelta
        );

        // Limit and scale the the motor output.
        allowedLeft = Limit(allowedLeft) * m_maxOutput;
        allowedRight = Limit(allowedRight) * m_maxOutput;

        // Update the motors.
        m_LeftMotors.Set(allowedLeft);
        m_RightMotors.Set(allowedRight);

        // Update dashboard vars.
        m_DashboardLeftOutput = allowedLeft;
        m_DashboardRightOutput = allowedRight;
        m_DashboardTimeDelta = timeDelta;
    #endif

    Feed();
}

void DriveTrain::StopMotor() {
    #ifdef USE_DRIVETRAIN_PID
        m_LeftPID.Disable();
        m_RightPID.Disable();
    #else
        ArcadeDrive(0, 0);
    #endif

    Feed();
}

void DriveTrain::GetDescription(wpi::raw_ostream& desc) const {
    desc << "DriveTrain";
}

void DriveTrain::InitSendable(frc::SendableBuilder& builder) {
    builder.SetSmartDashboardType("DifferentialDrive");
    builder.SetActuator(true);
    builder.SetSafeState([=] { StopMotor(); });

    #ifdef USE_DRIVETRAIN_PID
        builder.AddDoubleProperty(
            "Left Motor Speed",
            [=]() { return m_LeftPID.Get(); },
            nullptr
        );
        builder.AddDoubleProperty(
            "Right Motor Speed",
            [=]() { return m_RightPID.Get(); },
            nullptr
        );
    #else
        builder.AddDoubleProperty(
            "Left Motor Output (Limited)",
            [=]() { return m_DashboardLeftOutput; },
            nullptr
        );
        builder.AddDoubleProperty(
            "Right Motor Output (Limited)",
            [=]() { return m_DashboardRightOutput; },
            nullptr
        );
        builder.AddDoubleProperty(
            "Time Delta (s)",
            [=]() { return m_DashboardTimeDelta; },
            nullptr
        );
    #endif
}

void DriveTrain::RunReset() {
    // m_Drive.ArcadeDrive(0.0, 0.0, false);
    #ifdef USE_DRIVETRAIN_PID
        m_RightPID.Reset();
        m_LeftPID.Reset();
        m_RightPID.Enable();
        m_LeftPID.Enable();
    #else
        ArcadeDrive(0, 0);
    #endif
}

void DriveTrain::UseNormalSpeedLimit() {
    #ifdef USE_DRIVETRAIN_PID
    #else
        SetMaxOutput(m_MaxNormalSpeed);
    #endif
}

void DriveTrain::UseDukesSpeedLimit() {
    #ifdef USE_DRIVETRAIN_PID
    #else
        SetMaxOutput(m_SandstormStepSpeed);
    #endif
}

double DriveTrain::ComputeNextOutputDelta(double iVel, double fVel, double maxAccel, double timeDelta) {
    double deltaVel = fVel - iVel;
    return iVel + std::copysign(std::min(std::abs(deltaVel), maxAccel * timeDelta), deltaVel);
}
