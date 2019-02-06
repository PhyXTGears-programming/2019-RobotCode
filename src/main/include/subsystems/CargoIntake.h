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

    private:
        frc::DigitalInput m_CargoSensor {kCargoSensor};
    
    // Protobot must not use any of this.
#   ifndef PROTOBOT

    public:

        void TurnOffIntakeRoller();
        void TurnOnIntakeRoller();

    private:
        frc::AnalogPotentiometer  m_intakeRotation   {0};
        frc::DigitalInput         m_intakeTrigger    {4};
        frc::Servo                m_hatchGripBack    {0};
        frc::Servo                m_hatchGripFront   {1};
        WPI_TalonSRX              m_intakeArmMotor   {6};
        frc::Relay                m_intakeEject      {2};
        frc::Relay                m_intakeRoller     {1};
#   endif
};
