#include "OI.h"

#include <frc/WPILib.h>

OI::OI() {}

// Return references to either the driver gamepad or operator joystick/button-board.
// E.g.: passing the joysticks to the drive subsystem.
frc::XboxController& OI::GetDriverJoystick() { return m_HidDriver; }
OperatorHID& OI::GetOperatorJoystick() { return m_HidOperator; }
