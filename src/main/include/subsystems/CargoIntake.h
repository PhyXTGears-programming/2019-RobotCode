/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/commands/Subsystem.h>
#include <frc/DigitalInput.h>
#include "RobotMap.h"

#ifndef PROTOBOT
#   include <ctre/Phoenix.h>
#   include <frc/AnalogPotentiometer.h>
#   include <frc/Relay.h>
#   include <frc/Servo.h>
#endif

class CargoIntake : public frc::Subsystem {
    public:
        CargoIntake();
        void InitDefaultCommand() override;

        bool HasCargo();

        // HATCH functions

        // parallel w/ cargo-intake
        void GripHatchBottom() { m_HatchGripBottom.Set(0.5); }
        void GripHatchTop()    { m_HatchGripTop.Set(0.5); }
        // perpendicular (bottom goes down, top goes up)
        void ReleaseHatchBottom() { m_HatchGripBottom.Set(1); }
        void ReleaseHatchTop()    { m_HatchGripTop.Set(0); }

    private:
        frc::DigitalInput m_CargoSensor {kCargoSensor};
    
    // Protobot must not use any of this.
#   ifndef PROTOBOT

    public:

        void TurnOffIntakeRoller();
        void TurnOnIntakeRoller();

    private:
        frc::AnalogPotentiometer  m_IntakeRotation  {kCargoRotationSensor};
        frc::Servo                m_HatchGripBottom {kCargoHatchServoBottom};
        frc::Servo                m_HatchGripTop    {kCargoHatchServoTop};
        WPI_TalonSRX              m_IntakeArmMotor  {kCargoTalonRotate};
        frc::Relay                m_IntakeEject     {kCargoSpikeEjector};
        frc::Relay                m_IntakeRoller    {kCargoSpikeRoller};

#   endif

    // Add variables for servo positions
};
