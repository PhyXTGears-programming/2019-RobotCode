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

    ButtonBoard& board1 = m_HidOperator.GetBoard1();
    for (int i = 1; i <= board1.GetButtonCount(); i++) {
        board1.GetRawButtonPressed(i);
        board1.GetRawButtonReleased(i);
    }

    ButtonBoard& board2 = m_HidOperator.GetBoard2();
    for (int i = 1; i <= board2.GetButtonCount(); i++) {
        board2.GetRawButtonPressed(i);
        board2.GetRawButtonReleased(i);
    }

    frc::Joystick& flightstick = m_HidOperator.GetFlightStick();
    for (int i = 1; i <= flightstick.GetButtonCount(); i++) {
        flightstick.GetRawButtonPressed(i);
        flightstick.GetRawButtonReleased(i);
    }
}
