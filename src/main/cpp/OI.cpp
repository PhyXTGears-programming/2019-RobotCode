#include "OI.h"

#include <frc/WPILib.h>

OI::OI() {}

// Return references to either the driver or operator Xbox joystick.
// E.g.: passing the joysticks to the drive subsystem.
frc::XboxController& OI::GetDriverJoystick() { return m_HidDriver; }
frc::XboxController& OI::GetOperatorJoystick() { return m_HidOperator; }
