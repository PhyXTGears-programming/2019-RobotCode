/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "OI.h"

#include <frc/WPILib.h>

OI::OI() {}

frc::XboxController& OI::GetDriverJoystick() { return m_hidDriver; }

frc::XboxController& OI::GetOperatorJoystick() { return m_hidOperator; }
