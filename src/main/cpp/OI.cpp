#include "OI.h"

#include "OperatorHID.h"
#include <frc/WPILib.h>

OI::OI() {}

// Pressing buttons in Disabled sends the presses when re-enabling.
// That's bad, so we clear the presses when enabling.
void OI::ClearButtonBuffer() {
    for (int i = 1; i <= m_HidDriver.GetButtonCount(); i++) {
        m_HidDriver.GetRawButtonPressed(i);
        m_HidDriver.GetRawButtonReleased(i);
    }

    ButtonBoard& actionpad = m_HidOperator.GetActionPad();
    for (int i = 1; i <= actionpad.GetButtonCount(); i++) {
        actionpad.GetRawButtonPressed(i);
        actionpad.GetRawButtonReleased(i);
    }

    ButtonBoard& rotatepad = m_HidOperator.GetRotationPad();
    for (int i = 1; i <= rotatepad.GetButtonCount(); i++) {
        rotatepad.GetRawButtonPressed(i);
        rotatepad.GetRawButtonReleased(i);
    }

    frc::Joystick& flightstick = m_HidOperator.GetFlightStick();
    for (int i = 1; i <= flightstick.GetButtonCount(); i++) {
        flightstick.GetRawButtonPressed(i);
        flightstick.GetRawButtonReleased(i);
    }
}
